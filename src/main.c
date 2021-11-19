// Sample code for ECE 198

// Written by Bernie Roehl, August 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include "ece198.h"

int main(void);

char MorseToChar(char morse[]);

int main(void)
{
    HAL_Init(); // initialize the Hardware Abstraction Layer

    // Peripherals (including GPIOs) are disabled by default to save power, so we
    // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)

    // initialize the pins to be input, output, alternate function, etc...

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED

    // note: the on-board pushbutton is fine with the default values (no internal pull-up resistor
    // is required, since there's one on the board)

    // set up for serial communication to the host computer
    // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)

    SerialSetup(9600);

    // as mentioned above, only one of the following code sections will be used
    // (depending on which of the #define statements at the top of this file has been uncommented)

    while (true) {

        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)); // Waiting for button press

        char inputChar = ' ';

        char buff[100];
        uint32_t time1 = HAL_GetTick(); 

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)); // Waiting for button to be released

        uint32_t time2 = HAL_GetTick();
        uint32_t timePressed = time2 - time1; // Calculating time button is held for

        if(timePressed < 200){
            inputChar = '.';
        } else{
            inputChar = '-';
        }

        sprintf(buff, "Time Pressed: %lu ms\r\n", timePressed);
        SerialPuts(buff);
        sprintf(buff, "Input character: %c \r\n\n", inputChar);
        SerialPuts(buff);
        // Outputing press time

        // SerialPuts(inputChar);



        //Checking for dot vs dash

    }

    return 0;
}

// This function is called by the HAL once every millisecond
void SysTick_Handler(void)
{
    HAL_IncTick(); // tell HAL that a new tick has happened
    // we can do other things in here too if we need to, but be careful
}

char MorseToChar(char morse[]) {
    char morseAlphabet[26][4];

    //A
    char morseAlphabet[0] = {'.', '-'};

    //B
    char morseAlphabet[1] = {'-', '.', '.', '.'};

    //C
    char morseAlphabet[2] = {'-', '.', '-', '.'};

    //D
    char morseAlphabet[3] = {'-', '.', '.'};

    //E
    char morseAlphabet[4] = {'.'};

    //F
    char morseAlphabet[5] = {'.', '.', '-', '.'};

    //G
    char morseAlphabet[6] = {'-', '-', '.'};

    //H
    char morseAlphabet[7] = {'.', '.', '.', '.'};

    //I
    char morseAlphabet[8] = {'.', '.'};

    //J
    char morseAlphabet[9] = {'.', '-', '-', '-'};

    //K
    char morseAlphabet[10] = {'-', '.', '-'};

    //L
    char morseAlphabet[11] = {'.', '-', '.', '.'};

    //M
    char morseAlphabet[12] = {'-', '-'};

    //N
    char morseAlphabet[13] = {'-', '.'};

    //O
    char morseAlphabet[14] = {'-', '-', '-'};

    //P
    char morseAlphabet[15] = {'.', '-', '-', '.'};

    //Q
    char morseAlphabet[16] = {'-', '-', '.', '-'};

    //R
    char morseAlphabet[17] = {'.', '-', '.'};

    //S
    char morseAlphabet[18] = {'.', '.', '.'};

    //T
    char morseAlphabet[19] = {'-'};

    //U
    char morseAlphabet[20] = {'.', '.', '-'};

    //V
    char morseAlphabet[21] = {'.', '.', '.', '-'};

    //W
    char morseAlphabet[22] = {'.', '-', '-'};

    //X
    char morseAlphabet[23] = {'-', '.', '.', '-'};

    //Y
    char morseAlphabet[24] = {'-', '.', '-', '-'};

    //Z
    char morseAlphabet[25] = {'-', '-', '.', '.'};

    char result = '\n';
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 4; j++) {
            if (morseAlphabet[i][j] != morse[j]) {
                break;
            }
            result = i + 65;
            return result;
        }
    }
    return result;
}