/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "Bitmaps.h"

#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DeviceAddress 0x66
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */


  char msg[50];
  uint8_t valorLDR5 = 0;
  uint8_t valorLDR6 = 0;
  uint8_t valorLDR7 = 0;
  uint8_t valorLDR8 = 0;


  //Variables útiles para comunicación I2C
#define TXBUFFERSIZE 4
#define RXBUFFERSIZE 4

int count = 0;

uint8_t aTxBuffer[TXBUFFERSIZE];
/*
 * Si alguno de los bytes en el buffer aTXBuffer se encuentra en 1 es porque ese espacio está ocupado
 *
 * */

uint8_t aRxBuffer[RXBUFFERSIZE];

uint8_t Park5_available = 0;
uint8_t Park6_available = 0;
uint8_t Park7_available = 0;
uint8_t Park8_available = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  LCD_Init();

  if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK){
	  Error_Handler();
  }
  //HAL_ADC_Start(&hadc1);

  //********************* Iniciliazación del fondo **********************************************
  LCD_Clear(0x00);
  FillRect(0, 0, 319, 239, 0x4a49);
  FillRect(80, 0,3,75,0xff20 );
  FillRect(160, 0,3,75,0xff20 );
  FillRect(240, 0,3,75,0xff20 );
  FillRect(80, 154,3,75,0xff20 );
  FillRect(160, 154,3,75,0xff20 );
  FillRect(240, 154,3,75,0xff20 );

  //LCD_Sprite(x, y, width, height, bitmap, columns, index, flip, offset)
  //LCD_Sprite(0, 0, 70, 89, Car_red_prueba, 1, 0, 1, 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	      //HAL_ADC_Start(&hadc1);
	 	  //HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	 	  //valorADC = HAL_ADC_GetValue(&hadc1);

	      valorLDR5 = HAL_GPIO_ReadPin(GPIOC, LDR_1_Pin);
	 	  if (valorLDR5)
	 	      {
	 	        HAL_GPIO_WritePin(GPIOC, LED5_G_Pin, GPIO_PIN_RESET);
	 	        HAL_GPIO_WritePin(GPIOC, LED5_R_Pin, GPIO_PIN_SET);
	 	       LCD_Sprite(0, 0, 70, 89, Car_red_prueba, 1, 0, 1, 0);
	 	       Park5_available = 1;
	 	      }
	 	  else
	 	      {
	 	        HAL_GPIO_WritePin(GPIOC,  LED5_G_Pin, GPIO_PIN_SET);
	 	        HAL_GPIO_WritePin(GPIOC,  LED5_R_Pin, GPIO_PIN_RESET);
	 	       //green_count_local++;
	 	       Park5_available = 0;
	 	        FillRect(0, 0, 70, 89, 0x4a49);
	 	      }

	 	  valorLDR6 = HAL_GPIO_ReadPin(GPIOC, LDR_2_Pin);
	 	 if (valorLDR6)
	 	      {
	 	 	 	  HAL_GPIO_WritePin(GPIOA, LED6_G_Pin, GPIO_PIN_RESET);
	 	 	 	  HAL_GPIO_WritePin(GPIOA, LED6_R_Pin, GPIO_PIN_SET);
	 	 	 	  LCD_Sprite(83, 0, 70, 89, Car_red_prueba, 1, 0, 1, 0);
	 	 	 	  Park6_available = 1;
	 	 	   }
	 	 	 else
	 	 	   {
	 	 	       HAL_GPIO_WritePin(GPIOA,  LED6_G_Pin, GPIO_PIN_SET);
	 	 	 	   HAL_GPIO_WritePin(GPIOA,  LED6_R_Pin, GPIO_PIN_RESET);
	 	 	 	 //green_count_local++;
	 	 	 	   FillRect(83, 0, 70, 89, 0x4a49);
	 	 	 	   Park6_available = 0;
	 	 	 	}


	 	valorLDR7 = HAL_GPIO_ReadPin(GPIOC, LDR_3_Pin);
	 		 	 if (valorLDR7)
	 		 	      {
	 		 	 	 	  HAL_GPIO_WritePin(GPIOB, LED7_G_Pin, GPIO_PIN_RESET);
	 		 	 	 	  HAL_GPIO_WritePin(GPIOB, LED7_R_Pin, GPIO_PIN_SET);
	 		 	 	 	  LCD_Sprite(163, 0, 70, 89, Car_red_prueba, 1, 0, 1, 0);
	 		 	 	 	Park7_available = 1;
	 		 	 	   }
	 		 	 	 else
	 		 	 	   {
	 		 	 	       HAL_GPIO_WritePin(GPIOB,  LED7_G_Pin, GPIO_PIN_SET);
	 		 	 	 	   HAL_GPIO_WritePin(GPIOB,  LED7_R_Pin, GPIO_PIN_RESET);
	 		 	 	 	// green_count_local++;
	 		 	 	 	   FillRect(163, 0, 70, 89, 0x4a49);
	 		 	 	 	Park7_available = 0;
	 		 	 	 	}


	 		 	valorLDR8 = HAL_GPIO_ReadPin(GPIOC, LDR_4_Pin);
	 		 	if (valorLDR8)
	 		 		 		 	      {
	 		 		 		 	 	 	  HAL_GPIO_WritePin(GPIOB, LED8_G_Pin, GPIO_PIN_RESET);
	 		 		 		 	 	 	  HAL_GPIO_WritePin(GPIOB, LED8_R_Pin, GPIO_PIN_SET);
	 		 		 		 	 	 	  LCD_Sprite(243, 0, 70, 89, Car_red_prueba, 1, 0, 1, 0);
	 		 		 		 	 	 Park8_available = 1;
	 		 		 		 	 	   }
	 		 		 		 	 	 else
	 		 		 		 	 	   {
	 		 		 		 	 	       HAL_GPIO_WritePin(GPIOB,  LED8_G_Pin, GPIO_PIN_SET);
	 		 		 		 	 	 	   HAL_GPIO_WritePin(GPIOB,  LED8_R_Pin, GPIO_PIN_RESET);
	 		 		 		 	 	//  green_count_local++;
	 		 		 		 	 	 	   FillRect(243, 0, 70, 89, 0x4a49);
	 		 		 		 	 	 	Park8_available = 0;
	 		 		 		 	 	 	}


	 	      //HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	 	    /* aTxBuffer[0] = green_count_local;

	 	        uint8_t green_count_remote = aRxBuffer[0];
	 	        if (green_count_remote > 4) green_count_remote = 4;

	 	        uint8_t total_green = green_count_local + green_count_remote;
	 	        if (total_green > 8) total_green = 8;
	 	      HAL_Delay(100);*/



    /* USER CODE END WHILE */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  hi2c1.Init.OwnAddress1 = 170;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LED5_R_Pin|LCD_D1_Pin|LED5_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin|LED6_R_Pin|LED6_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LED8_G_Pin|LED7_R_Pin|LCD_D6_Pin
                          |LED7_G_Pin|LD2_Pin|LED8_R_Pin|LCD_D3_Pin
                          |LCD_D5_Pin|LCD_D4_Pin|SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LED5_R_Pin LCD_D1_Pin LED5_G_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LED5_R_Pin|LCD_D1_Pin|LED5_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LDR_1_Pin LDR_2_Pin LDR_3_Pin LDR_4_Pin */
  GPIO_InitStruct.Pin = LDR_1_Pin|LDR_2_Pin|LDR_3_Pin|LDR_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin LCD_D7_Pin
                           LCD_D0_Pin LCD_D2_Pin LED6_R_Pin LED6_G_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin|LED6_R_Pin|LED6_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LED8_G_Pin LED7_R_Pin LCD_D6_Pin
                           LED7_G_Pin LD2_Pin LED8_R_Pin LCD_D3_Pin
                           LCD_D5_Pin LCD_D4_Pin SD_SS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LED8_G_Pin|LED7_R_Pin|LCD_D6_Pin
                          |LED7_G_Pin|LD2_Pin|LED8_R_Pin|LCD_D3_Pin
                          |LCD_D5_Pin|LCD_D4_Pin|SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
	HAL_I2C_EnableListen_IT(hi2c);
}


void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle) {
	aTxBuffer[0] = Park5_available;
	aTxBuffer[1] = Park6_available;
	aTxBuffer[2] = Park7_available;
	aTxBuffer[3] = Park8_available;

}


void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHnadle) {
	HAL_UART_Transmit(&huart2, aRxBuffer, 4, 1000);
	if(aRxBuffer[0] == 'S'){
		//HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET);
		HAL_Delay(900);
	}else{//HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_RESET);
		}
}


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	if (TransferDirection == I2C_DIRECTION_TRANSMIT){
		if (HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, (uint8_t*) aRxBuffer,
				RXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) !=HAL_OK) {
			Error_Handler();
		}
	} else if (TransferDirection == I2C_DIRECTION_RECEIVE) {
		if (HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, (uint8_t*) aTxBuffer, 4, I2C_FIRST_AND_LAST_FRAME) != HAL_OK){
			Error_Handler();
		}
	}


}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle){
	HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET);
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
