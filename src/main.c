/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @authors        : Thomas Hart and Luc Edes
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ece198.h"
#include "lcd16x2_i2c.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A
  __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B
  __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C
  InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  uint32_t time2 = (INT_MIN/2) - 1;

    // Should we consider adding a space? Not at beginning, so we initialize to false
    bool considerSpace = false;

    // Array of characters to keep track of user input before translating to ASCII character
    char userInput[4] = {'\0', '\0', '\0', '\0'};
    char inputChar = ' ';
    int userCounter = 0;

    // Array to keep track of ASCII characters and check against correct answers
    char cityInput[85] = {'\0'};
    size_t cityIndex = 0;
  
    // Array of countries and corresponding cities of interest during WW2
    char const COUNTRIES[13][16] = {"GERMANY", "UNITED KINGDOM", "SPAIN", "ITALY", "UKRAINE", "BELGIUM", "FRANCE", "POLAND", "CZECHOSLOVAKIA", "NETHERLANDS", "AUSTRIA", "YUGOSLAVIA", "NORWAY"};
    char const CITIES[13][16] = {"BERLIN", "LONDON", "MADRID", "ROME", "KIEV", "BRUSSELS", "PARIS", "WARSAW", "PRAGUE", "AMSTERDAM", "VIENNA", "BELGRADE", "OSLO"};
  
    // Morse alphabet dictionary
    char const MORSE_ALPHABET[26][4] = {
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



  if(lcd16x2_i2c_init(&hi2c1)) {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  }
  lcd16x2_i2c_setCursor(0,0);
  lcd16x2_i2c_clear();

  // INITIALIZATION AND CHOOSING RANDOM COUNTRIES TO ASK USER
  while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));

  srand(HAL_GetTick());
  int guessIndex = rand() % 13;
  int round = 1;
  int previous[2] = {13,13};

  lcd16x2_i2c_printf(COUNTRIES[guessIndex]);

  while (!(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    int timeSincePressed = 0;

        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){

            timeSincePressed = HAL_GetTick() - time2;

            if(considerSpace && (timeSincePressed > 1400)){

                considerSpace = false;

                if(MorseToChar(userInput, MORSE_ALPHABET) != '\0'){

                    cityInput[cityIndex] = MorseToChar(userInput, MORSE_ALPHABET);
                    cityIndex++;

                    lcd16x2_i2c_clear();
                    lcd16x2_i2c_setCursor(0,0);
                    lcd16x2_i2c_printf(COUNTRIES[guessIndex]);
                    lcd16x2_i2c_setCursor(1,0);
                    lcd16x2_i2c_printf(cityInput);
                    
                    if (isEqual(cityInput, CITIES[guessIndex])) {
                      if (round == 3) {
                        lcd16x2_i2c_clear();
                        lcd16x2_i2c_setCursor(0,0);
                        lcd16x2_i2c_printf("Good job");
                        lcd16x2_i2c_setCursor(1,0);
                        lcd16x2_i2c_printf("Europe is safe!");
                      } else {
                        lcd16x2_i2c_clear();
                        lcd16x2_i2c_setCursor(0,0);
                        lcd16x2_i2c_printf("Successful");
                        lcd16x2_i2c_setCursor(1,0);
                        lcd16x2_i2c_printf("Transmission");
                        HAL_Delay(3000);
                        reset(cityInput);
                        reset(userInput);
                        cityIndex = 0;
                        previous[round-1] = guessIndex;
                        while ((guessIndex == previous[0]) || (guessIndex == previous[1])) {
                          guessIndex = rand() % 13;
                        }
                        round++;
                        lcd16x2_i2c_clear();
                        lcd16x2_i2c_setCursor(0,0);
                        lcd16x2_i2c_printf(COUNTRIES[guessIndex]);
                      }
                    }

                } else{
                  lcd16x2_i2c_clear();
                  lcd16x2_i2c_setCursor(0,0);
                  lcd16x2_i2c_printf(COUNTRIES[guessIndex]);
                  lcd16x2_i2c_setCursor(1,0);
                  lcd16x2_i2c_printf(cityInput);
                    
                }

                for (int i = 0; i < 4; i++) {
                    userInput[i] = '\0';
                }
                userCounter = 0;
            }

        } // Waiting for button press
        
        uint32_t time1 = HAL_GetTick(); 

        while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)); // Waiting for button to be released

        time2 = HAL_GetTick();
        uint32_t timePressed = time2 - time1; // Calculating time button is held for

        if(timePressed < 200){
            
            inputChar = '.';
        } else if(timePressed < 3000){
            inputChar = '-';
        } else if(cityIndex != 0){
            inputChar = '\0';
            
            cityIndex--;
            cityInput[cityIndex] = '\0';

            lcd16x2_i2c_clear();
            lcd16x2_i2c_setCursor(0,0);
            lcd16x2_i2c_printf(COUNTRIES[guessIndex]);
            lcd16x2_i2c_setCursor(1,0);
            lcd16x2_i2c_printf(cityInput);
        }

        if((inputChar != '\0') && userCounter < 4){
            userInput[userCounter] = inputChar;
            userCounter++;
            considerSpace = true;
        }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

char MorseToChar(char morse[], char morseAlphabet[][4]) { // Convert Morse input to ASCII character

    for (int i = 0; i < 26; i++) {
        if ((morseAlphabet[i][0] == morse[0]) && (morseAlphabet[i][1] == morse[1]) && (morseAlphabet[i][2] == morse[2]) && (morseAlphabet[i][3] == morse[3])) {
            return i + 65;
        }
    }
    return '\0';
}

bool isEqual(char str1[], char str2[]) {
    int i = 0;

    while (str1[i] != '\0') {
        
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }

    return str2[i] == '\0';
}

void reset(char *str[]) {
  int i = 0;
  while (str[i] != '\0') {
    str[i] = '\0';
    i++;
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
