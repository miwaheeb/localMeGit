/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 5/12/2022
  *    Date last modified       : 2/16/2022
  *    
  *    modifications : 2/16/2022          
  *    -> added lcd_func.h file
  *    -> added LCD.init function to set-up LCD
  *    -> added serial.begin for serial print
  *    
  *    -> added floatValve_func.h file
  *    -> implemented floatValve init
  *    -> implemented floatValve func
  *    
  *    -> added timeInterrupt_func.h file
  *    -> implemented updateSensors_func
  *    
  *    
  *    
  ***********************************************************************************
  
*/

/*libs*/
#include "Wire.h"               //for I2C
#include "lcd_func.h"           //lcd_func.h file
#include "floatValve_func.h"    //floatValve_func.h file
#include "timeInterrupt_func.h" //timeInterrupt_func.h file

/*defs*/
extern int token;     //from "timeInterrupt_func.h"
int counter = 0;      //testing lcd with ISR

extern int float_low; //from "floatValve_func.h"

/************************************************************************************
 *                      functions ".h" files declerations
*************************************************************************************/
void LCD_init();           //LCD initail startUp parameters
void floatValve_init();    //float valve sensor initail set-up
void getFloatValve();    //float valve main function
void updateSensors_func(); //time interrupt setup
/************************************************************************************/

void setup()
{
  //**************open serial over USB at 9600 baud*************
  Serial.begin(9600);
  //************************************************************
  
  LCD_init();           //initialize LCD 
  floatValve_init();    //initialize float valves 
}
void loop()
{
  
   updateSensors_func(); //initialize and call time interrupt SR

   if(token == 1)
   {
    getFloatValve(); //update floatValve sensors
    
    if(float_low == 1)
    {
      lcd.setCursor (10,0);  // Set cursor to 10,0 [need only update the data]
      lcd.print("WH");    // the 1 indicates only one decimal to be printed
      
    }
    else if(float_low == 0)
    {
      lcd.setCursor (10,0);  // Set cursor to 10,0 [need only update the data]
      lcd.print("WL");    // the 1 indicates only one decimal to be printed
    }
   }
   else if(token == 0)
   {
    
   }
}
