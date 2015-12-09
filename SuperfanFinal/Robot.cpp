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
/*this is the sum of distance from base of candle to flame
 * and from front ultrasonic sensor to pivot point of fan mount 
 * in the x direction
 */
#define offsetX 0/*TODO fill in real value*/
/* this is the offset from the floor to the pivot point 
 *  of the fan mount
 */
#define offsetY 0/*TODO fill in real value*/
/* converts from degrees to radians
 */
 #define initAngle 90.0
 #define degreesPerStep 1.8
 //distance from piv to sensor in straight line
 #define pivToSns 0 /*TODOD fill in real value*/
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

Robot::Robot(){
  left.attach(lServoPin, 1000, 2000);
  right.attach(rServoPin, 1000, 2000);
}
void Robot::updateUs(){
  /*Sensors return vcc/512 V per inch with max of 254 inches
  Since analogRead is from 0 to 1023 or vcc/1024 split
  to get inches divide by 2. 
  Since this will always be 254 or less it fits in a byte
  */
  wallDistances[fUsPin] = (byte)(analogRead(fUsPin)/2);
  wallDistances[lUsPin] = (byte)(analogRead(lUsPin)/2);
  wallDistances[rUsPin] = (byte)(analogRead(rUsPin)/2);
  wallDistances[bUsPin] = (byte)(analogRead(bUsPin)/2);
}

