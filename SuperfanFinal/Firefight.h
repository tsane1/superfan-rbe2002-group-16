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

enum ScanStates {
  FOUND_FIRE, SCANNING, NO_FIRE
};
enum FireStatus {
  FLAME_ON, FLAME_OFF
};

class Fighter{
  public:
    Fighter(){} // default constructor
    Fighter(int stepPin1, int stepPin2, int fanPin)
    int sweep(); // Function to scan for fire using stepper
    int extinguish(); // Function to turn on fan and blow out fire
  private:
    int stepPin1, stepPin2, fanPin;
};

#endif

