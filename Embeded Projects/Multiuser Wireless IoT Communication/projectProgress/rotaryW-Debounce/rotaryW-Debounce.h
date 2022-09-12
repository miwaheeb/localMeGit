////////////////////////////////////////////////////////////////////////////////////
//system defines

//for debounce
#define DEBOUNCE_INTERVAL      5  // 5mS works well for circuit-mount PBs
#define PB_UPDATE_TIME         8  // number of mS between button status checks

//for rotary
#define ROTARY_PIN1 D2 //right
#define ROTARY_PIN2 D1 //left
#define BUTTON_PIN  D3 //click

#define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 

////////////////////////////////////////////////////////////////////////////////////
//system functions

#include <Bounce2.h>    // to debounce pushbuttons
#include <Ticker.h>     // to make Bounce2 work

// create a Ticker object to periodically check the
// the state of a debounced pushbutton
Ticker pbTimer;

// create a Bounce object to debounce a pushbutton switch
Bounce debouncer = Bounce();

// global millisecond tick counter
unsigned long currentMillis = millis();

// event counters
int pressedCount, releasedCount;

void updatePBStatus() {
  // this function is called every PB_UPDATE_TIME milliseconds
  // (currently 8) to update the state of the pushbutton
  // debounder (adjust the time according to PB characteristics)
  debouncer.update();

  if(debouncer.fell()) {
    printf("Pressed  [%d]\r\n", ++pressedCount);
  }
  else if(debouncer.rose()) {
    printf("Released [%d]\r\n\r\n", ++releasedCount);
  }
}
