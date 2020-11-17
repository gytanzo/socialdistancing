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

// Set up Red and Green on board LEDs
DigitalOut redLED(PB_14);
DigitalOut greenLED(PC_7);

Watchdog &watch = Watchdog::get_instance();     // Initialize Watchdog
#define wdTimeout 15000                         // Define watchdog timer

// Pre-define functions
void reset();                                   // Prototype ISR to reset watchdog 
int check(int);                                 // Prototype check function
void startCount();                              // Prototype function to start counting.
void lcdwait();                                 // Prototype function for wait text
void lcdgo();                                   // Prototype function for go text

// set up LCD
CSE321_LCD lcd(16,2,LCD_5x8DOTS,PF_0,PF_1);
// PF0 = SDA, PF1 = SCL

// main() runs in its own thread in the OS
int main()
{
    lcd.begin();                            // Initialize LCD
    RCC->AHB2ENR |= 0x6;                    // Enable Clock for GPIOC and GPIOB
    // Use Port B for inputs
    GPIOB->MODER &= ~(0x000F0000);          // Set 0s for 8/9
    // Use Port C for outputs
    GPIOC->MODER &= ~(0x00AA0000);          // Set 0s for Registers 8/9/10/11
    GPIOC->MODER |= 0x00550000;             // Set 1s for Registers 8/9/10/11    

    while (true) {
        int PB8_value = check(8);           // What is the value of PB8?
        int PB9_value = check(9);           // What is the value of PB9?
        lcdgo();
        thread_sleep_for(5000);
        lcdwait();
        thread_sleep_for(5000);
    }
    return 0;
}

int check(int bit){
    if (bit == 8) {                         // Get data on 8th bit (audio)
        int check = GPIOB->IDR;
        check = check >> 8;
        check &= ~(0xFFFE);
        printf("%d\n", check);
        if(check == 0x0){                   // If PB8 = 0, sound is detected
            ;                               // Do nothing since no sound is detected
            printf("Sound Detected\n");
            return 0;
        }
        else if(check == 0x1){              // If PB8 = 1, no sound is detected 
            ;                               // Do something with Watchdog here
            printf("No Sound\n");
            return 1;
        }
    }
    else if (bit == 9) {                    // Get data on 9th bit (ultrasonic)
        int check = ~(0x0100);
        check = GPIOB->IDR & check;
        if(check == 0x0){                   // if PB9 = 0 (something happens)
        return 0;
        }
        else if(check == 0x0100){           // if PB9 = 1 (something happens)
        return 1;
        }
    }
    return -1;
}

void reset(){ // ISR to reset Watchdog
    watch.kick();
}

void startCount(){
    
}

void lcdwait(){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Please Wait.");
}

void lcdgo(){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("You may walk.");
}