/************************************************************************************
 *                       time interrupt function set-up
*************************************************************************************/

/*libs*/

/*defs*/
unsigned long previousMillis = 0; // var for captering time passed
const long interval = 5000;       // var for determining time interval (5s here)

int token = 0;

/* time interrupt initialization function, runs only once from void setup() */
void updateSensors_func()
{ 
  // using built in time ISR
  unsigned long currentMillis = millis();
  
  // setting up condition to update sensor info every 5s
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // updating time passed var

    token = 1;
  }
  else
  {
    token = 0;
  }
}
/************************************************************************************/
