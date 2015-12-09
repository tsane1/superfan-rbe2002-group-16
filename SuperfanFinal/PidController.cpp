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
	this->kp = 1;
	this->kd = 0;
	this->ki = 0;
	this->setLimits(-1023, 1023);//default values
	this->setSampleTime(DEFAULT_SAMPLE_TIME);
	this->reset();
}

PidController::PidController(double Kp, double Ki, double Kd)
{//TODO: check the limits
	this->setConstants(Kp, Ki, Kd);
	this->reset();
}

double PidController::calc(double error)
{
  return this->calc(error, 0);
}

double PidController::calc(double target, double sensor_value)
{
	now = millis();
	dt = now - last_time;
	
	if(dt >= sample_time)
	{
		error = target - sensor_value;
		sum += error;
		
		
		double output = kp*error + kd*(error-last_error)/dt + ki*sum*dt;
		
		output = constrain(output, min_output, max_output);
		
		last_output = output;
		
		last_error = error;
		last_time = now;
	}
	else return last_output;
	
	return output;
}
  
void PidController::reset()
{
	last_error = 0;
	last_time = millis();
	sum = 0;
	last_output = 0;
}

void PidController::setLimits(double min, double max)
{//TODO: check values
	min_output = min;
	max_output = max;
}

void PidController::setConstants(double Kp, double Ki, double Kd)
{
	this->kp = Kp;
	this->ki = Ki;
	this->kd = Kd;
}

void PidController::setSampleTime(int stime)//in ms
{//TODO: check stime is in a valid range
	this->sample_time = stime;
}
