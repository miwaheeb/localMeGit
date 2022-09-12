/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/DigitalReadSerial
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton_1 = 16;
int pushButton_2 = 13;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton_1, INPUT);
  pinMode(pushButton_2, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState_1 = digitalRead(pushButton_1);
  int buttonState_2 = digitalRead(pushButton_2);
  // print out the state of the button:
  Serial.println(buttonState_1);
  Serial.println(buttonState_2);
  delay(1);        // delay in between reads for stability
}
