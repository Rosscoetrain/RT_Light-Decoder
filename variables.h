/*
 * variables.h
 */


NmraDcc  Dcc ;
DCC_MSG  Packet ;


struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};


/*

So for each output pin need x  CVs for data storage

           output pin CV
           1   2   3   4   ...

data

mode       33  43  53  63  73  ... 
ontime     34  44  54  64
ontimeX    35  45  55  65
offtime    36  46  56  66
offtimeX   37  47  57  67
outputPin  38  48  58  68
outputPin2 39  49  59  69
fadein     40  50  60  70
fadeout    41  51  61  71

mode
  0 continuous (stays on until off command received)
  1 oneshot (on for ontime once) ms / 100
  2 flashing (alternate ontime offtime until off command received)
  3 flash/fade (alternate ontime offtime until off command received fade in/out
  4 flash alternate (requires two output pins)
  5 strobe double

ontime offtime
  time in ms / 10 for on or off functions

ontimeX offtimeX
  multiplier for on or off times

*/


#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
CVPair FactoryDefaultCVs [] =
{
  {CV_ACCESSORY_DECODER_ADDRESS_LSB, DEFAULT_ACCESSORY_DECODER_ADDRESS & 0xFF},
  {CV_ACCESSORY_DECODER_ADDRESS_MSB, DEFAULT_ACCESSORY_DECODER_ADDRESS >> 8},
  {CV_29_CONFIG, CV29_ACCESSORY_DECODER},

#ifdef ARDUINO_AVR_DIGISPARK
  {33, 0},
  {34, 10},
  {35, 100},
  {36, 10},
  {37, 100},
  {38, 3},
  {39, 0},
  {40, 10},
  {41, 10},

  {43, 2},
  {44, 10},
  {45, 100},
  {46, 10},
  {47, 100},
  {48, 4},
  {49, 0},
  {50, 10},
  {51, 10},
#endif
#ifndef ARDUINO_AVR_DIGISPARK
                        // default address 1
  {33, 0},
  {34, 10},
  {35, 100},
  {36, 10},
  {37, 100},
  {38, 3},
  {39, 0},
  {40, 10},
  {41, 10},
                        // default address 2
  {43, 1},
  {44, 10},
  {45, 100},
  {46, 10},
  {47, 100},
  {48, 4},
  {49, 0},
  {50, 10},
  {51, 10},
                        // default address 3
  {53, 2},
  {54, 10},
  {55, 100},
  {56, 10},
  {57, 100},
  {58, 5},
  {59, 0},
  {60, 10},
  {61, 10},
                        // default address 4
  {63, 3},
  {64, 10},
  {65, 100},
  {66, 10},
  {67, 100},
  {68, 6},
  {69, 0},
  {70, 10},
  {71, 10},
                        // default address 5
  {73, 4},
  {74, 10},
  {75, 100},
  {76, 10},
  {77, 100},
  {78, 7},
  {79, 8},
  {80, 10},
  {81, 10},
                        // default address 6
  {83, 5},
  {84, 10},
  {85, 100},
  {86, 10},
  {87, 100},
  {88, 9},
  {89, 0},
  {90, 10},
  {91, 10},
                        // default address 7
  {93, 0},
  {94, 10},
  {95, 100},
  {96, 10},
  {97, 100},
  {98, 10},
  {99, 0},
  {100, 10},
  {101, 10},
                        // default address 8
  {103, 0},
  {104, 10},
  {105, 100},
  {106, 10},
  {107, 100},
  {108, },
  {109, 0},
  {110, 10},
  {111, 10},
                        // default address 9
  {113, 0},
  {114, 10},
  {115, 100},
  {116, 10},
  {117, 100},
  {118, },
  {119, 0},
  {120, 10},
  {121, 10},
                        // default address 10
  {123, 0},
  {124, 10},
  {125, 100},
  {126, 10},
  {127, 100},
  {128, },
  {129, 0},
  {130, 10},
  {131, 10},
                        // default address 11
  {133, 0},
  {134, 10},
  {135, 100},
  {136, 10},
  {137, 100},
  {138, },
  {139, 0},
  {140, 10},
  {141, 10},
                        // default address 12
  {143, 0},
  {144, 10},
  {145, 100},
  {146, 10},
  {147, 100},
  {148, },
  {149, 0},
  {150, 10},
  {151, 10},
                        // default address 13
  {153, 0},
  {154, 10},
  {155, 100},
  {156, 10},
  {157, 100},
  {158, },
  {159, 0},
  {160, 10},
  {161, 10},
                        // default address 14
  {163, 0},
  {164, 10},
  {165, 100},
  {166, 10},
  {167, 100},
  {168, },
  {169, 0},
  {170, 10},
  {171, 10},
                        // default address 15
  {173, 0},
  {174, 10},
  {175, 100},
  {176, 10},
  {177, 100},
  {178, },
  {179, 0},
  {180, 10},
  {181, 10},
                        // default address 16
  {183, 0},
  {184, 10},
  {185, 100},
  {186, 10},
  {187, 100},
  {188, },
  {189, 0},
  {190, 10},
  {191, 10},




#endif
};
#endif

uint8_t FactoryDefaultCVIndex = 0;

uint16_t BaseDecoderAddress;

#ifdef LEARNING
// for address learning mode
int LEARNINGBUTTON = 0;    // pin 0
#define LEDCONTROL LED_BUILTIN
int learningMode = LOW;
#endif

byte thisCommand = 0;

#ifdef ENABLE_SERIAL
// buffer to hold serial commands
String readString;
#endif

#ifndef FORCE_RESET_FACTORY_DEFAULT_CV && ARDUINO_AVR_DIGISPARK
Lights lLights;
#endif

  
