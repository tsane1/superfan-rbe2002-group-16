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

Robot::Robot() {
}

void Robot::init() { //this is called to initialize all the variables needed for the robot
  x = 0; //the X pos relative to start
  y = 0; //the Y pos relative to start
  left.attach(leftServoPin, 1000, 2000); //attaches the motor to the correct pin, with additional info for the 393 motor use
  right.attach(rightServoPin, 1000, 2000);
  lEnc.init(inchesPerTick, MOTOR_393_TIME_DELTA); //initializes the encoders built into the motors with the correct motor type and wheel size constant
  rEnc.init(inchesPerTick, MOTOR_393_TIME_DELTA);
  lEnc.setReversed(true); //sets the left encoder to be reversed so that it counts positive when the robot drives forward
  gyro.init(); //initilizes the gyro
  tilt.init(); //initilizes the stepper motor object
  gotFire = false; //sets initial condition for if fire is found
  this->pid.setConstants(0.6, 0.005, 0.7); //sets predetermined conditions for the pid object
  this->pid.setLimits(-30, 30); //sets max speed limits. -30 and 30 allow medium speed, giving us greater accuracy
}

driveState Robot::updateUs() {
  /*Sensors return vcc/512 V per inch with max of 254 inches
  Since analogRead is from 0 to 1023 or vcc/1024 split
  to get inches divide by 2.
  Since this will always be 254 or less it fits in a byte
  */
  wallDistances[frontPin] = (analogRead(frontPin) / 2);
  wallDistances[leftPin] = (analogRead(leftPin) / 2);
  wallDistances[rightPin] = (analogRead(rightPin) / 2);
  wallDistances[backPin] = (analogRead(backPin) / 2);
  //this->front = this->front || wallDistances[frontPin] < 10;
  lcd.clear();
  lcd.setCursor(14, 1);
  lcd.print(wallDistances[rightPin]); //lets us know the distance to the right wall (which we are following)
  if (wallDistances[rightPin] > 20) return TURN_RIGHT; //if we've lost the wall, execute the turn to the right
  else if (wallDistances[frontPin] < 12) { //otherwise, if were about to run into a wall, turn to the left
    if (front) { 
      front = false;
      return TURN_LEFT;
    }
    else {
      front = true;
      return KEEP_GOING;
    }
  }
  else return KEEP_GOING;
}

void Robot::drive() {
  if (analogRead(sideFlameSensorPin) < flameCutOff) { //if we've found the candle to the left of us
    alignToFlame(); //turn to face the candle directly
    turn(leftTurn);
    --dir; //set correct direction
    extinguish(); //run extinguish function
  }

  switch (this->updateUs()) {
    case KEEP_GOING: this->left.write(62); this->right.write(115); break; //keep driving straight until issue found
    case TURN_LEFT: this->turn(leftTurn); --dir; break; // left
    case TURN_RIGHT: this->turn(rightTurn); ++dir; break; // right
  }
}

void Robot::alignToFlame() {
  left.write(90); //stop moving
  right.write(90);
  updateDist(); //find distance to candle
  resetEnc(); //reset the encoders
  int minReading = flameCutOff; //set the min reading
  int minDisp = 0;
  byte index = 0;
  bool done = false;
  while (!done) { //if it still isn't passed
    int temp = analogRead(sideFlameSensorPin); //take three readings and average them
    delay(1);
    temp += analogRead(sideFlameSensorPin);
    delay(1);
    temp += analogRead(sideFlameSensorPin);
    temp = temp / 3;
    if (temp < minReading) { //if a new min reading is found
      minReading = temp;
      minDisp = updateEnc();
    }
    if (temp > flameCutOff)
      done = true;//you have gone past the candle
    index++;
    left.write(70);
    right.write(110);
    while (updateEnc() < 0.5 * index - 0.5);
    left.write(90);
    right.write(90);
  }
  left.write(110);
  right.write(70);
  while (updateEnc() > minDisp);
}

void Robot::turn(float deg) {
  this->left.write(90); //stop moving
  this->right.write(90);
  delay(500); //wait to settle
  updateDist(); //update distance to go
  lcd.clear(); //state where you're going
  lcd.print("Turning: ");
  lcd.print(deg < 0 ? "left" : "right");
  this->pid.reset();//reset pid
  this->gyro.reset();//reset gyro readings, prevents massive error over time
  float gyroVal;
  do {
    gyroVal = gyro.getReading(); //get the reading of rotational pos
    lcd.clear();
    lcd.print("G");
    lcd.print(gyroVal);
    double control = this->pid.calc((double)(deg - gyroVal)); //calc pid
    lcd.print(" C");
    lcd.print(control);
    this->left.write(90 - control); //turn the correct way
    this->right.write(90 - control);
    delay(5); //wait to prevent spamming
  }
  while (abs(deg - gyroVal) > 1); //check if you're within one degree of pos
 
  resetEnc();
  
  if (deg > 0) { //right turn only because then needs to reestablish a wall contact.
    this->left.write(62);
    this->right.write(115);
    delay(2500); //drives a little to move
  }
  this->left.write(90);
  this->right.write(90);
}

void Robot::sweep() { //this sweeps the fan to find the height
  tilt.enable(); //enables the stepper motor, usually disabled to prevent battery drain
  tilt.goTo(-25); //go to -25 degrees 
  int minValue = 10000; //values for reference
  int minStep = -75;
  while (tilt.numSteps < 30) { //take 30 steps, rotating upward, rougly 60 degrees
    int temp = analogRead(flameHeightSensorPin); //store the temp reading
    if (temp < minValue) { //if heat still increasing
      minValue = temp; //store reading for refernce in next loop
      minStep = tilt.numSteps; //store which step it was in
    }
    tilt.step(tilt.up); //keep tilting up to check
  }
  tilt.goTo(minStep); //once it's all done go to the step with the most heat, right at the candle
  tilt.disable(); //shut off the stepper so no drain to battery
}

void Robot::extinguish() {
  resetEnc();
  lcd.clear();
  while(updateEnc() < 12. && wallDistances[frontPin] > 8){
    left.write(62);
    right.write(115);
    lcd.print(updateEnc());
    this->updateUs();
  }
  left.write(90);
  right.write(90);
  updateDist();
  sweep();
  do{
    tilt.on();
    lcd.print(analogRead(flameHeightSensorPin));
    delay(2500);
    tilt.off();
    lcd.clear();
    delay(500);
  }
  while(analogRead(flameHeightSensorPin) < flameCutOff);

  lcd.print("FIRE'S OUT!");
  tilt.off();
  delay(1000);
  lcd.clear();
  updateUs();
  lcd.print(getZ(wallDistances[frontPin]));
 
 /********************************/
 while(true);//stop doing things.
 /********************************/

}

void Robot::resetEnc() {
  lEnc.zero();
  rEnc.zero();
}

void Robot::updateDist() {
  switch (dir) { //depending on what direction were looking at, add or decrease pos
    case pX: x += updateEnc(); break;
    case pY: y += updateEnc(); break;
    case mX: x -= updateEnc(); break;
    case mY: y -= updateEnc(); break;
  }
}

double Robot::updateEnc() {
  return (lEnc.getPosition() + rEnc.getPosition()) / 2; //average distance turned
}

/* this is the sum of distance from base of candle to flame
 * and from front ultrasonic sensor to pivot point of fan mount
 * in the x direction
 * ultra to pivot is 2.5 candle is 2.0 
 */
#define offsetX 4.5
/* this is the offset from the floor to the pivot point
 *  of the fan mount
 */
#define offsetY 9
#define initAngle 90.0
#define degreesPerStep 1.8
//distance from piv to sensor in straight line
#define pivToSns 3 
/* converts from degrees to radians*/
#define degToRad(deg) (deg*PI/180)
#define stepsToDeg(steps) (degreesPerStep*steps)
/**Gets the height of the flame based on distance
* from robot of fan mount to the base of the candle
* for derivation of this formula see attached file
* @param dX the displacement from ultrasonic sensor to candle base
*/
float Robot::getZ(byte dX) {
  float theta = initAngle - stepsToDeg((float)tilt.numSteps);
  float snsX = offsetX + dX + //distance between pivot and flame
               //plus the distance between flame sensor and pivot in x direction
               //gives distance in x between candle and sensor
               ((pivToSns) / sin(degToRad(theta)));
  float yFromPivotToFlame = sin(degToRad(90 - theta)) *
                            ((snsX) / sin(degToRad(theta))); //from law of sines
  return offsetY + yFromPivotToFlame;
  //total is offset from ground to pivot + y from pivot to flame
}


