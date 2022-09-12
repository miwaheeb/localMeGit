/*
  ***********************************************************************************
  *    This code was created by : Mina Waheeb
  *    Date created             : 1/26/2022
  *    Date last modified       : 2/03/2022
  *    modifications            : 
  *    
  ***********************************************************************************
  
  motorControl
  
  This program controls a motor speed and spin, the component parts are:
    -NodeMCU
    -L293D
    -2 pushbuttons w/10 KOhms each
    -external 5v power supply
    -motor

    This code and pushbuttons control the motor-propeller in the following way:
      Upon power-up or processor reset, the motor inital state is off 
      (i.e.not rotating). Pushbutton_1 cycles the motor between 
      three states: Off, Fast, and Slow (in that order). Pushbutton_2 
      changes the direction of spin. The default direction causes air to 
      flow from the propeller toward the motor. Pressing the direction 
      button whether or not the motor is spinning reverses the direction.
      Pressing both buttons simultaneously stops the motor and reset its 
      direction to the default.
*/

// defining push-buttons pins
int pushButton_1 = 16;
int pushButton_2 = 13;

// defining functions
void button_1_Pressed();
void button_2_Pressed();
void both__Pressed();
void spin();

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

    Serial.println("button 1: mode = 1 fast");
    mode_button_1++;
    
    // Turn on motor
    spin();
  }
  else if(mode_button_1 == 2)
  {
    // Set motor to half speed (slow)
    // For PWM maximum possible values are 0 to 255
    analogWrite(EN1, 125);

    Serial.println("button 1: mode = 2 slow");
    mode_button_1 = 0;
    
    // Turn on motor
    spin();
  }

  delay(450); // button bounce delay
}

// if push-button 2 is pushed
void button_2_Pressed()
{
  if(mode_button_2 == 0)
  {
    // change motor spin (default: air towards motor)
    mode_button_2++;
    spin();
  }
  else if(mode_button_2 == 1)
  {
    // change motor spin (spin: air away from motor)
    mode_button_2 = 0;
    spin();
  }
  
  delay(450); // button bounce delay
}

// if both push-buttons are pushed
void both__Pressed()
{
  // reset direction and stop motors
  mode_button_1 = 1;
  mode_button_2 = 0;
  
  // Set motor off
  // For PWM maximum possible values are 0 to 255
  analogWrite(EN1, 0);
    
  // Turn on motor
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  
  Serial.print("button 2 & 1 state: ");
  Serial.print(buttonState_1);
  Serial.println(buttonState_2);

  delay(450); // button bounce delay
}

// change motor spin
void spin()
{
  if(mode_button_2 == 0)
  {
    // change motor spin (default: air towards motor)
    Serial.println("spin: mode = 0 default spin dir 1");
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  }
  else if(mode_button_2 == 1)
  {
    // change motor spin (spin: air away from motor)
    Serial.println("spin: mode = 1 reverse spin dir 2");
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  }
}
