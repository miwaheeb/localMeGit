/************************************************************************************
 *                          float valve function set-up
*************************************************************************************/

/*libs*/

/*defs*/
#define float_1_low              D5 
int float_low = -1;

/* float valve initialization function, runs only once from void setup() */
void floatValve_init()
{ 
  pinMode(float_1_low,INPUT_PULLUP);
}

void getFloatValve()
{
  if(digitalRead(float_1_low) == HIGH)
  {
    float_low = 1;
  }
  else if(digitalRead(float_1_low) == LOW)
  {
    float_low = 0;
  }
}
/************************************************************************************/
