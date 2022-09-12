/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 2/04/2022
  *    Date last modified       : 2/17/2022
  *    modifications            : 
  *    
  ***********************************************************************************
  
  The Electronic Temperature & Humidity Observation System (ETHOS)
  
  ETHOS gathers temperature and relative humidity information using:
    -NodeMCU
    -Si7021
    -LCD backpack
    -external 5v power supply
    -PuTTy software for remote users

    ETHOS, the Electronic Temperature & Humidity Observation System.
    Gathers temperature and relative humidity information from
    an Si7021 I2C-based temperature-humidity sensor and continuously 
    displays the results on an I2C-based 1602A 16x2LCD display.
    ETHOS gathers updated sensor information every 5s interval.
    ETHOS is also capable of providing temperature and humidity 
    information to remote users interactively via RS232 (PuTTy)
*/
#include "Wire.h" // lib for I2C
#include "SparkFun_Si7021_Breakout_Library.h"  // lib for Si7021 Temp and Humid

/*************************************************************************************************
 *                              pre-dec for Si7021 Temp and Humid
**************************************************************************************************/
float humidity = 0; // humid float var
float tempf = 0;    // temp float var

Weather sensor;     // Instance of temp and humidity sensor
/*************************************************************************************************/

/***************************************************************************************************
 *                                    time interval setup
****************************************************************************************************/
const long interval = 5000;       // var for determining time interval (5s here)
/***************************************************************************************************/

// setup
void setup()
{
  // **********open serial over USB at 9600 baud**********
  Serial.begin(74800);
  sensor.begin(); // Initialize the I2C sensors and ping them
  // *****************************************************
}

// loop
void loop()
{
  Serial.print("\n\rI'm awake!");
  Serial.print("\n\rT: ");
  Serial.print(sensor.getTempF(),1);
  Serial.print("F\n\rH: ");
  Serial.print(sensor.getRH(),0);
  Serial.print("%\n\rI'm getting sleepy");
  
  // using built in time ISR
  unsigned long timeT = 0;
  int n = 0;
  while(timeT <= interval)
  {
    timeT+=100;
    Serial.print(".");
    delay(100);
  }
  Serial.print("\n\rGood night!\n\r");
  
  //delay(1000);
  ESP.deepSleep(10e6); /* Sleep for 10 seconds */
  
}
/*******************************************************************
********************************************************************
*                            $$functions$$
********************************************************************
*******************************************************************/
