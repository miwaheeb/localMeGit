/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 5/12/2022
  *    Date last modified       : 7/07/2022
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
  *    modifications : 7/07/2022 
  *    -> added float valve for mid, and ful levels
  *    -> modified main and lcd.h for better structure
  *    
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
int counter = 0;      //testing lcd with ISR

/************************************************************************************
 *                      functions ".h" files declerations
*************************************************************************************/
void LCD_init();           //LCD initail startUp parameters
void LCD_valve_update();   //LCD water valves updater function

void floatValve_init();    //float valve sensor initail set-up

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
   //initialize and call time interrupt SR
   updateSensors_func(); 
   
   //LCD water valves updater function
   LCD_valve_update();

   
}
