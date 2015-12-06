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

SensorArray::SensorArray(int frontPin, int leftPin, int rightPin, int backPin){
  this->frontPin = frontPin;
  this->leftPin = leftPin;
  this->rightPin = rightPin;
  this->backPin = backPin;
}

SensorArray::update(){
  
}

DriveController::DriveController(int leftServoPin, int rightServoPin){
  this->leftMotor.attach(leftServoPin);
  this->rightMotor.attach(rightServoPin);
  Wire.begin();
  this->ultrasonics.setPins(0,1,2,3);
}

