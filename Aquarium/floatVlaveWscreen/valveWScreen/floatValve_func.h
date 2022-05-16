/************************************************************************************
 *                          float valve function set-up
*************************************************************************************/

/*libs*/

/*defs*/
#define float_1_low              D5 

/* float valve initialization function, runs only once from void setup() */
void floatValve_init()
{ 
  pinMode(float_1_low,INPUT_PULLUP);
}

void getFloatValve()
{
  
}
/************************************************************************************/
