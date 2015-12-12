/* FILENAME: Robot.cpp
 * 
 * BRIEF: 
 * 
 * AUTHORS:
 * @author Tanuj Sane
 * @author Dominic Cupo
 * @author Annie Hernandez
 * @author Patrick Murphy
 * 
 * START DATE: Dec. 8, 2015
 */
 #include "Robot.h"
 LiquidCrystal lcd2(40, 41, 42, 43, 44, 45);
 direction turnRight(direction dir){
  switch(dir){
    case pX: dir = mY; break;
    case mY: dir = mX; break;
    case mX: dir = pY; break;
    case pY: dir = pX; break; 
  }
  return dir;
}
 direction turnLeft(direction dir){
  switch(dir){
    case pX: dir = pY; break;
    case mY: dir = pX; break;
    case mX: dir = mY; break;
    case pY: dir = mX; break; 
  }
  return dir;
}
/* this is the sum of distance from base of candle to flame
 * and from front ultrasonic sensor to pivot point of fan mount 
 * in the x direction
 * ultra to pivot is 1.5 candle is 2.4 on average
 */
#define offsetX 3.9
/* this is the offset from the floor to the pivot point 
 *  of the fan mount
 */
#define offsetY 9
/* converts from degrees to radians
 */
 #define initAngle 90.0
 #define degreesPerStep 1.8
 //distance from piv to sensor in straight line
 #define pivToSns 3.5 /*TODOD fill in real value*/
 #define degToRad(deg) (deg*PI/180)
 #define stepsToDeg(steps) (degreesPerStep*steps)
/*Gets the height of the flame based on distance
* from robot of fan mount to the base of the candle
* for derivation of this formula see attached file
* @param dX the displacement from ultrasonic sensor to candle base
*/
float Robot::getZ(byte dX){
  float theta = initAngle - stepsToDeg((float)tilt.numSteps);
  float snsX = offsetX + dX + //distance between pivot and flame
  //plus the distance between flame sensor and pivot in x direction
  //gives distance in x between candle and sensor
  ((pivToSns)/sin(degToRad(90.0-stepsToDeg(tilt.numSteps))));
  float yFromPivotToFlame = sin(degToRad(90-theta))*
  ((snsX)/sin(degToRad(theta)));//from law of sines
  return offsetY + yFromPivotToFlame;
  //total is offset from ground to pivot + y from pivot to flame
}
Tilter::Tilter(){

}
void Tilter::init(){
  pinMode(tiltDirPin, OUTPUT);
  pinMode(tiltStepPin, OUTPUT);
  digitalWrite(tiltDirPin, LOW);
  digitalWrite(tiltStepPin, LOW);
}
void Tilter::step(bool dir){
  digitalWrite(tiltDirPin, dir ? HIGH : LOW);
  digitalWrite(tiltStepPin, HIGH);
  delay(10);
  digitalWrite(tiltStepPin, LOW);
  delay(10);
  numSteps+= dir? 1: -1;
}
void Tilter::goTo(int stepNum){
  while(stepNum>numSteps){//goal is higher than current
    step(up);
  }
  while(stepNum<numSteps){//goal is lower than current
    step(down);
  }
}
Gyro::Gyro(){
}
void Gyro::init(){
  Serial.println("Gyro init");
  if (!gyro.init()) // gyro init
  {
    lcd2.print("No gyro");
    while (1); 
  }
  gyro.enableDefault();
  //reset();
  Serial.println("Done");
}
void Gyro::reset(){
  for(int i =0;i<2000;i++){  // takes 2000 samples of the gyro
    gyro.read();
    this->gerrz += gyro.g.z;
    delay(5);
  } 
  this->gerrz = (this->gerrz)/2000;
  gyro_zold = 0;
  lastReading = micros();
}

float Gyro::getReading(){
  this->gyro.read();
  long deltaReading = micros()-lastReading;
  lastReading = micros();
  this->gyro_z = (float)(this->gyro.g.z - this->gerrz) * this->G_gain * ((float)(deltaReading)/1000000);//time delay since last reading in seconds
  this->gyro_z += this->gyro_zold;
  this->gyro_zold = this->gyro_z;
  return (gyro_z );
}

void Gyro::resetHeading(){
  gyro_zold = 0;
}
Robot::Robot(){
}
void Robot::init(){  
  x = 0;
  y = 0;
  left.attach(leftServoPin, 1000, 2000);
  right.attach(rightServoPin, 1000, 2000);
  lEnc.init(inchesPerTick, MOTOR_393_TIME_DELTA);
  rEnc.init(inchesPerTick, MOTOR_393_TIME_DELTA);
  lEnc.setReversed(true);
  gyro.init();
  tilt.init();
  gotFire = false;
  this->pid.setConstants(0.6, 0.005, 0.7);
  this->pid.setLimits(-30,30);
}

void Robot::goFwd(){
  left.write(80);
  right.write(100);
}
driveState Robot::updateUs(){
  /*Sensors return vcc/512 V per inch with max of 254 inches
  Since analogRead is from 0 to 1023 or vcc/1024 split
  to get inches divide by 2. 
  Since this will always be 254 or less it fits in a byte
  */
  wallDistances[frontPin] = (analogRead(frontPin)/2);
  wallDistances[leftPin] = (analogRead(leftPin)/2);
  wallDistances[rightPin] = (analogRead(rightPin)/2);
  wallDistances[backPin] = (analogRead(backPin)/2);
  //this->front = this->front || wallDistances[frontPin] < 10;
  lcd2.clear();
  lcd2.setCursor(14,1);
  lcd2.print(wallDistances[rightPin]);
  if(wallDistances[rightPin] > 20) return TURN_RIGHT;
  else if(wallDistances[frontPin] < 8){
    if(front){
      front = false;
      return TURN_LEFT;
    }
    else{
      front = true;
      return KEEP_GOING;
    }
  }
  else return KEEP_GOING;
}

void Robot::drive(){
  if(analogRead(sideFlameSensorPin)<flameCutOff){
    alignToFlame();
    turn(leftTurn);
    turnLeft(dir);
    extinguish();
  }
  
  switch(this->updateUs()){
    case KEEP_GOING:this->left.write(65); this->right.write(115);break;
    case TURN_LEFT: this->turn(leftTurn); turnLeft(dir); break; // left
    case TURN_RIGHT: delay(150); this->turn(rightTurn); turnRight(dir); break; // right
  }
}
void Robot::alignToFlame(){//TODO: implement
  left.write(90);
  right.write(90);
  updateDist();
  resetEnc();
  int minReading = flameCutOff;
  int minDisp = 0;
  byte index = 0;
  bool done = false;
  while(!done){
    int temp = analogRead(sideFlameSensorPin);
    delay(1);
    temp+= analogRead(sideFlameSensorPin);
    delay(1);
    temp+= analogRead(sideFlameSensorPin);
    temp = temp/3;
    if(temp<minReading){
      minReading = temp;
      minDisp = updateEnc();
    }
    if(temp>flameCutOff)
      done = true;//you have gone past the candle
    index++;
    left.write(70);
    right.write(110);
    while(updateEnc()<0.5*index);
    left.write(90);
    right.write(90);
  }
  left.write(110);
  right.write(70);
  while(updateEnc()>minDisp);
}


void Robot::turn(float deg){
  this->left.write(90);
  this->right.write(90);
  delay(500);
  updateDist();
  lcd2.clear();
  lcd2.print("Turning: ");
  lcd2.print(deg<0 ? "left" : "right");
  this->pid.reset();
  this->gyro.reset();
  float gyroVal;
  do{
    gyroVal = gyro.getReading();
    lcd2.clear();
    lcd2.print("G");
    lcd2.print(gyroVal);
    double control = this->pid.calc((double)(deg - gyroVal));
    lcd2.print(" C");
    lcd2.print(control);
    this->left.write(90 - control);
    this->right.write(90 - control);
    delay(5);
  }
  while(abs(deg - gyroVal) > 1);
  resetEnc();
  if(deg>0){//right turn only because then needs to reestablish a wall contact.
  this->left.write(65);
  this->right.write(115);
  delay(2000);
  }
  this->left.write(90);
  this->right.write(90);
}
void Robot::resetEnc(){
  lEnc.zero();
  rEnc.zero();
}
void Robot::updateDist(){
  switch(dir){
    case pX: x+= updateEnc(); break;
    case pY: y+= updateEnc(); break;
    case mX: x-=updateEnc(); break;
    case mY: y-=updateEnc(); break;
  }
}
double Robot::updateEnc(){
  return (lEnc.getPosition()+rEnc.getPosition())/2;//average distance turned
}
void Robot::sweep(){
  tilt.goTo(-25);
  int minValue = 10000;
  int minStep = -75;
  while(tilt.numSteps<30){
    int temp = analogRead(flameHeightSensorPin);
    lcd2.clear();
    lcd2.print("R");
    lcd2.print(temp);
    lcd2.print("S");
    lcd2.print(tilt.numSteps);
    if(temp<minValue){
      minValue = temp;
      minStep = tilt.numSteps;
    }
    delay(500);
    tilt.step(tilt.up);
  }
  tilt.goTo(minStep);
    lcd2.clear();
    lcd2.print("R");
    lcd2.print(analogRead(flameHeightSensorPin));
    lcd2.print("S");
    lcd2.print(tilt.numSteps);
}
void Robot::extinguish(){
sweep();
while(true);
}

