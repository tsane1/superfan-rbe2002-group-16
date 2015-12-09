/**  PidController - Library for using PID control to set motor positions
 *  This file contains declarations for the PidController class.
 *  
 *  Downloaded Oct. 10, 2015, from awtreth/RBE2001-Group16
 *  @author Mateus Amarante Araujo
 *  @author Dominic Cupo
 *  @author Tanuj Sane
 */
 
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PidController
{

  public:
		PidController();
	  PidController(double Kp, double Ki, double Kd);
	  void setTime(int dt_ms);
	  double calc(double target, double sensor_value);
	  double calc(double error);  
  	void reset();  
	  void setConstants(double Kp, double Ki, double Kd);  
	  void setLimits(double min, double max);	
	  void setSampleTime(int stime);//in ms
	  double kp, ki, kd;
    
  private:
	  double sum;
	  double last_time;
	  double last_error;
	  double max_output;
	  double min_output;
	  int sample_time;
	
	  double error;
	  double output;
	  double dt;
	  double now;
	  double last_output;
};

#endif

