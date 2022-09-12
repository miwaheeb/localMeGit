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
#include "SparkFun_Si7021_Breakout_Library.h"  // Added lib for Si7021 Temp and Humid

/********************************************************************************************************************
 *                                             pre-dec for LiquidCrystal
********************************************************************************************************************/
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article
/*******************************************************************************************************************/

/********************************************************************************************************************
 *                                         pre-dec for Si7021 Temp and Humid
*********************************************************************************************************************/
float humidity = 0;
float tempf = 0;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather sensor;
/********************************************************************************************************************/


void setup()
{
  // **********Serial for testing**********
  Serial.begin(9600);   // open serial over USB at 9600 baud
  //Initialize the I2C sensors and ping them
  sensor.begin();
  // **********Serial for testing**********

  
  // Set off LCD module
  lcd.begin (16,2); // 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
}
void loop()
{
/********************************************************************************************************************
*                                         void loop for Si7021 Temp and Humid
********************************************************************************************************************/
  //Get readings from all sensors
  getWeather();
  printInfo();
  delay(1000);
/*******************************************************************************************************************/

/********************************************************************************************************************
*                                            void loop for LiquidCrystal
********************************************************************************************************************/
   lcd.home (); // Set cursor to 0,0
   lcd.print("Temp (F): "); // Custom text
   lcd.print(tempf,1); // the 1 indicates only one decimal to be printed
   lcd.print(char(223)); // adds the percent
   
   lcd.setCursor (0,1); // Go to home of 2nd line
   lcd.print("Humidity: ");
   lcd.print(humidity,1); // the 1 indicates only one decimal to be printed
   lcd.print("%");
  
   lcd.setBacklight(HIGH); // Backlight on
   delay(5000); //reading every 5 seconds
/*******************************************************************************************************************/
}


/********************************************************************************************************************
*******************************************************************************************************************
*                                                   $$functions$$
*******************************************************************************************************************
********************************************************************************************************************/
//Si7021 Temp and Humid weather function
void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}

//Si7021 Temp and Humid print function
void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(tempf);
  Serial.print("F, ");

  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
}
