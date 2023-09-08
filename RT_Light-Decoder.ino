/*
 * This is an accessory decoder to control lights.
 * 
 * 
 * 
 */

#include <NmraDcc.h>
#include "defines.h"

#ifndef FORCE_RESET_FACTORY_DEFAULT_CV && ARDUINO_AVR_DIGISPARK
#include "Lights.h"
#endif

#include "variables.h"

#include "functions.h"

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
void notifyCVResetFactoryDefault()
{
  // Make FactoryDefaultCVIndex non-zero and equal to num CV's to be reset 
  // to flag to the loop() function that a reset to Factory Defaults needs to be done
  FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs)/sizeof(CVPair);
};
#endif

#ifdef ENABLE_DCC_ACK

// This function is called by the NmraDcc library when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain on the power supply for 6ms to ACK a CV Read 
void notifyCVAck(void)
{
  Serial.println("notifyCVAck") ;
  
  digitalWrite( ENABLE_DCC_ACK, HIGH );
  delay( 10 );  
  digitalWrite( ENABLE_DCC_ACK, LOW );
}
#endif


#ifdef NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
{
  Serial.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    Serial.print(Msg->Data[i], HEX);
    Serial.write(' ');
  }
  Serial.println();
}
#endif

/*
// This function is called whenever a normal DCC Turnout Packet is received and we're in Board Addressing Mode
void notifyDccAccTurnoutBoard( uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower )
{
  Serial.print("notifyDccAccTurnoutBoard: ") ;
  Serial.print(BoardAddr,DEC) ;
  Serial.print(',');
  Serial.print(OutputPair,DEC) ;
  Serial.print(',');
  Serial.print(Direction,DEC) ;
  Serial.print(',');
  Serial.println(OutputPower, HEX) ;
}
*/

// This function is called whenever a normal DCC Turnout Packet is received and we're in Output Addressing Mode
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{

#ifdef TESTING_DCC
  showAcknowledge(2);
#endif

#ifdef  NOTIFY_TURNOUT_MSG
  Serial.print("notifyDccAccTurnoutOutput: Turnout: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(" Direction: ");
  Serial.print(Direction ? "Closed" : "Thrown") ;
  Serial.print(" Output: ");
  Serial.print(OutputPower ? "On" : "Off") ;
#endif

#ifdef LEARNING
// check to see if in learning mode and update address
  if (learningMode == HIGH) {

//    int H = (Addr - 1) / 64;
//    int L = Addr - (H * 64);
    byte L = (Addr + 3) / 4;
    byte H = (Addr + 3) / 1024;

#ifdef DEBUG_MSG
    Serial.println("");
    Serial.print(F("Value = ")); Serial.println(Addr,DEC);
    Serial.print(F(" H = ")); Serial.println(H,DEC);
    Serial.print(F(" L = ")); Serial.println(L,DEC);
#endif
                  
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_MSB, H);
    Dcc.setCV(CV_ACCESSORY_DECODER_ADDRESS_LSB, L);

   }
  else
#endif
   {

    if(( Addr >= BaseDecoderAddress ) && ( Addr < (BaseDecoderAddress + MAXACCESSORIES )) && OutputPower )
     {

      thisCommand = ( ( ( Addr - BaseDecoderAddress ) * 10 ) + 1 ) + Direction;

#ifndef FORCE_RESET_FACTORY_DEFAULT_CV && ARDUINO_AVR_DIGISPARK
      lLights.addCommand(thisCommand);
#endif

#ifdef  DEBUG_MSG
      Serial.print("ndato thisCommand: ");
      Serial.println(thisCommand,DEC);
#endif
     }

   }

#ifdef  NOTIFY_TURNOUT_MSG
  Serial.println();
#endif

}

/*
// This function is called whenever a DCC Signal Aspect Packet is received
void notifyDccSigOutputState( uint16_t Addr, uint8_t State)
{
  Serial.print("notifyDccSigOutputState: ") ;
  Serial.print(Addr,DEC) ;
  Serial.print(',');
  Serial.println(State, HEX) ;
}
*/

/**********************************************************************
 * setup
 */

void setup()
{
  pinMode(2, INPUT);
  
#ifdef ENABLE_SERIAL
  Serial.begin(115200);
  uint8_t maxWaitLoops = 255;
  while(!Serial && maxWaitLoops--)
    delay(20);
#endif

  // Configure the DCC CV Programing ACK pin for an output
#ifdef ENABLE_DCC_ACK
  pinMode( DccAckPin, OUTPUT );
#endif

#ifndef FORCE_RESET_FACTORY_DEFAULT_CV && ARDUINO_AVR_DIGISPARK
  lLights.init(Dcc);
#endif

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up
  // Many Arduino Cores now support the digitalPinToInterrupt() function that makes it easier to figure out the
  // Interrupt Number for the Arduino Pin number, which reduces confusion. 

#ifdef digitalPinToInterrupt
  Dcc.pin(DCC_PIN, 0);
#else
  Dcc.pin(0, DCC_PIN, 1);
#endif

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init( MAN_ID_DIY, DCC_DECODER_VERSION_NUM, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );

#ifdef ENABLE_SERIAL
//  Serial.println("NMRA DCC Example 1");
  Serial.print("Rosscoe Train Lights Decoder Version: ");Serial.println(DCC_DECODER_VERSION_NUM,DEC);
#endif

#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
#ifdef ENABLE_SERIAL
  Serial.println("Resetting CVs to Factory Defaults");
#endif
  notifyCVResetFactoryDefault(); 
#endif

  BaseDecoderAddress = (((Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_MSB) * 256) + Dcc.getCV(CV_ACCESSORY_DECODER_ADDRESS_LSB) - 1) * 4) + 1  ;

#ifdef DEBUG_MSG
  Serial.print("DCC Base Address: "); Serial.println(BaseDecoderAddress, DEC);
#endif

#ifndef ARDUINO_AVR_DIGISPARK
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(600);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(600);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(600);
  digitalWrite(LED_BUILTIN, LOW);
#else
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1200);
  digitalWrite(LED_BUILTIN, LOW);
#endif

#ifdef ENABLE_SERIAL
  Serial.println("Init Done");
#endif
}

void loop()
{
#ifdef LEARNING
  static int learningbuttonOldval = 0,learningbuttonVal = 0;
#endif
  
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();

#ifndef FORCE_RESET_FACTORY_DEFAULT_CV && ARDUINO_AVR_DIGISPARK
  lLights.process();
#endif
  
#ifdef FORCE_RESET_FACTORY_DEFAULT_CV
  if( FactoryDefaultCVIndex && Dcc.isSetCVReady())
  {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array 
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV, FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }
#endif

#ifdef LEARNING
  learningbuttonVal = dr(LEARNINGBUTTON);

  if (learningbuttonOldval != learningbuttonVal) {
    learningMode = learningbuttonVal;
    if (learningMode == HIGH) showAcknowledge(3);
   }
  learningbuttonOldval = learningbuttonVal;
#endif

#ifdef ENABLE_SERIAL
    // see if there are serial commands
  readString="";              //empty for next input

  while (Serial.available())
   {
    char c = Serial.read();     //gets one byte from serial buffer
    readString += c;            //makes the string readString
    delay(10);                   //slow looping to allow buffer to fill with next character
   }

  // act on serial commands

  if (readString.length() >0)
   {
    doSerialCommand(readString);
   } 
#endif

}




  
