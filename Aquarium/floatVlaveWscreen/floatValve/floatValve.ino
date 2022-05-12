/*
Project: I2C LCD Backpack Arduino Experiment
By: T.K.Hareendran/TechNode Protolabz
For: https://www.electroschematics.com
Includes: Library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads*
Hardware/Controller: See article
Software/Sketch: Precisely adapted – Ref: Internet
Date: December 2015
*/
#include "Wire.h" // For I2C

#define port              D5

    /*
  Liquid Level Float Switch
  made on 13 oct 2020
  by Amir Mohammad Shojaee @ Electropeak
  Home
*/
void setup() {
 pinMode(port,INPUT_PULLUP);
 Serial.begin(9600);
}

void loop() {
  
  if(digitalRead(port)== HIGH){
    Serial.println("On Water");
    while(digitalRead(port)== HIGH) {}
    delay(100);
  }
  else{
    Serial.println("Outside Water");
    while(digitalRead(port)== LOW) {}
    delay(100); 
  }
  }
