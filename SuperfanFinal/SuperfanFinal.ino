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
#include <LiquidCrystal.h>
#include "Robot.h"
#include "Utils.h"

#define PULSE_TRIGGER 11
#define start 29

LiquidCrystal lcd(40, 41, 42, 43, 44, 45);
Robot superfan;

void setup() {
  lcd.begin(16 , 2);
  Wire.begin();
  superfan.init();
  lcd.print("Initiated");
  delay(500);
  pinMode(start, INPUT_PULLUP);
  lcd.clear();
  lcd.print("Wait on Button");
  while (digitalRead(start) == HIGH); //don't go until start is pressed
  lcd.clear();
  lcd.print("Firefighting!");
  pinMode(PULSE_TRIGGER, OUTPUT);
  digitalWrite(PULSE_TRIGGER, HIGH);
  delay(20);
  digitalWrite(PULSE_TRIGGER, LOW);
  pinMode(PULSE_TRIGGER, INPUT);//set to high Z state
}

void loop() {
  superfan.drive();
  
}
