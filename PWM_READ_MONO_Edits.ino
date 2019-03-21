/*|===========================|
 *|ppppp   w  w  w   mmm mm   |
 *|p    p  w  w  w   m  m  m  |
 *|ppppp    ww ww    m  m  m  |
 *|p----NEW_ORLEANS_MAKERS----|
 *|p==========================|
 *
 *,____,_________________,
 *|    |                 |
 *|  5v|>-------, - - - -|
 *|    ||||||||||        |
 *|    ||||||||||        |
 *|2.5v|>-------|--------|
 *|    |        ||||||||||
 *|    |        ||||||||||
 *|  0v|>  -  - '--------|
 *|____|_________________|
 */
/*

  Pinout:
  1: Segment E
  2: Segment D
  3: Segment DP
  4: Segment C
  5: Segment G
  6: Digit 4
  7: Segment B
  8: Digit 3
  9: Digit 2
  10: Segment F
  11: Segment A
  12: Digit 1

  ToDo:
  Picture of setup with pin 1 indicator
  Covert big byte array to binary: http://arduino.cc/forum/index.php/topic,39760.0.html
  Measure current going through limiting resistors to make sure we're getting 20mA per segment per digit (should be 80mA for four digits)

  2264 bytes
  2134 bytes with new BigTime functions
  2214 if full DP support

*/

//Header files/Libs

//seven segment stuff:
#include "SevSeg.h"
#include "elapsedMillis.h"


// Variable
float pulseHigh = 0;
float pulseTotal = 0;
float pulseLow = 0;
int period = 0;
int duty = 0;
int delay0 = 0;
bool sw0 = true;

int incomingByte = 0;
long interval = 5000;
elapsedMillis timePeriod;


//Initiate a seven segment controller object
SevSeg sevseg;

void setup() {

  Serial.begin(9600);

  //seven segment setup
  byte numDigits = 4;
  byte digitPins[] = {13, 10, 9, 7};
  byte segmentPins[] = {12, 8, 5, 3, 2, 11, 6, 4};
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);

  // Signal Input setup

  pinMode (A0, INPUT);


}

void loop() {
  // This block of code 
  if(sw0){
      sw0 = false;
      delay0 = 0;
      sevseg.blank();
      // Detect 
      pulseHigh = pulseInLong(A0,HIGH,3*(1000000));
      
      if(pulseHigh >0){
        pulseLow = pulseInLong(A0,LOW,3*(1000000));
        duty = pulseHigh*100/(pulseHigh + pulseLow);
      }else{
        // Edge case: when duty = 0% or 100%
        pulseLow = 0;
        if(analogRead(A0)>=552)
           duty = 100;
        else
           duty = 0;
      }
  }
  
  // For serial debug, set the following if to true.
  if(false){ 
  //  Serial.print("ananlogVal(A0): ");
  //  Serial.println(analogRead(A0));
  //  Serial.print("pulseHigh: ");
  //  Serial.println(pulseHigh);
  //  Serial.print("pulseLow: ");
  //  Serial.println(pulseLow);
    Serial.print("sw0: ");
    Serial.println(sw0);
    Serial.print("duty: ");
    Serial.print(duty);
    Serial.println("%\n");
  }
  sevseg.setNumber(duty, 2); // display pulse width;

  sevseg.refreshDisplay(); // Must run repeatedly

  delay0++;
  if(delay0 > 5000){
     sw0 = true;  
  }
}
