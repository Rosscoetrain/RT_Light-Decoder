/*
 * Lights.cpp
 * 
 * A class to control light decoder
 * 
 */

#include <NmraDcc.h>

#include "Lights.h"
#include "defines.h"

void Lights::init(NmraDcc Dcc)
{
  this->state = TT_IDLE;
  memset(this->commandQueue, TT_MOVER_SLOT_EMPTY, TT_MOVER_MAX_TRACKS + 1);

#ifndef ARDUINO_AVR_DIGISPARK
  for (byte i=0; i < MAXACCESSORIES; i++)
   {
#else
  byte i = 0;
#endif
     accessory[i].mode = Dcc.getCV(33 + i * 10);          // mode
     accessory[i].ontime = Dcc.getCV(34 + i * 10);        // ontime
     accessory[i].ontimeX = Dcc.getCV(35 + i * 10);       // ontime multiplier
     accessory[i].offtime = Dcc.getCV(36 + i * 10);       // offtime
     accessory[i].offtimeX = Dcc.getCV(37 + i * 10);      // offtime multiplier
     accessory[i].outputPin = Dcc.getCV(38 + i * 10);     // output pin 1
     accessory[i].dccstate = 0;                           // Internal use. DCC state of accessory: 1=on, 0=off
     accessory[i].onoff = 0;                              // Internal use. Output state of accessory: 1=on, 0=off
     accessory[i].onMilli = 0;                            // Internal use.
     accessory[i].offMilli = 0;                           // Internal use.

#ifdef ACTION_FLASHALTERNATE
     accessory[i].outputPin2 = Dcc.getCV(39 + i * 10);
#endif

#ifndef ARDUINO_AVR_DIGISPARK
     accessory[i].fadein = Dcc.getCV(40 + i * 10);             // fade in time for fader
     accessory[i].fadeout = Dcc.getCV(41 + i * 10);            // fade out time for fader
     accessory[i].fade = 0;                              // Internal use. Output state of accessory: 1=on, 0=off
#endif

     pinMode(accessory[i].outputPin, OUTPUT);
     digitalWrite(accessory[i].outputPin, LOW);
#ifndef ARDUINO_AVR_DIGISPARK
   }
#endif
}


void Lights::addCommand(uint8_t command)
{
  for(uint8_t i = 0; i < TT_MOVER_MAX_TRACKS; i++)
  {
    if(this->commandQueue[i] == command)
    {
      return;
    }

    else if(commandQueue[i] == TT_MOVER_SLOT_EMPTY)
    {
      this->commandQueue[i] = command;
      this->process();
      return;
    }
  }
  return;
}



void Lights::process(void)
{
    if ( this->commandQueue[0] != TT_MOVER_SLOT_EMPTY )
     {
      this->thisCommand = this->commandQueue[0];
      this->state = TT_MOVE;

#ifdef DEBUG_MSG_LIGHTS
      Serial.print("commandQueue 0 = ");Serial.println(this->commandQueue[0], DEC);
      Serial.print("state = ");Serial.println(this->state);
#endif

     }

/*
 * this starts and stops the applicable mode depending on the state of accessory[target].dccstate
 * for each mode there will be a #ifdef ACTION_ #endif section
 * 
 */



    if ( this->state == TT_MOVE )
     {

      this->target = byte( this->thisCommand / 10);
      this->direction = ( ( this->thisCommand ) - ( this->target * 10 ) - 1);

#ifdef DEBUG_MSG_LIGHTS
      Serial.print("thisCommand = ");Serial.println(this->thisCommand);
      Serial.print("target = ");Serial.println(this->target);
      Serial.print("direction = ");Serial.println(this->direction);
#endif

      if ( !accessory[this->target].dccstate && this->direction)         // if turned off and direction turn on
       {
        switch (accessory[this->target].mode)
         {
#ifdef ACTION_ONOFF
          case 0:                                                        // on/off
            digitalWrite(accessory[this->target].outputPin, HIGH);
            accessory[this->target].dccstate = 1;
            accessory[this->target].onoff = 1;
            this->state = TT_STOP;
          break;
#endif
#ifdef ACTION_ONESHOT
          case 1:                                                        // oneshot
            accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
            digitalWrite(accessory[this->target].outputPin, HIGH);
            accessory[this->target].dccstate = 1;
            accessory[this->target].onoff = 1;
            this->state = TT_STOP;
          break;
#endif
#ifdef ACTION_FLASH
          case 2:                                                        // flashing
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
          break;
#endif



#ifdef ACTION_FLASHFADER
          case 3:                                                        // flashing fade turn on
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);

              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;

              accessory[this->target].fade = 0;
              analogWrite(accessory[this->target].outputPin, accessory[this->target].fade);

              this->state = TT_STOP;
             }
          break;
#endif



#ifdef ACTION_FLASHALTERNATE
          case 4:
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              digitalWrite(accessory[this->target].outputPin, HIGH);
              digitalWrite(accessory[this->target].outputPin2, LOW);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
//              accessory[this->target].onoff2 = 0;
              this->state = TT_STOP;
             }
          break;
#endif

#ifdef ACTION_STROBEDOUBLE
          case 5:                                                        // strobe double
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].offMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              this->strobe(accessory[this->target].outputPin);
              delay(100);
              this->strobe(accessory[this->target].outputPin);

              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
          break;
#endif

#ifdef ACTION_RANDOMBRIGHT
          case 6:                                                        // random bright turn on  changes random brightness after ontime * ontimeX
            if (!accessory[this->target].dccstate)
             {
              accessory[this->target].onMilli = millis() + (accessory[this->target].ontime * accessory[this->target].ontimeX);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;

              accessory[this->target].fade = random(255);
              analogWrite(accessory[this->target].outputPin, accessory[this->target].fade);

              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_7
          case 7:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_8
          case 8:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_9
          case 9:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_10
          case 10:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_11
          case 11:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_12
          case 12:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_13
          case 13:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_14
          case 14:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_15
          case 15:
            if (!accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, HIGH);
              accessory[this->target].dccstate = 1;
              accessory[this->target].onoff = 1;
              this->state = TT_STOP;
             }
#endif

         }
       }
      else
       {
        if ( accessory[this->target].dccstate && !this->direction )         // if turned on and direction turn off
         {
          switch (accessory[this->target].mode)
           {
#ifdef ACTION_ONOFF
            case 0:
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif
#ifdef ACTION_ONESHOT
            case 1:
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif
#ifdef ACTION_FLASH
            case 2:
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif


#ifdef ACTION_FLASHFADER
            case 3:
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;

              accessory[this->target].fade = 0;
              analogWrite(accessory[this->target].outputPin, accessory[this->target].fade);

              this->state = TT_STOP;
            break;
#endif


#ifdef ACTION_FLASHALTERNATE
            case 4:
              digitalWrite(accessory[this->target].outputPin, LOW);
              digitalWrite(accessory[this->target].outputPin2, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
//              accessory[this->target].onoff2 = 0;
              this->state = TT_STOP;
            break;
#endif

#ifdef ACTION_STROBEDOUBLE
            case 5:
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
            break;
#endif

#ifdef ACTION_RANDOMBRIGHT
            case 6:
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;

              accessory[this->target].fade = 0;
              analogWrite(accessory[this->target].outputPin, accessory[this->target].fade);

              this->state = TT_STOP;
            break;
#endif


#ifdef ACTION_7
          case 7:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_8
          case 8:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_9
          case 9:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_10
          case 10:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_11
          case 11:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_12
          case 12:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_13
          case 13:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_14
          case 14:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif

#ifdef ACTION_15
          case 15:
            if (accessory[this->target].dccstate)
             {
              digitalWrite(accessory[this->target].outputPin, LOW);
              accessory[this->target].dccstate = 0;
              accessory[this->target].onoff = 0;
              this->state = TT_STOP;
             }
#endif



           }
         }
        else
         {
          this->state = TT_STOP;
         }

       }
     }

/*
 * this alters the output depending on mode, time and dccstate
 */

    for ( int i = 0; i < MAXACCESSORIES; i++)
     {
      if (accessory[i].dccstate)
       {

#ifdef DEBUG_MSG_LIGHTS
        Serial.print("onMilli = ");Serial.println(accessory[i].onMilli);
        Serial.print("offMilli = ");Serial.println(accessory[i].offMilli);
        Serial.print("millis = ");Serial.println(millis());
#endif

        switch (accessory[i].mode)
         {
#ifdef ACTION_ONOFF
          case 0:                                         // oneshot
          break;
#endif
#ifdef ACTION_ONESHOT
          case 1:                                         // oneshot
            if (millis() > accessory[i].offMilli)
             {
              digitalWrite(accessory[i].outputPin, LOW);
              accessory[i].dccstate = 0;
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
          break;
#endif

#ifdef ACTION_FLASH
          case 2:                                         // flashing
            if (accessory[i].onoff && millis() > accessory[i].offMilli)
             {
              digitalWrite(accessory[i].outputPin, LOW);
              accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
            else
             {
              if (!accessory[i].onoff && millis() > accessory[i].onMilli)
               {
                digitalWrite(accessory[i].outputPin, HIGH);
                accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                accessory[i].onoff = 1;
                this->state = TT_STOP;
               }
             }
          break;
#endif

#ifdef ACTION_FLASHFADER
          case 3:                                        // flashing fade
            if ( (accessory[i].onoff) && (millis() > accessory[i].offMilli) )
             {

              if ( accessory[i].fade <= 255 )
               {
                if ( millis() > accessory[i].offMilli + accessory[i].fadein )
                 {
                  analogWrite(accessory[i].outputPin, accessory[i].fade);
                  accessory[i].offMilli += accessory[i].fadein;

                  accessory[i].fade += 5;

                 }
               }
              else
               {
                accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
                accessory[i].onoff = 0;
                accessory[i].fade = 255;

                this->state = TT_STOP;
               }

             }
            else
             {
              if ( (!accessory[i].onoff) && (millis() > accessory[i].onMilli))
               {

                if ( accessory[i].fade >= 0 )
                 {
                  if ( millis() > accessory[i].onMilli + accessory[i].fadeout )
                   {
                    analogWrite(accessory[i].outputPin, accessory[i].fade);
                    accessory[i].onMilli += accessory[i].fadeout;

                    accessory[i].fade -= 5;

                   }
                 }
                else
                 {
                  accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                  accessory[i].onoff = 1;
                  accessory[i].fade = 0;

                  this->state = TT_STOP;
                 }

               }
             }
          break;
#endif




#ifdef ACTION_FLASHALTERNATE
          case 4:                                         // flashing alternate
            if (accessory[i].onoff && millis() > accessory[i].offMilli)
             {
              digitalWrite(accessory[i].outputPin, LOW);
              digitalWrite(accessory[i].outputPin2, HIGH);
              accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
            else
             {
              if (!accessory[i].onoff && millis() > accessory[i].onMilli)
               {
                digitalWrite(accessory[i].outputPin, HIGH);
                digitalWrite(accessory[i].outputPin2, LOW);
                accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                accessory[i].onoff = 1;
                this->state = TT_STOP;
               }
             }
          break;
#endif

#ifdef ACTION_STROBEDOUBLE
          case 5:                                         // strobe double
            if (accessory[i].onoff && millis() > accessory[i].offMilli)
             {
              accessory[i].onMilli = millis() + (accessory[i].offtime * accessory[i].offtimeX);
              accessory[i].onoff = 0;
              this->state = TT_STOP;
             }
            else
             {
              if (!accessory[i].onoff && millis() > accessory[i].onMilli)
               {
                this->strobe(accessory[i].outputPin);
                delay(STROBE_DELAY);
                this->strobe(accessory[i].outputPin);
                accessory[i].offMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
                accessory[i].onoff = 1;
                this->state = TT_STOP;
               }
             }
          break;
#endif

#ifdef ACTION_RANDOMBRIGHT
          case 6:                                         // random bright
            if (accessory[i].onoff && millis() > accessory[i].onMilli)
             {
              accessory[i].fade = random(255);
              analogWrite(accessory[i].outputPin, accessory[i].fade);
              accessory[i].onMilli = millis() + (accessory[i].ontime * accessory[i].ontimeX);
              this->state = TT_STOP;
             }
          break;
#endif


#ifdef ACTION_7
          case 7:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_8
          case 8:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_9
          case 9:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_10
          case 10:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_11
          case 11:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_12
          case 12:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_13
          case 13:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_14
          case 14:
            this->state = TT_STOP;
          break;
#endif

#ifdef ACTION_15
          case 15:
            this->state = TT_STOP;
          break;
#endif


         }
       }
     }

/*
 * 
 */

    if (this->state == TT_STOP)
     {
      memmove(this->commandQueue, this->commandQueue + 1, TT_MOVER_MAX_TRACKS);
      this->state = TT_IDLE;
     }

}




#ifdef ACTION_STROBEDOUBLE
void Lights::strobe(byte pin)
 {
  digitalWrite(pin, HIGH);
  delay(10);
  digitalWrite(pin, LOW);
 }
#endif
  
