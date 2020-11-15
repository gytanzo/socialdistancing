/*
Author: Andrew Zhou and Ben Miller
Purpose: Create a system that helps enforce social distancing.
Assignment: Project 3

References: 

Other: 
*/ 
// Inputs: 2 Inputs. Ultrasonic and Audio Transucer.
/* Outputs: 
2 Outputs for on board LEDs (Red/Green). 
1 Output for the seven segment display. 
1 Output to tell Ultrasonic Transducer to check for sound. 
1 LCD. 
5 Outputs total.
*/
#include "mbed.h"

// Set up Red and Green on board LEDs
DigitalOut redLED(PB_14);
DigitalOut greenLED(PC_7);

Watchdog &watch = Watchdog::get_instance();      // Initialize Watchdog
#define wdTimeout 15000                         // Define watchdog timer
void reset();                                   // Prototype ISR to reset watchdog  

// main() runs in its own thread in the OS
int main()
{
    RCC->AHB2ENR |= 0x6;                    // Enable Clock for GPIOC and GPIOB
    // Use Port B for inputs
    GPIOC->MODER &= ~(0x000F0000);          // Set 0s for 8/9
    // Use Port C for outputs
    GPIOB->MODER &= ~(0x00AA0000);          // Set 0s for Registers 8/9/10/11
    GPIOB->MODER |= 0x00550000;             // Set 1s for Registers 8/9/10/11    

    while (true) {
        // use variable "check" to obtain info on just the 8th bit (PB8)
        int check = ~(0x0100);
        check = GPIOB->IDR & check;
        if(check == 0x0){                   // if PB8 = 0 so sound is detected so do nothing
        
        }
        else if(check == 0x0100){           // if PB8 = 1 so no sound is detected so turn off power to unnecessary instruments

        }

    }
}

void reset(){           // reset watchdog
    watch.kick();
}