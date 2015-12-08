/* FILENAME: Drive.cpp
 * 
 * BRIEF: 
 * 
 * AUTHORS:
 * @author Tanuj Sane
 * @author Dominic Cupo
 * @author Annie Hernandez
 * @author Patrick Murphy
 * 
 * START DATE: Dec. 5, 2015
 */

#include "Drive.h"

SensorArray::SensorArray(
  byte frontPin, byte leftPin, byte rightPin, byte backPin){
  this->frontPin = frontPin;
  this->leftPin = leftPin;
  this->rightPin = rightPin;
  this->backPin = backPin;
}

SensorArray::update(){
  /*Sensors return vcc/512 V per inch with max of 254 inches
  Since analogRead is from 0 to 1023 or vcc/1024 split
  to get inches divide by 2. 
  Since this will always be 254 or less it fits in a byte
  */
  readings[FRONT_PIN] = (byte)(analogRead(frontPin)/2);
  readings[LEFT_PIN] = (byte)(analogRead(leftPin)/2);
  readings[RIGHT_PIN] = (byte)(analogRead(rightPin)/2);
  readings[BACK_PIN] = (byte)(analogRead(backPin)/2);
}

DriveController::DriveController(byte leftServoPin, byte rightServoPin){
  this->leftMotor.attach(leftServoPin,1000, 2000);
  this->rightMotor.attach(rightServoPin, 1000, 2000);
  Wire.begin();
  this->ultrasonics.setPins(FRONT_PIN,LEFT_PIN,RIGHT_PIN,BACK_PIN);
}

