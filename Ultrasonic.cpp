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

#include "mbed.h"
#include "Ultrasonic.h"

Timer timer;

Ultrasonic::Ultrasonic(PinName TP, PinName EP)
{
    /*
    Nothing gets done in this function. 
    Originally, this would take the TP and EP arguments to set the trigger pin and echo pin...
    However, the differences between the Arduino and the Nucleo are too significant.
    A result of these differences is that this function is no longer used.
    I am leaving it in here simply to make the code easier to understand.
    I believe it makes sense for the reader to know where PC_10 and PB_9 are being used. 
    In other words, you can ignore this function. The functions that are actually used are explained below.
    Cheers,

    -Ben
    */
}

long Ultrasonic::Timing()                       // Finds the duration of the echo
{
    // GPIOC modifies PC_10, which is the trigger bit
    // GPIOB modifies PB_9, which is the echo bit
    timer.reset();                              // Reset the timer in case it already has a value
    GPIOC -> ODR &= ~(0x400);                   // Turn trigger pin off 
    wait_us(2);                                 // Wait for 2 us
    GPIOC -> ODR |= 0x400;                      // Turn trigger pin back on
    wait_us(10);                                // Wait for 10 us; this is what gives the echo pin power
    GPIOC -> ODR &= ~(0x400);                   // Turn trigger pin back off
    while (!(GPIOB -> IDR >> 9));               // Echo pin is sending...
    timer.start();                              // Turn timer on
    while (GPIOB -> IDR >> 9);                  // Echo pin is receiving...
    timer.stop();                               // Turn timer off
    duration = timer.elapsed_time().count();    // We need to return the duration of the echo; it is converted into a long and stored in a private variable
    
    return duration;                            // Return the duration of the echo; this doesn't seem to get used and appears to be for error prevention
}

long Ultrasonic::Ranging(int unit)              // Takes the above duration and uses it to calculate the distance between the ultrasonic transducer and an object
{
    Timing();                                   // Activate the timing function and store the duration
    distance_cm = duration / 29 / 2 ;           // Calculate distance in centimeters
    distance_inc = duration / 74 / 2;           // Calculate distance in feet
    if (unit)                                   // Find what unit user is asking for
    return distance_cm;                         // Return centimeters
    else
    return distance_inc;                        // Return inches
}