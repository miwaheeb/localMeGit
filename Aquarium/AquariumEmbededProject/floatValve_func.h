/************************************************************************************
 *                          float valve function set-up
*************************************************************************************/

/*libs*/

/*defs*/
#define float_1_low              D5 
#define float_2_mid              D6
#define float_3_ful              D7

int float_low = -1;
int float_mid = -1;
int float_ful = -1;

/* float valve initialization function, runs only once from void setup() */
void floatValve_init()
{ 
  pinMode(float_1_low,INPUT_PULLUP);
  pinMode(float_2_mid,INPUT_PULLUP);
  pinMode(float_3_ful,INPUT_PULLUP);
}

void getFloatValve()
{
  //---------float valve low-----------//
  if(digitalRead(float_1_low) == HIGH)
  {
    float_low = 1;
  }
  else if(digitalRead(float_1_low) == LOW)
  {
    float_low = 0;
  }
  //-----------------------------------//

  //---------float valve mid-----------//
  else if(digitalRead(float_2_mid) == HIGH)
  {
    float_mid = 1;
  }
  else if(digitalRead(float_2_mid) == LOW)
  {
    float_mid = 0;
  }
  //-----------------------------------//

  //---------float valve ful-----------//
  else if(digitalRead(float_3_ful) == HIGH)
  {
    float_ful = 1;
  }
  else if(digitalRead(float_3_ful) == LOW)
  {
    float_ful = 0;
  }
  //-----------------------------------//
}
/************************************************************************************/
