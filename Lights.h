/*
 * Lights.h
 * 
 * A class to control the light decoder
 * 
 */

#include <Arduino.h>
#include "defines.h"

enum TT_State
{  
  TT_IDLE = 0,
  TT_MOVE,
  TT_MOVING,
  TT_STOP,
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
//  int               address;          // User Configurable. DCC address to respond to
  byte              mode;             // User Configurable. Mode: 1=Continuous, 2=Oneshot, 3=Flasher, 4=Flasher fader
  byte              outputPin;        // User Configurable. Arduino pin where accessory is connected to
#ifdef ACTION_FLASHALTERNATE
  byte              outputPin2;       // User Configurable. 2nd pin for AlternatingFlasher (e.g. railway crossing)
#endif
  byte              ontime;           // User Configurable. Oneshot or Flasher on time in ms X ontimeX
  byte              ontimeX;          // User Configurable. on time multiplier
  byte              offtime;          // User Configurable. Flasher off time in ms X offtimeX
  byte              offtimeX;         // User Configurable. off time multiplier
  byte              dccstate;         // Internal use. DCC state of accessory: 1=on, 0=off
  byte              onoff;            // Internal use. Output state of accessory: 1=on, 0=off
#ifndef ARDUINO_AVR_DIGISPARK
  byte              fadein;           // fade in time for fader
  byte              fadeout;          // fade out time for fader
  int               fade;             // fade level
//  byte              onoff2;           // Internal use. Output state of AlternatingFlasher 1=on, 0=off NOT USED
  byte              finished;         // Internal use. Memory that says the Oneshot is finished
#endif
  unsigned long     onMilli;          // Internal use.
  unsigned long     offMilli;         // Internal use.
} DCCAccessoryAddress;

class Lights
{
  private:
    TT_State state = TT_IDLE;
    uint8_t commandQueue[TT_MOVER_MAX_TRACKS + 1];
    byte thisCommand, newCommand;
    byte target;
    byte direction;
    
    DCCAccessoryAddress accessory[MAXACCESSORIES];


#ifndef ARDUINO_AVR_DIGISPARK
#ifdef ACTION_FLASHFADER
    void fader(byte pin, int rate, byte fade);
#endif
#else
#ifdef ACTION_FLASHFADER
//    void fader(byte pin, byte rate);
#endif
#endif

#ifdef ACTION_STROBEDOUBLE
    void strobe(byte pin);
#endif


  public:
    void init(NmraDcc Dcc);
    void addCommand(uint8_t command);
    void process(void);

};


  
