/* Infrared receiver test 
 *  MGP Feb 2016
 *  Decodes message sent by Trilobot_IR_transmitter_test.ino
 *  prints to serial port
 *  
 *  IR receiver device across pins 2,3,4 (receive, gnd, pwr)
 *  
 *  modified from IRrecord
   by: Jim Lindblom
   SparkFun Electronics
   date: October 1, 2013



*/
#include <IRremote.h> // Include the IRremote library

/* Connect the output of the IR receiver diode to pin 11. */
int IR_POWER_PIN = 4;
int IR_GND_PIN   = 3;
int RECV_PIN     = 2;
/* Initialize the irrecv part of the IRremote  library */
IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes

IRsend irsend; // Enables IR transmit on pin 3

/* Storage variables for the received IR code: */
unsigned int irLen = 0; // The length of the code
unsigned int irBuffer[RAWBUF]; // Storage for raw (UNKNOWN) codes
int codeType; // The type of code (NEC, SONY, UNKNOWN, etc.)
unsigned int codeValue; // The value of a known code
boolean codeReceived = false; // A boolean to keep track of if a code was received

//const int buttonPin = 12; // Button pin. Pulled up. Triggered when connected to ground.

// setup() initializes serial and the Infrared receiver.
void setup()
{
  pinMode(IR_POWER_PIN, OUTPUT);
  pinMode(IR_GND_PIN, OUTPUT);
  digitalWrite(IR_POWER_PIN, HIGH);
  digitalWrite(IR_GND_PIN, LOW);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

//  pinMode(buttonPin, INPUT_PULLUP);
}

// loop() checks for either a button press or a received IR code.
void loop()
{


  if (irrecv.decode(&results)) 
  {
      Serial.println("hello");
    updateIRReceive(); // Sort out the received code
    codeReceived = true; // Enables code transmit on button press
    irrecv.resume(); // re-enable receive
  }


}

/* This function reads in the received IR data, and updates the
   following variables:
    * codeType -- How was the IR signal encoded? SONY, NEC, 
      UNKNOWN, etc.
    * irLen -- The length (number of marks and ticks) of an IR code.
    * Depending on the codeType, one of these two variables will
      be updated as well:
      * codeValue -- IF the codeType is a known type, this variable
        will contain the received code.
      * irBuffer -- If the codeType is UNKNOWN, this buffer will
        contain the format of the received code.

    This function borrows heavily from the IRrecord example code.
*/
void updateIRReceive()
{
  codeType = results.decode_type;
  irLen = results.rawlen;
  if (codeType == UNKNOWN)
  {
    irLen--;
    // We need to convert from ticks to microseconds
    for (int i = 1; i <= irLen; i++) 
    {
      if (i % 2) 
      {
        // Mark
        irBuffer[i-1] = results.rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      } 
      else 
      {
        // Space
        irBuffer[i-1] = results.rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(irBuffer[i-1], DEC);
    }
    Serial.println();
  }
  else 
  {
    if (codeType == NEC) 
    {
      Serial.print("Received NEC. Codelength = 0x");
      Serial.print(irLen, HEX); Serial.print(", Code = 0x");
      if (results.value == REPEAT) 
      {

        Serial.print("(REPEAT)");
        //return;
      }
    } 
//    else if (codeType == SONY) 
//    {
//      Serial.print("Received SONY: ");
//    } 
//    else if (codeType == RC5) 
//    {
//      Serial.print("Received RC5: ");
//    } 
//    else if (codeType == RC6) 
//    {
//      Serial.print("Received RC6: ");
//    } 
    else 
    {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results.value, HEX);
    codeValue = results.value;
    irLen = results.bits;
  }
}


