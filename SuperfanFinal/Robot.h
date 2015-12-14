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
    /**
     * Performs initialization on the robot
     */
    void init();
    /**
     * Calculates the motion the robot needs to perform and performs it
     * Call repeatedly in loop
     */
    void drive();
    /**
     * Goes to under 8 inches away from the fire and extinguishes the fire
     * Only call if already facing the fire.
     */
    void extinguish();
    /**
     * Turns the given number of degrees 
     * @param deg the number of degrees to turn (- is left + is right)
     */
    void turn(float deg);
    /**
     * Sweeps the stepper to find the fire
     */
    void sweep();
    Fan tilt;
    Servo left, right;
    I2CEncoder lEnc, rEnc;
  private:
    /**
    * Updates the ultrasonic sensors and returns the action which the robot should perform.
    */
    driveState updateUs();
    /**
     * Gets the z height of the fire
     * @param dX the distance in inches from the ultrasonic sensor to the flame base.
     */
    float getZ(byte dX);
    /**
     * Updates the distances X and Y based on encoder values and current direction
     */
    void updateDist();
    /**
     * Calculates the distance travelled using the average of the two encoder readings
     */
    double updateEnc();
    /**
     * Resets the encoders to zero
     */
    void resetEnc();
    /**
     * Once the flame has been spotted, determines the proper location to turn so that the flame will be facing the front of the robot.
     */
    void alignToFlame();
    boolean gotFire;
    PidController pid;
    
    Gyro gyro;
    
    
    direction dir = pX; // init to front facing, defined as positive x
    boolean front;
    double x,y; // location storage variables

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
