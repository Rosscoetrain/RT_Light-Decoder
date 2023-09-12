/*
   defines.h
*/


// Un-comment the following to allow address learning
//#define LEARNING

// Un-comment the following to allow serial communications
#define ENABLE_SERIAL

// to show all debug messages
//#define DEBUG_MSG

// to show lights debug messages
//#define DEBUG_MSG_LIGHTS

// Uncomment to print all DCC Packets
//#define  NOTIFY_DCC_MSG

// Un-comment the following line to activate testing routines.
//#define TESTING

// Un-comment the following line to activate dcc testing routine.
//#define TESTING_DCC

// Un-Comment the line below to force CVs to be written to the Factory Default values
// defined in the FactoryDefaultCVs below on Start-Up
//<#define FORCE_RESET_FACTORY_DEFAULT_CV

// Un-Comment the line below to Enable DCC ACK for Service Mode Programming Read CV Capablilty
//#define ENABLE_DCC_ACK  15  // This is A1 on the Iowa Scaled Engineering ARD-DCCSHIELD DCC Shield


// Define the Arduino input Pin number for the DCC Signal
#define DCC_PIN     2

// Set the Decoder Version - Used by JMRI to Identify the decoder
#define DCC_DECODER_VERSION_NUM 15


// digispark attiny85 usb board.  Actions are limited by the available flash memory.

// Following is able to be loaded together.  All actions (except flash facer) can be loaded singly.
//  ACTION_ONOFF + ACTION_ONESHOT
//  ACTION_ONOFF + ACTION_FLASH
//  ACTION_ONOFF + ACTION_STROBEDOUBLE

#if defined(ARDUINO_AVR_DIGISPARK) || defined(ARDUINO_AVR_DIGISPARKPRO)

//#define ACTION_ONOFF                        // on off by command
//#define ACTION_ONESHOT                      // one shot flash
//#define ACTION_FLASH                        // flash on single pin
//#define ACTION_FLASHFADER                     // flash with fade up/down
//#define ACTION_FLASHALTERNATE               // flash on alternate pins needs two output pins can only have TT_MOVER_MAX_TRACKS = 1
#define ACTION_STROBEDOUBLE                   // double strobe flash on single pin


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fill in the number of accessories you want to control  on attiny85 this is 1 - 4
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(ACTION_ONOFF) && (defined(ACTION_ONESHOT) || defined(ACTION_FLASH) || defined(ACTION_STROBEDOUBLE))
#define MAXACCESSORIES 2
#else
#define MAXACCESSORIES 1
#endif

#ifdef ARDUINO_AVR_DIGISPARKPRO
#define MAXACCESSORIES 13
#endif

// this determines the number of queued commands.
#define TT_MOVER_MAX_TRACKS 1

#define LED_BUILTIN 1

#else

#define ACTION_ONOFF                        // on off by command
#define ACTION_ONESHOT                      // one shot flash
#define ACTION_FLASH                        // flash on single pin
#define ACTION_FLASHFADER                   // flash with fade up/down
#define ACTION_FLASHALTERNATE               // flash on alternate pins needs two output pins
#define ACTION_STROBEDOUBLE                 // double strobe flash on single pin

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fill in the number of accessories you want to control depends on board in use
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAXACCESSORIES 16


// this determines the number of queued commands.
#define TT_MOVER_MAX_TRACKS 16

#define LED_BUILTIN 13

#endif


#define TT_MOVER_SLOT_EMPTY 255

#define STROBE_DELAY 100

  
  
