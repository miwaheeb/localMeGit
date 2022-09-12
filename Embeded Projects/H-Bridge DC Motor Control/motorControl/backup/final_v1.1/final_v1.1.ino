// defining push-buttons pins
int pushButton_1 = 16;
int pushButton_2 = 13;

// defining functions
void button_1_Pressed();
void button_2_Pressed();
void both__Pressed();

// defining button state vars
int buttonState_1 = digitalRead(pushButton_1);
int buttonState_2 = digitalRead(pushButton_2);

void setup() 
{
  Serial.begin(9600); // setting up serial for testing
  
  // setting push-buttons pins as inputs
  pinMode(pushButton_1, INPUT);
  pinMode(pushButton_2, INPUT);
}

void loop() 
{
  // read push-buttons input state
  buttonState_1 = digitalRead(pushButton_1);
  buttonState_2 = digitalRead(pushButton_2);

  if (buttonState_1 && buttonState_2 == 1)
  {
    both__Pressed();
  }
  else if (buttonState_2 == 1)
  {
    button_2_Pressed();
  }
  else if (buttonState_1 == 1)
  {
    button_1_Pressed();
  }
}


void button_1_Pressed()
{
  Serial.print("button 1: ");
  Serial.println(buttonState_1);
  
  delay(500);
}

void button_2_Pressed()
{
  Serial.print("button 2: ");
  Serial.println(buttonState_2);

  delay(500);
}

void both__Pressed()
{
  Serial.print("button 2 & 1 state: ");
  Serial.print(buttonState_1);
  Serial.println(buttonState_2);

  delay(500);
}
