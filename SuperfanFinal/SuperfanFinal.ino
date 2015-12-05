/* FILENAME: SuperfanFinal.ino
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

#include <Servo.h>
#include <Wire.h>
#include "Firefight.h"
#include "Drive.h"

DriveController robot();
Fan fan(/*TODO: Choose pins*/);
SensorArray ultrasonics();

void setup() {
  pinMode(PULSE_TRIGGER, OUTPUT);
  digitalWrite(PULSE_TRIGGER, HIGH);
  delay(20);
  digitalWrite(PULSE_TRIGGER, LOW);
}

void loop() {
  
}
