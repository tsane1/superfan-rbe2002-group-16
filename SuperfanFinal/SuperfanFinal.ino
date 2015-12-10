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
 * START DATE: Dec. 1, 2015
 */

#include <Servo.h>
#include <Wire.h>
#include <L3G.h>
#include <I2CEncoder.h>
#include "Robot.h"

#define PULSE_TRIGGER 11

Robot superfan;

void setup() {
  pinMode(PULSE_TRIGGER, OUTPUT);
  digitalWrite(PULSE_TRIGGER, HIGH);
  delay(20);
  digitalWrite(PULSE_TRIGGER, LOW);
  pinMode(PULSE_TRIGGER, INPUT);//set to high Z state
}

long timer = 0;
long lastTime = 0;
long elapsedTime;

void loop() {
  //superfan.drive();
  superfan.goFwd();
}
