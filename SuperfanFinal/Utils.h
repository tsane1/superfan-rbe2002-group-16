#pragma once
#include "Wire.h"
#include "L3G.h"
#include "LiquidCrystal.h"
#define fanPin 10 //digital
#define tiltDirPin 7 //stepper motor direction
#define tiltStepPin 8 //stepper motor step pin
/**
 * Class to control the stepper motor
 */
class Tilter {
  public:
    Tilter();
    /**
     * Initializes the pins to proper states
     */
    void init();
    /**
     * Takes 1 step in given direction
     * @param dir the direction to step down is false up is true
     * Use the constants Tilter.down or Tilter.up
     */
    void step(bool dir);
    int numSteps;
    const bool down = false;
    const bool up = true;
    /**
     * Goes to a particular step
     * @param stepNum the step to go to. (the initial position on robot start is 0)
     */
    void goTo(int stepNum);
};
/**
 * Gyro class
 */
class Gyro {
  public:
    Gyro(); // default constructor
    /**
     * Initializes the gyro and sets to default settings
     */
    void init();
    /**
     * Resets the total of the gyro and resets the calibration of error
     */
    void reset();
    /**
     * Calculates the heading and returns it.
     */
    float getReading();
  private:
    L3G gyro;
    long lastReading;
    float gyro_z; //gyro x val
    float gyro_zold; //gyro cummulative z value
    float gerrz; // Gyro z error
    static const float G_gain = .00875; // gyros gain factor for 250deg/sec
};
enum direction {
  pX,
  mY,
  mX,
  pY
};
direction turnRight(direction dir);
direction turnLeft(direction dir);

