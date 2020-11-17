/*
File: CSE321_project3_bdm23+amzhou.cpp

Author 1: Ben Miller (bdm23@buffalo.edu)
Author 2: Andrew Zhou (amzhou@buffalo.edu)
Date: Fall 2020
Partner: See authors
Course: CSE321

Summary of File:
    
    Purpose: Implement a social distancing enforcement system. 
    Modules/Subroutines: reset(); (will fill out more as coding continues)
    Assignment: Project 3. 
    Input(s): An ultrasonic and audio transducer. 
    Output(s): LEDs, an LCD, and a seven-segment display. 
    Constraint(s): See documentation (specifically [Project 3] Part 3 A.pdf)
    Source references: Any references will be in the documentation folder. 
    
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