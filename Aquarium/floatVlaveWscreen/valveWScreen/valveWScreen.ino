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
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*

#define port              D5
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article change addreess as needed, use I2C scanner
void setup()
{
   // Set off LCD module
   lcd.begin (16,2); // 16 x 2 LCD module
   lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
   lcd.setBacklight(HIGH);

   pinMode(port,INPUT_PULLUP);
}
void loop()
{
   lcd.home (); // Set cursor to 0,0
   lcd.print("Veros Love Meter:"); // Custom text
   if(digitalRead(port)== HIGH)
   {
    
    lcd.setCursor (0,1); // Go to home of 2nd line
    lcd.print("HIGHERRRRRRRRRRRRRRRRR");
    while(digitalRead(port)== HIGH) {}
    delay(25);
   }
   else
   {
    
    lcd.setCursor (0,1); // Go to home of 2nd line
    lcd.print("High                            ");
    while(digitalRead(port)== LOW) {}
    delay(25); 
   }
   //lcd.setCursor (0,1); // Go to home of 2nd line
   //lcd.print("I Love You Most");
   //delay(1000); // Blinks of backlight
   //lcd.setBacklight(LOW); // Backlight off
   delay(1);
   lcd.setBacklight(HIGH); // Backlight on
   delay(1);
}
