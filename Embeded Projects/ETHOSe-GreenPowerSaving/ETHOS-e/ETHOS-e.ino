/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 3/25/2022
  *    Date last modified       : 4/07/2022
  *    modifications            : 
  *    
  ***********************************************************************************
  
  The Electronic Temperature & Humidity Observation System eco Edition (ETHOS-e)
  
  ETHOS-e gathers temperature and relative humidity information using:
    -NodeMCU
    -Si7021
    -PuTTy software for remote users

    ETHOS-e, the Electronic Temperature & Humidity Observation System.
    Gathers temperature and relative humidity information from
    an Si7021 I2C-based temperature-humidity sensor.
    ETHOS-s gathers updated sensor information during the processor
    awake period of 5 seconds. Afterwhich, ETHOS-e puts the
    ESP8266 microcontroller to deep sleep for 10 seconds.
    ETHOS-e provides temperature and humidity information to 
    remote users interactively via RS232 (PuTTy)
*/
#include "Wire.h" // lib for I2C
#include "SparkFun_Si7021_Breakout_Library.h"  // lib for Si7021 Temp and Humid
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "ETHOS-e.h"         // this project's .h file

//functions decleration
WiFiClient wfClient;
void connect_wifi();
void printEthos();


//pre-dec for Si7021 Temp and Humid
float humidity = 0; // humid float var
float tempf = 0;    // temp float var
Weather sensor;     // Instance of temp and humidity sensor

//time interval setup
const long interval = 5000; // var for determining time interval (5s here)

// setup
void setup()
{
  // **********open serial over USB at 9600 baud**********
  Serial.begin(74800);
  sensor.begin(); // Initialize the I2C sensors and ping them
  // *****************************************************
  
  connect_wifi(); // setup Wifi connection
  printEthos();   // get temprature and print to user interface
}

// loop
void loop()
{}
/*******************************************************************
*                            functions
*******************************************************************/
//connect to lipscombGuest wifi network
void connect_wifi()
{
  delay(10);
  #ifdef LIPSCOMB
    WiFi.begin(ssid);            // Lipscomb WiFi does NOT require a password
  #else
    WiFi.begin(ssid, password);  // For WiFi networks that DO require a password
  #endif

  // advance a "dot-dot-dot" indicator until connected to WiFi network
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(300);
  }
}

void printEthos()
{
  //print status along with temp&humid info
  Serial.print("\n\rI'm awake!");
  Serial.print("\n\rT: ");
  Serial.print(sensor.getTempF(),1);
  Serial.print("F\n\rH: ");
  Serial.print(sensor.getRH(),0);
  Serial.print("%\n\rI'm getting sleepy");
  
  unsigned long timeT = 0;
  //5 second delay pro is awake during
  while(timeT < interval)
  {
    timeT+=100;
    Serial.print(".");
    delay(100);
  }
  Serial.print("\n\rGood night!\n\r"); //pro going to sleep message
  
  ESP.deepSleep(10e6); /* Sleep proc for 10 seconds */
  
}
