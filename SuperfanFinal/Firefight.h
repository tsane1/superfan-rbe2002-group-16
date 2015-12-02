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
    Fan(){} // unused, default constructor
    Fan(int stepPin1, int stepPin2, int fanPin)
    boolean sweep(); // Function to scan for fire using stepper
    boolean extinguish(); // Function to turn on fan and blow out fire
  private:
    int stepPin1, stepPin2, fanPin;
};

#endif

