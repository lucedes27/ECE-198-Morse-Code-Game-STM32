// Sample code for ECE 198

// Written by Bernie Roehl, August 2021

// This file contains code for a number of different examples.
// Each one is surrounded by an #ifdef ... #endif block inside of main().

// To run a particular example, you should remove the comment (//) in
// front of exactly ONE of the following lines:

#include <stdbool.h> // booleans, i.e. true and false
#include <stdio.h>   // sprintf() function
#include <stdlib.h>  // srand() and random() functions

#include <limits.h>

#include "ece198.h"

int main(void);

char MorseToChar(char morse[], char morseAlphabet[][4]);

void outputMorse(char morse[], size_t capacity, char morseAlphabet[][4]);

bool isEqual(char str1[], char str2[]);

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

    uint32_t time2 = (INT_MIN/2) - 1;

    bool considerSpace = false;

    char userInput[4] = {'\0', '\0', '\0', '\0'};
    char inputChar = ' ';
    int userCounter = 0;

    char cityInput[85];
    size_t cityIndex = 0;

    char BERLIN[7] = {'B', 'E', 'R', 'L', 'I', 'N', '\0'};

    char buff[100];
  
  char morseAlphabet[26][4] = {
        //A
        {'.', '-'},

        //B
        {'-', '.', '.', '.'},

        //C
        {'-', '.', '-', '.'},

        //D
        {'-', '.', '.'},

        //E
        {'.'},

        //F
        {'.', '.', '-', '.'},

        //G
        {'-', '-', '.'},

        //H
        {'.', '.', '.', '.'},

        //I
        {'.', '.'},

        //J
        {'.', '-', '-', '-'},

        //K
        {'-', '.', '-'},

        //L
        {'.', '-', '.', '.'},

        //M
        {'-', '-'},

        //N
        {'-', '.'},

        //O
        {'-', '-', '-'},

        //P
        {'.', '-', '-', '.'},

        //Q
        {'-', '-', '.', '-'},

        //R
        {'.', '-', '.'},

        //S
        {'.', '.', '.'},

        //T
        {'-'},

        //U
        {'.', '.', '-'},

        //V
        {'.', '.', '.', '-'},

        //W
        {'.', '-', '-'},

        //X
        {'-', '.', '.', '-'},

        //Y
        {'-', '.', '-', '-'},

        //Z
        {'-', '-', '.', '.'}

    };

    char SOS[3] = {'S', 'O', 'S'};

    while (true) {

        int timeSincePressed = 0;

        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
          
          	//outputMorse(SOS, 3, morseAlphabet);

            timeSincePressed = HAL_GetTick() - time2;

            if(considerSpace && (timeSincePressed > 1400)){
                SerialPuts("Space\r\n");
                considerSpace = false;

                if(MorseToChar(userInput, morseAlphabet) != '\0'){
                
                    sprintf(buff, "Letter: %c\r\n", MorseToChar(userInput, morseAlphabet));
                    SerialPuts(buff);

                    cityInput[cityIndex] = MorseToChar(userInput, morseAlphabet);
                    cityIndex++;

                    if (isEqual(cityInput, BERLIN)) {
                        SerialPuts("You saved Europe!");
                    }

                }

                for (int i = 0; i < 4; i++) {
                    userInput[i] = '\0';
                    userCounter = 0;
                }
            }

        } // Waiting for button press

        
        uint32_t time1 = HAL_GetTick(); 

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)); // Waiting for button to be released

        time2 = HAL_GetTick();
        uint32_t timePressed = time2 - time1; // Calculating time button is held for

        if(timePressed < 200){
            inputChar = '.';
        } else{
            inputChar = '-';
        }

        userInput[userCounter] = inputChar;
        userCounter++;
        considerSpace = true;

        // Outputing press time
        sprintf(buff, "Time Pressed: %lu ms\r\n", timePressed);
        SerialPuts(buff);
        //Outputting inputChar
        sprintf(buff, "Input character: %c \r\n\n", inputChar);
        SerialPuts(buff);        



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

char MorseToChar(char morse[], char morseAlphabet[][4]) {

    for (int i = 0; i < 26; i++) {
        if ((morseAlphabet[i][0] == morse[0]) && (morseAlphabet[i][1] == morse[1]) && (morseAlphabet[i][2] == morse[2]) && (morseAlphabet[i][3] == morse[3])) {
            return i + 65;
            SerialPuts("Match found\r\n");
        }
    }

    SerialPuts("Invalid\r\n");
    return '\0';
}

void outputMorse(char morse[], size_t capacity, char morseAlphabet[][4]){
	size_t letterIndex = 0;
    for(size_t i = 0; i < capacity; i++){
        letterIndex = morse[i] - 65;
        for(size_t j = 0; j < 4; j++){
            if(morseAlphabet[letterIndex][j] != '\0'){
                if(morseAlphabet[letterIndex][j] == '.') {
                    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                    delayedToggle(200, GPIOA, GPIO_PIN_5);
                    HAL_Delay(1400);
                }
                if(morseAlphabet[letterIndex][j] == '-'){
                    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                    delayedToggle(600, GPIOA, GPIO_PIN_5);
                    HAL_Delay(1400);
                }  
            }

        }
    }

    HAL_Delay(5000);

}

bool isEqual(char str1[], char str2[]) {
    int i = 0;
    while (str1[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return str2[i] == '\0';
}