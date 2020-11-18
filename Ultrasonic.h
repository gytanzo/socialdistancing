/*
  ORIGINAL HEADER - From manufacturer
  Ultrasonic.h - Library for HR-SC04 Ultrasonic Ranging Module.
  Created by ITead studio. Alex, Apr 20, 2010.
  iteadstudio.com
*/

/*
  NEW HEADER - From students
  Ultrasonic.h - Adapted from HR-SCO4 Documentation
  Ben Miller and Andrew Zhou, November 18, 2020
*/


#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "mbed.h"

#define CM 1                                                                // Put units in centimeters
#define INC 0                                                               // Put units in inches

class Ultrasonic
{
  public:                                                                   // Viewable outside 
    Ultrasonic(PinName TP, PinName EP);                                     // TP = Trigger pin, EP = Echo pin
    long Timing();                                                          // Defined in Ultrasonic.cpp
    long Ranging(int unit);                                                 // Defined in Ultrasonic.cpp

    private:                                                                // Not viewable outside
    long duration;                                                          // Duration of the echo pulse
    long distance_cm;                                                       // Distance in centimeters
    long distance_inc;                                                      // Distance in inches
    
};

#endif