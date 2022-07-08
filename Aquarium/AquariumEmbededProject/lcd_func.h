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

extern int valveCase; //from "floatValve_func.h"

void getFloatValve(); //float valve main function

/* LCD initialization function, runs only once from void setup() */
void LCD_init()
{ 
 // Set off LCD module
  lcd.begin (16,2); // 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);

  lcd.home (); // Set cursor to 0,0
  lcd.print(""); // Custom text
   
  lcd.setCursor (0,1); // Go to home of 2nd line
  lcd.print("Water: ");
  
  lcd.setBacklight(HIGH); // Backlight on
}

/* LCD valve update function */
void LCD_valve_update()
{
  if(token == 1)
   {
    getFloatValve(); //update floatValve sensors

    if(valveCase == 0)
    {
      lcd.setCursor (7,1);  // Set cursor to 10,0 [need only update the data]
      lcd.print("Empty");      // the 1 indicates only one decimal to be printed
    }
    else if(valveCase == 1)
    {
      lcd.setCursor (7,1);
      lcd.print("Low");
    }
    else if(valveCase == 2)
    {
      lcd.setCursor (7,1);
      lcd.print("Half-way");
    }
    else if(valveCase == 3)
    {
      lcd.setCursor (7,1);
      lcd.print("Full");
    }
    else if(valveCase == 4)
    {
      lcd.setCursor (7,1);
      lcd.print("Error");      
    }
   else if(token == 0)
   {
    
   }
  }
}
/************************************************************************************/
