/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#define digit1 GPIO_PIN_7
#define digit2 GPIO_PIN_8
#define digit3 GPIO_PIN_9
#define digit4 GPIO_PIN_10
int LEDS[10]={0xC0,     /* 0 */
				0xF9,   /* 1 */
				0xA4, 	/* 2 */
				0xB0,	/* 3 */
				0x99, 	/* 4 */
				0x92, 	/* 5 */
				0x82, 	/* 6 */
				0xF8,	/* 7 */
				0x80, 	/* 8 */
				0x90};	/* 9 */
int MSD,MID2,MID1,LSD;
int m,n;
int Count = 0;
int flag = 0;

TIM_HandleTypeDef htim2;   //Auto Generate เพื่อเปิดระบบ Timer2 Interrupt

UART_HandleTypeDef huart2; //Auto Generate เพื่อเปิดระบบการใช้ UART

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //ฟังก์ชัน Timer2 Interrupt
{
	MSD = Count / 1000;

	m = Count % 1000;
	MID2 = m / 100;

	n = m % 100;
	MID1 = n/10;

	LSD = n % 10;
	if(flag ==0)
	{
		if(Count >999)
		{
		GPIOC->ODR = LEDS[MSD];
	    HAL_GPIO_WritePin(GPIOC, digit1, 0);
	    HAL_GPIO_WritePin(GPIOC, digit4, 1);
		HAL_GPIO_WritePin(GPIOC, digit3, 1);
		HAL_GPIO_WritePin(GPIOC, digit2, 1);
		}

	    flag++;
	}
	else if(flag == 1)
	{
		if(Count > 99)
		{
		GPIOC->ODR = LEDS[MID2];
		HAL_GPIO_WritePin(GPIOC, digit2, 0);
		HAL_GPIO_WritePin(GPIOC, digit1, 1);
		HAL_GPIO_WritePin(GPIOC, digit4, 1);
		HAL_GPIO_WritePin(GPIOC, digit3, 1);
		}

		flag++;
	}
	else if(flag == 2)
	{
		if(Count >9)
		{
		GPIOC->ODR = LEDS[MID1];
		HAL_GPIO_WritePin(GPIOC, digit3, 0);
		HAL_GPIO_WritePin(GPIOC, digit2, 1);
		HAL_GPIO_WritePin(GPIOC, digit4, 1);
		HAL_GPIO_WritePin(GPIOC, digit1, 1);
		}

		flag++;
	}
	else if(flag == 3)
	{

		GPIOC->ODR = LEDS[LSD];
		HAL_GPIO_WritePin(GPIOC, digit4, 0);
		HAL_GPIO_WritePin(GPIOC, digit1, 1);
		HAL_GPIO_WritePin(GPIOC, digit2, 1);
		HAL_GPIO_WritePin(GPIOC, digit3, 1);
		flag = 0;
	}
}

void UART_SEND_TXT(UART_HandleTypeDef *huart,char buffer[],int m)
{
	HAL_UART_Transmit(huart, (uint8_t*) buffer,strlen(buffer), HAL_MAX_DELAY);
	if(m == 1)
		HAL_UART_Transmit(huart, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
}
void UART_SEND_CHR(UART_HandleTypeDef *huart,char c,int m)
{
	HAL_UART_Transmit(huart, (uint8_t*)&c, 1, HAL_MAX_DELAY);
	if(m == 1)
		HAL_UART_Transmit(huart, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
}
void UART_SEND_NL(UART_HandleTypeDef *huart)
{
	HAL_UART_Transmit(huart, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
}
void UART_SEND_INT(UART_HandleTypeDef *huart,int i,int m)
{
	char buffer[10];
	itoa(i,buffer,10);
	HAL_UART_Transmit(huart, (uint8_t*) buffer, strlen(buffer),HAL_MAX_DELAY);
	if(m == 1)
		HAL_UART_Transmit(huart, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
}

int main(void)
{
  char readbuf[1];
  int N,n;

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);  //เขียนเองเพื่อเปิดระบบ Timer2 Interrupt
  /* USER CODE END 2 */

  while (1)
  {
    /* USER CODE END WHILE */
	  	  N = 0;
	 	  UART_SEND_NL(&huart2);
	 	  UART_SEND_TXT(&huart2, "Number To Show", 1);
	 	  UART_SEND_TXT(&huart2, "------------", 1);
	 	  UART_SEND_NL(&huart2);
	 	  UART_SEND_TXT(&huart2, "Enter Number:", 0);
	 	  while(1)
	 	  {
	 		  HAL_UART_Receive(&huart2, (uint8_t*)readbuf, 1, HAL_MAX_DELAY);
	 		  UART_SEND_CHR(&huart2, readbuf[0], 0);
	 		  if(readbuf[0] == '\r') //ระวังผิดนะต้องใช้ฟันเดี่ยว
	 			  break;
	 		  n = atoi(readbuf);
	 		  N = 10*N+n; //ที่ต้อ งมี 10 * n เพราะไม่งั้นกรณีตัวเลขมากว่า 1 ตำแหน่งจะผิด
	 	  }
	     Count = N;

	 /* Count++;
	  if(Count>9999)
	  	Count = 0;
	  HAL_Delay(1000);*/
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 39999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim2.Init.Period = 2;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 
                           PC4 PC5 PC6 PC7 
                           PC8 PC9 PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
