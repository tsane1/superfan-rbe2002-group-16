/* FILENAME: Firefight.h
 * 
 * BRIEF: 
 * 
 * AUTHORS:
 * @author Tanuj Sane
 * @author Dominic Cupo
 * @author Annie Hernandez
 * @author Patrick Murphy
 * 
 * START DATE: Dec. 12, 2015
 */

#ifndef FIREFIGHT_H
#define FIREFIGHT_H

#include "Arduino.h"

class Fan{
  public:
    Fan(int stepPin1, int stepPin2, int stepPin3, int stepPin4, int fanPin, int flameSensorPin);
    boolean sweep(); // Function to scan for fire using stepper
    void extinguish(); // Function to turn on fan and blow out fire
  private:
    int stepPin1, stepPin2, stepPin3, stepPin4, fanPin, flameSensorPin;
};

#endif

