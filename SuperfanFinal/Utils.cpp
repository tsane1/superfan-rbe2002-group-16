#include "Utils.h"
//Fan section
Fan::Fan() {}

void Fan::init() {
  pinMode(tiltDirPin, OUTPUT);
  pinMode(tiltStepPin, OUTPUT);
  pinMode(stepperEnablePin, OUTPUT);
  digitalWrite(tiltDirPin, LOW);
  digitalWrite(tiltStepPin, LOW);
  digitalWrite(stepperEnablePin, HIGH);
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);
}

void Fan::step(bool dir) {
  digitalWrite(tiltDirPin, dir ? HIGH : LOW);
  digitalWrite(tiltStepPin, HIGH);
  delay(10);
  digitalWrite(tiltStepPin, LOW);
  delay(10);
  numSteps += dir ? 1 : -1;
}

void Fan::goTo(int stepNum) {
  while (stepNum > numSteps) { //goal is higher than current
    step(up);
  }
  while (stepNum < numSteps) { //goal is lower than current
    step(down);
  }
}

void Fan::on() {
  digitalWrite(fanPin, HIGH);
}

void Fan::off() {
  digitalWrite(fanPin, LOW);
}

void Fan::enable(){
  digitalWrite(stepperEnablePin, LOW);
}

void Fan::disable(){
  digitalWrite(stepperEnablePin, HIGH);
}

//Gyro section
Gyro::Gyro() {}

void Gyro::init() {
  Serial.println("Gyro init");
  if (!gyro.init()) // gyro init
  {
    lcd.print("No gyro");
    while (1);
  }
  gyro.enableDefault();
  Serial.println("Done");
}

void Gyro::reset() {
  for (int i = 0; i < 1000; i++) { // takes 2000 samples of the gyro
    gyro.read();
    this->gerrz += gyro.g.z;
    delay(5);
  }
  this->gerrz = (this->gerrz) / 1000;
  gyro_zold = 0;
  lastReading = micros();
}

float Gyro::getReading() {
  this->gyro.read();
  long deltaReading = micros() - lastReading;
  lastReading = micros();
  this->gyro_z = (float)(this->gyro.g.z - this->gerrz) * this->G_gain * ((float)(deltaReading) / 1000000); //time delay since last reading in seconds
  this->gyro_z += this->gyro_zold;
  this->gyro_zold = this->gyro_z;
  return (gyro_z );
}

//Miscellaneous section
direction turnRight(direction dir) {
  switch (dir) {
    case pX: dir = mY; break;
    case mY: dir = mX; break;
    case mX: dir = pY; break;
    case pY: dir = pX; break;
  }
  return dir;
}

direction turnLeft(direction dir) {
  switch (dir) {
    case pX: dir = pY; break;
    case mY: dir = pX; break;
    case mX: dir = mY; break;
    case pY: dir = mX; break;
  }
  return dir;
}
