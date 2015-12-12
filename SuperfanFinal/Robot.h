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
#include <L3G.h>
#include <I2CEncoder.h>
#include <LiquidCrystal.h>
#include "PidController.h"

enum direction {
  FORWARD,
  LEFT,
  RIGHT,
  BACKWARD
};

enum driveState {
  KEEP_GOING,
  TURN_LEFT,
  TURN_RIGHT,
  OBSTACLE
};

enum mode {
  TESTDRIVE,
  DRIVE2FIRE,
  SWEEPING,
  EXT
};

class Tilter {
  public:
    Tilter();
    void init();
    void step(bool dir);//true = downwards
    int numSteps;
};

class Gyro {
  public:
    Gyro(); // default constructor
    void init();
    void reset(); // initialize gyro
    float getReading(); // get and calculate reading

  private:
    L3G gyro;
    long lastReading;
    float gyro_z; //gyro x val
    float gyro_zold; //gyro cummulative z value
    float gerrz; // Gyro 7 error
    static const float G_gain = .00875; // gyros gain factor for 250deg/sec
    static const float G_Dt = 0.005;  // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

};

class Robot {
  public:
    int wallDistances[4];
    Robot();//implemented

    void init();
    void drive();
    void goFwd();//test only.
    boolean scanForFire();
    void extinguish();// goes to 8 inches away from fire and extinguishes (must be facing fire already)
    float getZ(byte dX);//implemented
    void turn(float deg);
  private:
    driveState updateUs();//implemented
    
    boolean gotFire;
    Tilter tilt;
    PidController pid;
    Servo left, right;
    Gyro gyro;
    I2CEncoder lEnc, rEnc;
    boolean front;
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
#define tiltDirPin 8 //stepper motor direction
#define tiltStepPin 9 //stepper motor step pin
//encoder is 1 rotation / 39.2 ticks * (36 teeth on motor / 60 teeth on wheel * 4.05 inch diameter * pi) <- inches per rotation
#define inchesPerTick  0.194740943877551
#define rightTurn 85//because for some reason gyro reading of 82 degrees is 90 degrees of real life
#define leftTurn -84.7
