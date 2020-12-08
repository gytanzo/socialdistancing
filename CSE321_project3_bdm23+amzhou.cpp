/*
File: CSE321_project3_bdm23+amzhou.cpp

Author 1: Ben Miller (bdm23@buffalo.edu)
Author 2: Andrew Zhou (amzhou@buffalo.edu)
Date: Fall 2020
Course: CSE321

Summary of File:
    
    Purpose: Implement a social distancing enforcement system. 
    Modules/Subroutines: Pre-defined functions include...
        check: Accepts an int. Used to check the value of specific pins.
        startCount: Hasn't been defined yet, I believe its purpose is to update the seven-segment display timer.
        lcdwait: Tells a person when to stop.
        lcdgo: Tells a person when to go.
    Assignment: Project 3. 
    Input(s): An ultrasonic and audio transducer. 
    Output(s): LEDs, an LCD, and a seven-segment display. 
    Constraint(s): See documentation (specifically [Project 3] Part 3 A.pdf)
    Source references: References include...
        Documentation: Documentation created for this project will be located in the appropriately named documentation folder. 
        Ultrasonic files: Ultrasonic files are adapted from the HC_SR04's Github repo, found here: https://github.com/CytronTechnologies/Arduno_UltrasonicRangingModule
            These files are also appropriately credit in our iterations of them. 
*/


#include "mbed.h"
#include "1802.h"                               // location of prototyping and definitions for 1802 LCD
#include "Ultrasonic.h"                         // location of prototyping and definitions for ultrasonic transducer
#include "DigitDisplay.h"                       // location of prototyping and definitions for seven segment display
#include <cstdint>

// Set red and green onboard LEDs as digital outputs
DigitalOut redLED(PB_14);                   
DigitalOut greenLED(PC_7);

Watchdog &watch = Watchdog::get_instance();     // Initialize Watchdog
#define wdTimeout 30000                         // Define watchdog timer as 30 seconds

// Pre-define functions
int check(int);                                 // Function to check value of pins
void startCount();                              // Function for seven segment display timer
void lcdwait();                                 // Function to print to LCD to tell person to wait
void lcdgo();                                   // Function to print to LCD to give person permission to go

// set up LCD
CSE321_LCD lcd(16,2,LCD_5x8DOTS,PF_0,PF_1);     // PF0 = SDA, PF1 = SCL
Ultrasonic ultrasonic(PC_10, PB_9);             // PC_10 is TP, PB_9 is EP; these values are not used but rather included for transparency and ease of understanding
DigitDisplay dd(PC_4, PC_5);                    // dd is digital display; PC_4 = CLK, PC_5 = DIO

Ticker tick;                                    // Ticker to run counting function
int timeLeft = 3;                               // To keep track of the time left

int main()
{
    dd.on();                                    // Initialize 7-Seg Display
    dd.clear();                                 // Clear anything leftover 
    RCC->AHB2ENR |= 0x6;                        // Enable Clock for GPIOC and GPIOB
    // Use Port B for inputs
    GPIOB->MODER &= ~(0xF0000);                 // Set 0s for 8/9
    // Use Port C for outputs
    GPIOC->MODER &= ~(0x20002A);                // Set 0s for Registers 0/1/2/10
    GPIOC->MODER |= 0x100015;                   // Set 1s for Registers 0/1/2/10   

    while (true) {
        if (check(8) == 0x0){                   // Sound detected; turn everything on 
            watch.start(wdTimeout);             // Start watchdog
            GPIOC -> ODR |= 0x7;                // Turn on LCD, ultrasonic transducer, (and later) seven segment display

            lcd.begin();                        // Initialize LCD
            lcdgo();                            // First person gets to walk

            long base = ultrasonic.Ranging(INC);    // Base distance detected

            while (base > 156) {                // Edge case where distance jumpts up to 475 in
                base = ultrasonic.Ranging(INC); // Re-adjust base value
            } 
            while(true){    
                long distance = ultrasonic.Ranging(INC);
                if (distance <= 156){           // Edge case where distance jumps up to 475 in
                    long difference = base - distance;
                    if (difference < 0){        // Find absolute value of difference
                        difference = difference * -1;
                    }
                    if (difference > 10){        // Value sometimes bounces, this solves that issue
                        watch.kick();
                        timeLeft = 3000;
                        tick.attach(&startCount, .001);
                        lcdwait();
                        while(timeLeft > 0){
                            printf("%d\n", timeLeft);
                        }
                        tick.detach();
                        lcdgo();
                    }
                }
            }
        } 
    }
    return 0;                                   // Precaution against errors
}

int check(int bit){                             // Returns the value of a bit given its pin
    if (bit == 8) {                             // Get data on 8th bit (audio)
        int check = GPIOB->IDR;                 // Output is in GPIOB IDR
        check = check >> 8;                     // Shift right eight bits, this just makes masking easier
        check &= ~(0xFFFE);                     // Mask every bit except the one we want to check
        if(check == 0x0){                       // If PB8 = 0, sound is detected                             
            return 0;                       
        }
        else if(check == 0x1){                  // If PB8 = 1, no sound is detected 
            return 1;
        }
    }
    return -1;                                  // If -1 gets returned, something very bad has happened
}

void startCount(){                              // Andrew is eventually going to do something with this
    timeLeft -= 1;
    if (timeLeft >= 0){
        dd.write(timeLeft);
    }
}

void lcdwait(){                                 // Tell person to kindly wait for their turn
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please Wait.");
}

void lcdgo(){                                   // Give whoever is wait the go-ahead 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You may walk.");
}