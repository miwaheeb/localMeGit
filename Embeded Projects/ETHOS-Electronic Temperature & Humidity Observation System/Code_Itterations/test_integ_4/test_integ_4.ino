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

/********************************************************************************************************************
 *                                                     PuTTy setup
*********************************************************************************************************************/
char response;
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

  // PuTTY setup
  //puTTy();
  //puTTy_menu();
  
}
void loop()
{
/********************************************************************************************************************
*                                         void loop for Si7021 Temp and Humid
********************************************************************************************************************/
  //Get readings from all sensors
  getWeather();
  puTTy();
  
  //delay(1000);
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

//PuTTy menu function
void puTTy()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    puTTy_menu();
    response = char(Serial.read());

    // say what you got:
    Serial.print("I received: ");
    Serial.println(char(response));
  }
  

  //Serial.print("Temp:");
  //Serial.print(tempf);
  //Serial.print("F, ");

  //Serial.print("Humidity:");
  //Serial.print(humidity);
  //Serial.println("%");
}

void puTTy_menu()
{
  Serial.print("Welcome to ETHOS\n\r");
  Serial.print("The Electronic Temperature & Humidity Observation System\n\r");
  Serial.print("\n\r");
  Serial.print("Your options:\n\r");
  Serial.print("  T  Request temperature\n\r");
  Serial.print("  H  Request relative humidity\n\r");
  Serial.print("  B  Request both temperature and relative humidity\n\r");
  Serial.print("  ?  Help (shows this menu)\n\r");
  Serial.print("  X  Exit\n\r");
  
  Serial.print("Choice?\n\r");
  
}





/*void getline(char * buf, size_t bufsize)
{
  //puTTy_menu();
  byte i;

  while(Serial.available())
  {
    puTTy_menu();
    Serial.read();
  }

  for(i = 0; i < bufsize - 1; )
  {
    if(Serial.available())
    {
      int c = Serial.read ();

      if(c == '\n')
      {
        break;
      }
      if(!isspace (c))
      {
        buf[i++] == toupper(c);
      }
    }
  }
  buf [i] = 0;
  Serial.println (buf);
  */
