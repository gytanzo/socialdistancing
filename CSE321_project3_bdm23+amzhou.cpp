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
#include "1802.h"                           // location of prototyping and definitions for 1802 LCD
#include "Ultrasonic.h"

// Set red and green onboard LEDs as digital outputs
DigitalOut redLED(PB_14);                   
DigitalOut greenLED(PC_7);

Watchdog &watch = Watchdog::get_instance();     // Initialize Watchdog
#define wdTimeout 15000                         // Define watchdog timer as 15 seconds

// Pre-define functions
int check(int);                                 // Function to check value of pins
void startCount();                              // Function for seven segment display timer
void lcdwait();                                 // Function to print to LCD to tell person to wait
void lcdgo();                                   // Function to print to LCD to give person permission to go

// Any other objects should be set up here

// set up LCD
CSE321_LCD lcd(16,2,LCD_5x8DOTS,PF_0,PF_1);     // PF0 = SDA, PF1 = SCL
Ultrasonic ultrasonic(PC_10, PB_9);             // PC_10 is TP, PB_9 is EP; these values are not used but rather included for transparency and ease of understanding

int main()
{
    RCC->AHB2ENR |= 0x6;                    // Enable Clock for GPIOC and GPIOB
    // Use Port B for inputs
    GPIOB->MODER &= ~(0xF0000);             // Set 0s for 8/9
    // Use Port C for outputs
    GPIOC->MODER &= ~(0xAA002A);            // Set 0s for Registers 0/1/2/8/9/10/11
    GPIOC->MODER |= 0x550015;               // Set 1s for Registers 0/1/2/8/9/10/11    

    GPIOC -> ODR |= 0x3;                    // Turn on LCD (and ultrasonic transducer's old port)
    lcd.begin();                            // Initialize LCD
    lcd.print("You may walk.");             // First person gets to walk

    while (true) {
        printf("Object was %ld inches away!\n", ultrasonic.Ranging(INC));
    } 

    return 0;                               // Precaution against errors
}

int check(int bit){                         // Returns the value of a bit given its pin
    if (bit == 8) {                         // Get data on 8th bit (audio)
        int check = GPIOB->IDR;             // Output is in GPIOB IDR
        check = check >> 8;                 // Shift right eight bits, this just makes masking easier
        check &= ~(0xFFFE);                 // Mask every bit except the one we want to check
        if(check == 0x0){                   // If PB8 = 0, sound is detected                             
            return 0;                       
        }
        else if(check == 0x1){              // If PB8 = 1, no sound is detected 
            return 1;
        }
    }

    /*
    I feel like this part of the code is now useless, but I'll check in with my partner before deleting it.

    else if (bit == 9) {                    // Get data on 9th bit (ultrasonic); same logic as above
        int check = GPIOB->IDR;
        check = check >> 9;
        check &= ~(0xFFFE);
        if(check == 0x0){                   // If PB9 = 0, echo is currently 1
            return 0;
        }
        else if(check == 0x1){              // If PB9 = 1, echo is current 0 
            return 1;
        }
    }
    */
    return -1;                              // If -1 gets returned, something very bad has happened
}

void startCount(){                          // Andrew is eventually going to do something with this
    ;
}

void lcdwait(){                             // Tell person to kindly wait for their turn
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please Wait.");
}

void lcdgo(){                               // Give whoever is waiting the go-ahead 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You may walk.");
}