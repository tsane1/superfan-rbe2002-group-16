#include "Utils.h"
LiquidCrystal lcd3(40, 41, 42, 43, 44, 45);

Tilter::Tilter(){}

void Tilter::init(){
  pinMode(tiltDirPin, OUTPUT);
  pinMode(tiltStepPin, OUTPUT);
  digitalWrite(tiltDirPin, LOW);
  digitalWrite(tiltStepPin, LOW);
}
void Tilter::step(bool dir){
  digitalWrite(tiltDirPin, dir ? HIGH : LOW);
  digitalWrite(tiltStepPin, HIGH);
  delay(10);
  digitalWrite(tiltStepPin, LOW);
  delay(10);
  numSteps+= dir? 1: -1;
}
void Tilter::goTo(int stepNum){
  while(stepNum>numSteps){//goal is higher than current
    step(up);
  }
  while(stepNum<numSteps){//goal is lower than current
    step(down);
  }
}
Gyro::Gyro(){}
void Gyro::init(){
  Serial.println("Gyro init");
  if (!gyro.init()) // gyro init
  {
    lcd3.print("No gyro");
    while (1); 
  }
  gyro.enableDefault();
  Serial.println("Done");
}
void Gyro::reset(){
  for(int i =0;i<2000;i++){  // takes 2000 samples of the gyro
    gyro.read();
    this->gerrz += gyro.g.z;
    delay(5);
  } 
  this->gerrz = (this->gerrz)/2000;
  gyro_zold = 0;
  lastReading = micros();
}

float Gyro::getReading(){
  this->gyro.read();
  long deltaReading = micros()-lastReading;
  lastReading = micros();
  this->gyro_z = (float)(this->gyro.g.z - this->gerrz) * this->G_gain * ((float)(deltaReading)/1000000);//time delay since last reading in seconds
  this->gyro_z += this->gyro_zold;
  this->gyro_zold = this->gyro_z;
  return (gyro_z );
}

 direction turnRight(direction dir){
  switch(dir){
    case pX: dir = mY; break;
    case mY: dir = mX; break;
    case mX: dir = pY; break;
    case pY: dir = pX; break; 
  }
  return dir;
}
 direction turnLeft(direction dir){
  switch(dir){
    case pX: dir = pY; break;
    case mY: dir = pX; break;
    case mX: dir = mY; break;
    case pY: dir = mX; break; 
  }
  return dir;
}
