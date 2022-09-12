/////////////////////////////////////////////////////////////////
//libs
#include <Button2.h>;
#include <Rotary.h>;
#include "rotary7-sigBuzz.h"
////////////////////////////////////////////////////////////////
//functions
void updatePBStatus();

void myfnUpdateDisplay(byte eightBits);
byte myfnNumToBits(int someNumber);
void ringRing();

/////////////////////////////////////////////////////////////////
//globals
Rotary r;
Button2 b;

/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSimple Counter");

  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);
  r.setUpperBound(16);
  r.setLowerBound(-1);

  b.begin(BUTTON_PIN, INPUT_PULLUP);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);

  // attach a debouncer to rotary pushbutton and set debounce interval
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(DEBOUNCE_INTERVAL);

  // Create a timer to periodically call the updatePBStatus routine
  pbTimer.attach_ms(PB_UPDATE_TIME, updatePBStatus);

  // initialize counters
  pressedCount = 0;
  releasedCount = 0;

  // initialize I/O pins for 7-sig
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  r.loop();
  b.loop();
}

/////////////////////////////////////////////////////////////////

// on change
void rotate(Rotary& r) {
  
   Serial.println(r.getPosition());
   
   if(r.getPosition() == r.getUpperBound())
   {
    r.resetPosition(0, true);
   }
   else if(r.getPosition() == r.getLowerBound())
   {
    r.resetPosition(15,true);
   }
   
   byte bits = myfnNumToBits(r.getPosition());
   myfnUpdateDisplay(bits);
}

// on left or right rotation
void showDirection(Rotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}

// single click
void click(Button2& btn) {
  Serial.println("Click!\n");
  if(r.getPosition() == 14)
  {
    ringRing();
  }
  Serial.println(r.getPosition());
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}

/////////////////////////////////////////////////////////////////
