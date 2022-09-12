/*****************************************************
 * Date: 1/26/2022
 * Written by: Mina Waheeb
 * ***************************************************/

// PushButtons state declerations
boolean motor_dir = 0; // pushButton_2 controls motor direction

// Defining PINs
#define pushButton_2 0 // Pushbutton_2 NodeMCU D3 GPIO0

int EN1 = 14; // Pin 1 of L293D IC, GPIO14-D5 Pin of NodeMCU

int A1 = 5; // NodeMCU D1 GPIO5 = L293D Control_IN1(A1) PIN2 
int A2 = 4; // NodeMCU D2 GPIO4 = L293D Control_IN2(A2) PIN7

void setup()
{
  // Set up pushButtons
  pinMode(pushButton_2, INPUT_PULLUP); // Pull-up resistor
  
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
  directionControl();
  delay(1000);
  speedControl();
  delay(1000);
}

// This function controls spinning direction of motor
void directionControl() {
  // Set motor to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(EN1, 255);

  // Turn on motor
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  delay(2000);
  
  // Now change motor directions
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  delay(2000);
  
  // Turn off motors
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

// This function controls speed of motor
void speedControl() {
  // Turn on motors
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(EN1, i);
    delay(20);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(EN1, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}
