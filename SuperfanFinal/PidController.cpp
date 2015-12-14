/**  PidController - Library for using PID control to set motor positions
 *  This file contains implementation of PidController objects.
 *
 *  Downloaded Oct. 10, 2015, from awtreth/RBE2001-Group16
 *  @author Mateus Amarante Araujo
 *  @author Dominic Cupo
 *  @author Tanuj Sane
 */

#include "PidController.h"
#include "Arduino.h"

#define DEFAULT_SAMPLE_TIME 75

PidController::PidController()
{
  this->kp = 1; //constant for Proportion control
  this->kd = 0; //constant for Derivative control
  this->ki = 0; //constant for Integral control
  this->setLimits(-90, 90);//default values
  this->setSampleTime(DEFAULT_SAMPLE_TIME);
  this->reset();
}

PidController::PidController(double Kp, double Ki, double Kd)
{
  this->setConstants(Kp, Ki, Kd); //when created, uses default values
  this->reset(); //instantiates and clears cache
}

double PidController::calc(double error)
{
  return this->calc(error, 0); //calculates off of given error, in effect same as setpoint and actual
}

double PidController::calc(double target, double sensor_value)
{
  now = millis(); //takes current time
  dt = now - last_time; //finds the time step

  if (dt >= sample_time) //if it's been long enough to calculate the next step
  {
    error = target - sensor_value; //calculates error from difference in actual location and set point
    sum += error; //sums up the error

    //calculates the output by multiplying error times kp +
                             //the difference in the rate of error to last error times kd +
                             //the sum of all errors times the time step times ki
    double output = kp * error + kd * (error - last_error) / dt + ki * sum * dt;

    output = constrain(output, min_output, max_output); //constrains the output to values the Servos can understand

    last_output = output; //stores the output for reference later
    last_error = error;   //stores the error for reference later
    last_time = now;      //stores the last time for reference later
    
    return output; //returns the output and exits the function
  }
  else return last_output; //if not enough time has passed just return the previous calculation
}

void PidController::reset() //resets the continuous variables to 0 or their set values
{
  last_error = 0;
  last_time = millis();
  sum = 0;
  last_output = 0;
}

void PidController::setLimits(double min, double max) //sets the contraint limits
{
  min_output = min;
  max_output = max;
}

void PidController::setConstants(double Kp, double Ki, double Kd) //sets the constants for interal reference later
{
  this->kp = Kp;
  this->ki = Ki;
  this->kd = Kd;
}

void PidController::setSampleTime(int stime)//in ms, sets the sample time
{
  this->sample_time = stime;
}
