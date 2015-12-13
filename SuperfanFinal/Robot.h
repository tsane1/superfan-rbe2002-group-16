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
#include "Utils.h"


enum driveState {
  KEEP_GOING,
  TURN_LEFT,
  TURN_RIGHT,
  OBSTACLE
};

class Robot {
  public:
    int wallDistances[4];
    Robot();//implemented

    void init();
    void drive();
    void extinguish();// goes to 8 inches away from fire and extinguishes (must be facing fire already)
    void turn(float deg);
    void sweep();//sweeps the stepper to find the fire.

  private:
    driveState updateUs();//implemented
    boolean gotFire;
    void alignToFlame();
    PidController pid;
    Servo left, right;
    Gyro gyro;
    Tilter tilt;
    I2CEncoder lEnc, rEnc;
    direction dir = pX;
    boolean front;
    double x,y;
    float getZ(byte dX);//implemented
    void updateDist();
    double updateEnc();
    void resetEnc();
};


//pin defines
#define leftServoPin 4 //servos
#define rightServoPin 5

#define frontPin 0 //analog
#define leftPin 1
#define rightPin 2
#define backPin 3
#define sideFlameSensorPin 5
#define flameHeightSensorPin 4
//encoder is 1 rotation / 39.2 ticks * (36 teeth on motor / 60 teeth on wheel * 4.05 inch diameter * pi) <- inches per rotation
#define inchesPerTick  0.194740943877551
#define rightTurn 83//because for some reason gyro reading of 85 degrees is 90 degrees of real life
#define leftTurn -77.0
#define flameCutOff 900
