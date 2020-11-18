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

Timer timeru;

Ultrasonic::Ultrasonic(DigitalOut TP, DigitalIn EP)
{
    Trig_pin=TP;
    Echo_pin=EP;
}

long Ultrasonic::Timing()
{
    printf("Before anything is done.");
    Trig_pin = 0;
    wait_us(2);
    Trig_pin = 1;
    wait_us(10);
    Trig_pin = 0;
    printf("Before the first while loop.");
    while(!Echo_pin);   
    printf("After the first while loop.");                                                                  // wait for high
    timeru.start();
    while(Echo_pin);                                                                                        // wait for low
    timeru.stop();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(timeru.elapsed_time()).count();         // Convert chrono::microseconds to long
    return duration;
}

long Ultrasonic::Ranging(int sys)
{
  Timing();
  distance_cm = duration / 29 / 2 ;
  distance_inc = duration / 74 / 2;
  if (sys)
  return distance_cm;
  else
  return distance_inc;
}