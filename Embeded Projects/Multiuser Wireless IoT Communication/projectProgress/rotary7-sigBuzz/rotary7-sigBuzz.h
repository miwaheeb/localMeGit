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

//for buzzer
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E2  82
#define NOTE_E3  165
#define NOTE_AS2 117
#define REST      0

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

/*----------------------------------------------------------------------*/

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

/*######################################################################*/

const int dataPin = D8;  // blue wire to 74HC595 pin 14
const int latchPin = D7; // green to 74HC595 pin 12
const int clockPin = D6; // yellow to 74HC595 pin 11

//const char common = 'a';    // common anode
const char common = 'c';    // common cathode

/*----------------------------------------------------------------------*/

void myfnUpdateDisplay(byte eightBits) {
  if (common == 'a') {                  // using a common anonde display?
    eightBits = eightBits ^ B11111111;  // then flip all bits using XOR 
  }
  digitalWrite(latchPin, LOW);  // prepare shift register for data
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits); // send data
  digitalWrite(latchPin, HIGH); // update display
}

byte myfnNumToBits(int someNumber) {

  switch (someNumber) {
    case 0:
      return B11111100;
      break;
    case 1:
      return B01100000;
      break;
    case 2:
      return B11011010;
      break;
    case 3:
      return B11110010;
      break;
    case 4:
      return B01100110;
      break;
    case 5:
      return B10110110;
      break;
    case 6:
      return B10111110;
      break;
    case 7:
      return B11100000;
      break;
    case 8:
      return B11111110;
      break;
    case 9:
      return B11110110;
      break;
    case 10:
      return B11101110; // Hexidecimal A
      break;
    case 11:
      return B00111110; // Hexidecimal B
      break;
    case 12:
      return B10011100; // Hexidecimal C or use for Centigrade
      break;
    case 13:
      return B01111010; // Hexidecimal D
      break;
    case 14:
      return B10011110; // Hexidecimal E
      break;
    case 15:
      return B10001110; // Hexidecimal F or use for Fahrenheit
      break;  
    default:
      return B10010010; // Error condition, displays three vertical bars
      break;   
  }
}
/*######################################################################*/
// change this to make the song slower or faster
int tempo = 225;
// change this to whichever pin you want to use
int buzzer = D5;

const int melody[] PROGMEM = {
// At Doom's Gate (E1M1)
// Score available at https://musescore.com/pieridot/doom

  NOTE_E2, 8, NOTE_E2, 8, NOTE_E3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_D3, 8, NOTE_E2, 8, NOTE_E2, 8, //1
  NOTE_C3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_AS2, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_B2, 8, NOTE_C3, 8,
  NOTE_E2, 8, NOTE_E2, 8, NOTE_E3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_D3, 8, NOTE_E2, 8, NOTE_E2, 8,
  NOTE_C3, 8, NOTE_E2, 8, NOTE_E2, 8,
};
  
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

/*----------------------------------------------------------------------*/
void ringRing()
{
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = pgm_read_word_near(melody+thisNote + 1);
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, pgm_read_word_near(melody+thisNote), noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}
