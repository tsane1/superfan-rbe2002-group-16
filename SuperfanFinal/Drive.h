/* FILENAME: Drive.h
 * 
 * BRIEF: 
 * 
 * AUTHORS:
 * @author Tanuj Sane
 * @author Dominic Cupo
 * @author Annie Hernandez
 * @author Patrick Murphy
 * 
 * START DATE: Dec. 2, 2015
 */

#ifndef DRIVE_H
#define DRIVE_H

#include "Arduino.h"
#include <Servo.h>
#include <Wire.h>

#define PULSE_TRIGGER 11

enum sensorPins{
  FRONT_PIN = 0, LEFT_PIN, RIGHT_PIN, BACK_PIN
};
enum driveState{
  KEEP_GOING, FOUND_OPENING, OBSTACLE
};
enum turnState{
  LEFT, RIGHT, ABOUT_FACE_LEFT, ABOUT_FACE_RIGHT
};

class SensorArray{
  public:
    byte readings[4]; // sensor values
    
    SensorArray(); // default constructor
    void setPins(byte frontPin, byte leftPin, byte rightPin, byte backPin); // init pins
    void update(); // read new values, store to array

  private:
    byte frontPin, leftPin, rightPin, backPin;
};

class DriveController{
  public:
    DriveController(int leftServoPin, int rightServoPin); // default constructor
    driveState drive(); // drive until a stopping condition is met
    void turn(turnState dir); // turns robot 90 or 180 degrees in specified direction
    
  private:
    Servo leftMotor, rightMotor;
    SensorArray ultrasonics;
};
  
#endif
