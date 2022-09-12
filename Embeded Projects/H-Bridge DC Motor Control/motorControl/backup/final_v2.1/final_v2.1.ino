/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 1/26/2022
  *    Date last modified       : 2/03/2022
  *    modifications            : 
  *    
  ***********************************************************************************
  
  (insert code name here)
  
  (instert code description here)

*/

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

// defining push-button mode vars
int mode_button_1 = 1;
int mode_button_2 = 0;

/*************************************************************
 *            L293D and NodeMCU pin definitions              *
**************************************************************/
int EN1 = 14; // Pin 1 of L293D IC, GPIO14-D5 Pin of NodeMCU

int A1 = 5; // NodeMCU D1 GPIO5 = L293D Control_IN1(A1) PIN2 
int A2 = 4; // NodeMCU D2 GPIO4 = L293D Control_IN2(A2) PIN7

void setup() 
{
  Serial.begin(9600); // setting up serial for testing
  
  // setting push-buttons pins as inputs
  pinMode(pushButton_1, INPUT);
  pinMode(pushButton_2, INPUT);

  // Set all motor control pins to outputs
  pinMode(EN1, OUTPUT); // NodeMCU GPIO14-D5 = L293D PIN1 Enable (control_motor_speed)
  pinMode(A1, OUTPUT);  // L293D motor Control_IN1(A1) PIN2  
  pinMode(A2, OUTPUT);  // L293D motor Control_IN2(A2) PIN7

  // Turn off motor - Initial state
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

void loop() 
{
  // read push-buttons input state
  buttonState_1 = digitalRead(pushButton_1);
  buttonState_2 = digitalRead(pushButton_2);

  // push-buttons conditional statements
  if(buttonState_1 && buttonState_2 == 1)
  {
    mode_button_1 = 0;
    mode_button_2 = 0;
    both__Pressed();
  }
  else if(buttonState_2 == 1)
  {
    button_2_Pressed();
  }
  else if(buttonState_1 == 1)
  {
    button_1_Pressed();
  }

  delay(125); // button bounce delay
}

/***************************************************
 ***************************************************
 *         push-buttons functions below            *
 ***************************************************
***************************************************/

// if push-button 1 is pushed
void button_1_Pressed()
{
  if(mode_button_1 == 0)
  {
    // Set motor off
    // For PWM maximum possible values are 0 to 255
    analogWrite(EN1, 0);
    
    // Turn on motor
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    
    Serial.println("button 1: mode = 0 off");
    mode_button_1++;
  }
  else if(mode_button_1 == 1)
  {
    // Set motor to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(EN1, 255);
    
    // Turn on motor
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  
    Serial.println("button 1: mode = 1 fast");
    mode_button_1++;
  }
  else if(mode_button_1 == 2)
  {
    // Set motor to half speed (slow)
    // For PWM maximum possible values are 0 to 255
    analogWrite(EN1, 125);
    
    // Turn on motor
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    
    Serial.println("button 1: mode = 2 slow");
    mode_button_1 = 0;
  }

  delay(500); // button bounce delay
}

// if push-button 2 is pushed
void button_2_Pressed()
{
  if(mode_button_2 == 0)
  {
    Serial.println("button 2: mode = 0 spin dir 1");
    mode_button_2++;
    
  }
  else if(mode_button_2 == 1)
  {
    Serial.println("button 2: mode = 1 spin dir 2");
    mode_button_2 = 0;
  }
  
  delay(500); // button bounce delay
}

// if both push-buttons are pushed
void both__Pressed()
{
  Serial.print("button 2 & 1 state: ");
  Serial.print(buttonState_1);
  Serial.println(buttonState_2);

  delay(500); // button bounce delay
}
