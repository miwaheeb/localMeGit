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

/**************************************************************************************************
 *                                        PuTTy setup
**************************************************************************************************/
char response;          // var for storing line char
char responseUpperCase; // var for storing upper case converted chars

char serialArray[100];  // array to detect \r or Enter key press
int arrayInc = 0;       // array incrementer

bool exitBit = 0;       // bool exit condition, if == 1 remote user selected 'X' Exit
void exit_puTTy();      // exit putty func is what changes the exitBit from 0 to 1
/**************************************************************************************************/

/***************************************************************************************************
 *                                    time interrupt setup
****************************************************************************************************/
unsigned long previousMillis = 0; // var for captering time passed
const long interval = 5000;       // var for determining time interval (5s here)
/***************************************************************************************************/

// setup
void setup()
{
  // **********open serial over USB at 9600 baud**********
  Serial.begin(9600);
  sensor.begin(); // Initialize the I2C sensors and ping them
  // *****************************************************
  
  puTTy_menu();   // Initialize PuTTY
}

// loop
void loop()
{
  // using built in time ISR
  unsigned long currentMillis = millis();

  // setting up condition to update sensor info every 5s
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // updating time passed var
    
    getWeather();          // Get readings from all sensors
  }
  // if exitBit not triggered (aka remote did not press 'X' to Exit)
  // communicate with PuTTy, else exit PuTTy communication
  else if(exitBit != 1)
  {
    puTTy();
  }
  else{}
}
/*******************************************************************
********************************************************************
*                            $$functions$$
********************************************************************
*******************************************************************/
// Si7021 Temp and Humid weather function
void getWeather()
{
  humidity = sensor.getRH();    // Measure Relative Humidity
  tempf    = sensor.getTempF(); // Measure Temperature in F
}

// PuTTy menu function
void puTTy(void)
{
  // if serial communication avil
  if(Serial.available() > 0)
  {
    response = char(Serial.read());   // store char in var
    Serial.print(response);           // print the char typed
    response = toUpperCase(response); // store char as upperCase
    serialArray[arrayInc] = response; // store char in array
    arrayInc++;                       // inc array

    // if response equal \r -> ENTER key pressd
    if(response == '\r')
    {
      // dec array pointer twice
      arrayInc--;
      arrayInc--;

      // if and if else conditional statements below control 
      // input response from and to remote user via PuTTy
      if(serialArray[arrayInc] == 'T' && arrayInc < 1)
      {
        Serial.print("\n\r");
        Serial.print("\n\rT: ");
        Serial.print(tempf,1);
        Serial.print("F");
        
        Serial.print("\n\n\rChoice? ");
        arrayInc = 0;
      }
      else if(serialArray[arrayInc] == 'H' && arrayInc < 1)
      {
        Serial.print("\n\r");
        Serial.print("\n\rH: ");
        Serial.print(humidity,0);
        Serial.print("%");

        Serial.print("\n\n\rChoice? ");
        arrayInc = 0;
      }
      else if(serialArray[arrayInc] == '?' && arrayInc < 1)
      {
        Serial.print("\n\n\r");
        puTTy_menu();
      }
      else if(serialArray[arrayInc] == 'X' && arrayInc < 1)
      {
        Serial.print("\n\r");
        Serial.print("\n\rGoodbye!");
        exit_puTTy();
      }
      else
      {
        Serial.print("\n\n\r");
        puTTy_menu();
      }
    }
  }
}

// PuTTy menu function responsible for printing ETHOS menu
void puTTy_menu()
{
  Serial.print("I'm awake!\n\r");
  Serial.print("\n\r");
  Serial.print("Your options:\n\r");
  Serial.print("  T  Request temperature\n\r");
  Serial.print("  H  Request relative humidity\n\r");
  Serial.print("  X  Exit\n\r");
  
  Serial.print("Choice? ");
  arrayInc = 0;
}

// PuTTy exit func responsible for flipping exit bit on Exit
void exit_puTTy()
{
  exitBit = 1;
}
