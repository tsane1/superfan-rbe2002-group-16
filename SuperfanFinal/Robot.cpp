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
  float theta = initAngle + stepsToDeg(tilt.numSteps);
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

Robot::Robot(){
}
void Robot::init(){  
  left.attach(leftServoPin, 1000, 2000);
  right.attach(rightServoPin, 1000, 2000);
  lEnc.init(inchesPerTick, MOTOR_393_TIME_DELTA);
  rEnc.init(inchesPerTick, MOTOR_393_TIME_DELTA);
  gyro.init();
  tilt.init();
  gotFire = false;
  this->pid.setConstants(0.6, 0.005, 0.7);
  this->pid.setLimits(-30,30);
}

void Robot::goFwd(){
  left.write(60);
  right.write(120);
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
  lcd2.setCursor(0,0);
  lcd2.print(wallDistances[frontPin]);
  if(wallDistances[rightPin] > 40) return TURN_RIGHT;
  else if(wallDistances[frontPin] < 10) return TURN_LEFT;
  else return KEEP_GOING;
}

void Robot::drive(){
  this->left.write(60);
  this->right.write(120);
  switch(this->updateUs()){
    case KEEP_GOING: break;
    case TURN_LEFT: this->turn(-rightAngle); break; // left
    case TURN_RIGHT: this->turn(rightAngle); break; // right
  }
}



void Robot::turn(int deg){
  this->left.write(90);
  this->right.write(90);
  delay(500);
  //lcd2.clear();
  //lcd2.print("Gyro Reset");
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
  this->left.write(60);
  this->right.write(120);
  delay(2000);
  this->left.write(90);
  this->right.write(90);
}

boolean Robot::scanForFire(){

}

void Robot::extinguish(){

}

