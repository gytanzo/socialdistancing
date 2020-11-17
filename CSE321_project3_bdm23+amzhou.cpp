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
int check(int);                                 // Prototype check function
void startCount();                              // Prototype function to start counting.
void lcdwait();                                 // Prototype function for wait text
void lcdgo();                                   // Prototype function for go text
int distance();                                 // Prototype function for distance for ultrasonic transducer

Timer timer;                                    // Initialize Timer


// set up LCD
CSE321_LCD lcd(16,2,LCD_5x8DOTS,PF_0,PF_1);
// PF0 = SDA, PF1 = SCL

// main() runs in its own thread in the OS
int main()
{
    RCC->AHB2ENR |= 0x6;                    // Enable Clock for GPIOC and GPIOB
    // Use Port B for inputs
    GPIOB->MODER &= ~(0x000F0000);          // Set 0s for 8/9
    // Use Port C for outputs
    GPIOC->MODER &= ~(0x00AA002A);          // Set 0s for Registers 0/1/2/8/9/10/11
    GPIOC->MODER |= 0x00550015;             // Set 1s for Registers 0/1/2/8/9/10/11    

    while (true) {
        if (check(8) == 0x0){               // Sound detected, turn everything on 
            watch.start(wdTimeout);         // Start the watchdog
            GPIOC -> ODR |= 0x3;            // Turn on LCD + ultrasonic transducer (and later 7-seg display)
            lcd.begin();
            lcd.print("You may walk.");     // First person gets to walk
            distance();
            while(true){                    // After everything has been turned on
                if (check(8) == 0x0){
                    watch.kick();
                }
                printf("%d\n", distance());

            }
        }
        else;                               // Sound not detected, do nothing 
    }
    return 0;
}

int check(int bit){
    if (bit == 8) {                         // Get data on 8th bit (audio)
        int check = GPIOB->IDR;
        check = check >> 8;
        check &= ~(0xFFFE);
        if(check == 0x0){                   // If PB8 = 0, sound is detected                             
            return 0;                       // Do nothing since no sound is detected
        }
        else if(check == 0x1){              // If PB8 = 1, no sound is detected 
                                          // Do something with Watchdog here
            return 1;
        }
    }
    else if (bit == 9) {                    // Get data on 9th bit (ultrasonic)
        int check = GPIOB->IDR;
        check = check >> 9;
        check &= ~(0xFFFE);
        if(check == 0x0){                   // If PB9 = 0
            printf("am zero");
            return 0;
        }
        else if(check == 0x1){              // If PB9 = 1
            printf("am one");
            return 1;
        }
    }
    return -1;
}

void startCount(){
    ;
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

int distance(){
    GPIOC->ODR |= 0x400;
    while(check(9) == 0);
    printf("Distance is run1\n");
    timer.start();
    printf("Distance is run2\n");
    while(check(9) == 1);
    printf("Distance is run3\n");
    timer.stop();
    printf("Distance is run4\n");
    int inch = timer.read_us();
    printf("Distance is run5\n");
    inch = inch / 148;
    printf("Distance is run6\n");
    GPIOC->ODR &= ~(0x400);
    printf("Distance is run7\n");
    timer.reset();
    printf("Distance is run8\n");
    return inch;
}