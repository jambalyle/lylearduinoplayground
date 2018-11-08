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

float pulseHigh = 0;
float pulseLow = 0;
int period = 0;
int duty = 0;



//seven segment stuff:
#include "SevSeg.h"

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

  pinMode (A0,INPUT);

  
}

void loop() {

 pulseHigh = pulseInLong (A0,HIGH) / 1; // reading pulse from pin A0 in microseconds, timing out if it has to wait longer than 3 seconds, then converting to seconds
 
 pulseLow = pulseInLong (A0,LOW) / 1000000;

 // Calculate the period, or "T", of the frequency, or "f," using the formula: T = 1/f. For example, if the frequency is 20 hz, then T = 1/20, with a result of 0.05 seconds.
 // Determine the duty cycle, represented by "D," through the formula D = PW/T. As an example, if PW is 0.02 seconds and T is 0.05 seconds, then D = 0.02/0.05 = 0.4, or 40%.

  period = pulseLow + pulseHigh; // .5 hz is the frequency,

  duty = (pulseHigh / period) * 100; //calculate the duty cycle and turn it into a percentage
  
  sevseg.setNumber(pulseLow, 2); // display pulse width;
   
  sevseg.refreshDisplay(); // Must run repeatedly
  
}
