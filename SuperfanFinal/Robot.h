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

enum direction{
  FORWARD, LEFT, RIGHT, BACKWARD
};

enum driveState{
  KEEP_GOING, TURN_LEFT, TURN_RIGHT, OBSTACLE
};

class Tilter{
  public:
  void step(bool dir);//true = downwards
  int numSteps;
};

class Gyro{
  public:
    Gyro(); // default constructor
    void init(); // initialize gyro
    float getReading(); // get and calculate reading
    
  private:
    L3G gyro;
    float gyro_z; //gyro x val
    float gyro_zold; //gyro cummulative z value
    float gerrz; // Gyro 7 error
    float G_gain=.00875; // gyros gain factor for 250deg/sec
    float G_Dt=0.005;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible
};

class Robot{
  public:
    byte wallDistances[4];
    
    Robot();//implemented
    void drive();
    boolean scanForFire();
    void extinguish();// goes to 8 inches away from fire and extinguishes (must be facing fire already)
    float getZ(byte dX);//implemented
    
  private:
    driveState updateUs();//implemented
    void turn(int deg);
    void turn(direction dir);
    Tilter tilt;
    Servo left, right;
    Gyro gyro;
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
