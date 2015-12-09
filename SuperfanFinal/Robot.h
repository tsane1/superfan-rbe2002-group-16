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
#include "Servo.h"
#include "Wire.h"
#include "L3G.h"
enum driveState{
  KEEP_GOING, FOUND_OPENING, OBSTACLE
};

class Robot{
  public:
  Robot();
  byte wallDistances[4];
  driveState drive();
  void turn(int deg);
  boolean scanForFire();
  void extinguish();//goes to 8 inches away from fire and extinguishes (must be facing fire already)
  float getZ();
  private:
  void updateUs();
  Tilter tilt;
  Servo left, right;
}

class Tilter{//really just to make stepper motor stepping pretty
  public:
  void step(bool dir);//true = downwards
  int numSteps;
}
//pin defines
#define lServoPin 4//servos
#define rServoPin 5
#define fanPin 10//digital
#define fUsPin 0//analog
#define lUsPin 1
#define rUsPin 2
#define bUsPin 3
#define flameSensorPin 4
#define tiltDirPin 11//stepper motor direction
#define tiltStepPin 12//stepper motor step pin
