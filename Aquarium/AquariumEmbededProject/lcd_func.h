/************************************************************************************
 *                           LiquidCrystal function set-up
*************************************************************************************/

/*libs*/
#include "LCD.h"                //for LCD
#include "LiquidCrystal_I2C.h"  //added library for liquid crystal

/*defs*/
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); //0x27 default I2C bus address

extern int token;     //from "timeInterrupt_func.h"

extern int float_low; //from "floatValve_func.h"
extern int float_mid; //from "floatValve_func.h"
extern int float_ful; //from "floatValve_func.h"

void getFloatValve();      //float valve main function

/* LCD initialization function, runs only once from void setup() */
void LCD_init()
{ 
 // Set off LCD module
  lcd.begin (16,2); // 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);

  lcd.home (); // Set cursor to 0,0
  lcd.print(""); // Custom text
  //lcd.print(12.265,1); // the 1 indicates only one decimal to be printed
  //lcd.print(char(223)); // adds the percent
   
  lcd.setCursor (0,1); // Go to home of 2nd line
  lcd.print("TOW: W");
  //lcd.print(65.265,1); // the 1 indicates only one decimal to be printed
  //lcd.print("%");
  
  lcd.setBacklight(HIGH); // Backlight on
}

/* LCD valve update function */
void LCD_valve_update()
{
  if(token == 1)
   {
    getFloatValve(); //update floatValve sensors
    
    if(float_low == 1)
    {
      lcd.setCursor (5,1);  // Set cursor to 10,0 [need only update the data]
      lcd.print("WH");    // the 1 indicates only one decimal to be printed
      
    }
    else if(float_low == 0)
    {
      lcd.setCursor (5,1);  // Set cursor to 10,0 [need only update the data]
      lcd.print("WL");    // the 1 indicates only one decimal to be printed
    }
   }
   else if(token == 0)
   {
    
   }
}
/************************************************************************************/
