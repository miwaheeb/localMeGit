#include <OneWire.h>
#include <DallasTemperature.h>

int sensePin = A0;     //This is the Arduino Pin that will read the sensor output
int sensorInput;       //The variable we will use to store the sensor input
double temp;           //The variable we will use to store temperature in degrees. 
const int relay = A1;  //Relay switch pin
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Start the Serial Port at 9600 baud (default)
 
}
void loop() {
  // initialize digital pin A1 as an output.
  pinMode(relay, OUTPUT);
  
  // put your main code here, to run repeatedly: 
  sensorInput = analogRead(A0);    //read the analog sensor and store it
  temp = (double)sensorInput / 933;       //find percentage of input reading
  temp = temp * 5;                 //multiply by 5V to get voltage
  temp = temp - 0.5;               //Subtract the offset 
  temp = temp * 100;               //Convert to degrees 
  temp= (temp * 1.8) + 32;         //Convert to F

  //Control statement for fanRelay Switch
  if (((temp > 77) && (temp <= 78)) || (temp > 79 && temp <= 82))
  {
    delay(10000); //delay 10 seconds
    if ( temp <= 74 || temp > 80)
    {
      return;
    }
    digitalWrite(relay, HIGH);
    delay(300000); //delay for 5min - 1000ms = 1s
  }
  else 
  {
    digitalWrite(relay, LOW);
  }
  
 
  Serial.print("Current Temperature: ");
  Serial.println(temp);
}
