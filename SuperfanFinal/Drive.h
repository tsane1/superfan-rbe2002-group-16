/* FILENAME: Drive.h
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

#ifndef DRIVE_H
#define DRIVE_H

#include "Arduino.h"
#include <Wire.h>

#define PULSE_TRIGGER 11

enum sensorPins{
  FRONT_PIN, LEFT_PIN, RIGHT_PIN, BACK_PIN
};

class SensorArray{
  public:
    int readings[4]; // sensor values
    SensorArray(); // default constructor
    void update(); // read new values, store to array
    void start(); // starts reading cycle
};
  
#endif
