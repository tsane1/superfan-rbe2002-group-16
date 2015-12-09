/* FILENAME: Robot.h
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
#pragma once
#include "Arduino.h"
#include <Servo.h>
#include <Wire.h>
#include "L3G.h"

using namespace std;

enum driveState{
  KEEP_GOING, FOUND_OPENING, OBSTACLE
};
class Tilter{//really just to make stepper motor stepping pretty
  public:
  void step(bool dir);//true = downwards
  int numSteps;
};
class Robot{
  public:
    byte wallDistances[4];
    
    Robot();//implemented
    void drive();
    void turn(int deg);
    boolean scanForFire();
    void extinguish();// goes to 8 inches away from fire and extinguishes (must be facing fire already)
    float getZ(byte dX);//implemented
  private:
    driveState updateUs();//implemented
    Tilter tilt;
    Servo left, right;
};

//pin defines
#define leftServoPin 4 //servos
#define rightServoPin 5
#define fanPin 10 //digital
#define frontPin 0 //analog
#define leftPin 1
#define rightPin 2
#define backPin 3
#define flameSensorPin 4
#define tiltDirPin 11 //stepper motor direction
#define tiltStepPin 12 //stepper motor step pin
