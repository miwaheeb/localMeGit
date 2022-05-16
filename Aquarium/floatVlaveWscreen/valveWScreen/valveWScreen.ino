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
extern int token;
int counter = 0; //testing lcd with ISR

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
    getFloatValve();
    lcd.setCursor (10,0);  // Set cursor to 10,0 [need only update the data]
    lcd.print(counter++,1);    // the 1 indicates only one decimal to be printed
    
   }
   else if(token == 0)
   {
    
   }
   
   lcd.home (); // Set cursor to 0,0
   lcd.print("Veros Love Meter:"); // Custom text
   if(digitalRead(float_1_low)== HIGH)
   {
    
    lcd.setCursor (0,1); // Go to home of 2nd line
    lcd.print("HIGHERRRRRRRRRRRRRRRRR");
    while(digitalRead(float_1_lowrt)== HIGH) {}
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
