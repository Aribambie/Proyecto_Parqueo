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
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// Buffers I2C
#define TXBUFFERSIZE 4
#define RXBUFFERSIZE 4

//Variables para almacenar la transmisión de datos
uint8_t aTxBuffer[TXBUFFERSIZE];
uint8_t aRxBuffer[RXBUFFERSIZE];

// Estructura para mapear cada segmento con su puerto y pin correspondiente
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} SegmentPin;

// Definición de cada uno de los 7 segmentos del display
const SegmentPin segment_pins[7] = {
    {GPIOC, A_Pin}, {GPIOC, B_Pin}, {GPIOA, C_Pin},
    {GPIOA, D_Pin}, {GPIOA, E_Pin}, {GPIOC, F_Pin},
    {GPIOC, G_Pin}
};

// Función para mostrar número (0-8) en el display de 7 segmentos
void DisplayNumber(uint8_t number) {
    // Apagar todos los segmentos antes de mostrar un nuevo número
    for (int i = 0; i < 7; ++i) {
        HAL_GPIO_WritePin(segment_pins[i].port, segment_pins[i].pin, GPIO_PIN_RESET);
    }

    // Encender pines según el número
    switch (number) {
        case 0:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, D_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, E_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, F_Pin, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, E_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, D_Pin, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, D_Pin, GPIO_PIN_SET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOC, F_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, F_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, D_Pin, GPIO_PIN_SET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, F_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, E_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, D_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            break;
        case 8:
            HAL_GPIO_WritePin(GPIOC, A_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, B_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, C_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, D_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, E_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, F_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, G_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}


// Función CheckLDRs lee el valor de cada fotoresistencia, actualiza LEDs RGB, envía datos por I2C y actualiza display
void CheckLDRs(void) {
    uint8_t valorFotores;
    uint8_t green_count_local = 0;

    valorFotores = HAL_GPIO_ReadPin(GPIOC, fotores1_Pin); //Lectura sensor 1
    if (valorFotores) {
    	//Si no hay luz se enciende el RGB en rojo indicando que esta ocupado
        HAL_GPIO_WritePin(GPIOC, RGB1_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, RGB1_G_Pin, GPIO_PIN_RESET);
    } else { //Se enciende en verde para indicar que esta libre cuando hay luz
        HAL_GPIO_WritePin(GPIOC, RGB1_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, RGB1_G_Pin, GPIO_PIN_SET);
        green_count_local++;
    }

    valorFotores = HAL_GPIO_ReadPin(GPIOC, fotores2_Pin);//Lectura sensor 2
    if (valorFotores) {
    	//Si no hay luz se enciende el RGB en rojo indicando que esta ocupado
        HAL_GPIO_WritePin(GPIOC, RGB2_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, RGB2_G_Pin, GPIO_PIN_RESET);
    } else { //Se enciende en verde para indicar que esta libre cuando hay luz
        HAL_GPIO_WritePin(GPIOC, RGB2_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, RGB2_G_Pin, GPIO_PIN_SET);
        green_count_local++;
    }

    valorFotores = HAL_GPIO_ReadPin(GPIOA, fotores3_Pin);//Lectura sensor 3
    if (valorFotores) {
    	//Si no hay luz se enciende el RGB en rojo indicando que esta ocupado
        HAL_GPIO_WritePin(GPIOC, RGB3_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, RGB3_G_Pin, GPIO_PIN_RESET);
    } else { //Se enciende en verde para indicar que esta libre cuando hay luz
        HAL_GPIO_WritePin(GPIOC, RGB3_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, RGB3_G_Pin, GPIO_PIN_SET);
        green_count_local++;
    }

    valorFotores = HAL_GPIO_ReadPin(GPIOA, fotores4_Pin);//Lectura sensor 4
    if (valorFotores) {
    	//Si no hay luz se enciende el RGB en rojo indicando que esta ocupado
        HAL_GPIO_WritePin(GPIOC, RGB4_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, RGB4_G_Pin, GPIO_PIN_RESET);
    } else { //Se enciende en verde para indicar que esta libre cuando hay luz
        HAL_GPIO_WritePin(GPIOC, RGB4_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, RGB4_G_Pin, GPIO_PIN_SET);
        green_count_local++;
    }

    //Escribe en el buffer la cantidad de luces verdes (parqueos libres encendidos)
    aTxBuffer[0] = green_count_local;

    //Obtiene del ESP32 los otros 4 parqueos
    uint8_t green_count_remote = aRxBuffer[0];
    if (green_count_remote > 4) green_count_remote = 4;

    //Suma la cantidad de parqueos disponibles
    uint8_t total_green = green_count_local + green_count_remote;
    if (total_green > 8) total_green = 8;

    //Enciende en el 7 segmentos la cantidad de leds encendidos
    DisplayNumber(total_green);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  //Condición para escuchar al maestro
  if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK) {
  	  Error_Handler();
    }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //Llamar función para encender leds
	  CheckLDRs();
	  HAL_Delay(100);
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
  HAL_GPIO_WritePin(GPIOC, RGB4_R_Pin|RGB1_G_Pin|RGB2_G_Pin|F_Pin
                          |G_Pin|B_Pin|A_Pin|RGB1_R_Pin
                          |RGB2_R_Pin|RGB3_R_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RGB3_G_Pin|LD2_Pin|E_Pin|C_Pin
                          |D_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RGB4_G_GPIO_Port, RGB4_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RGB4_R_Pin RGB1_G_Pin RGB2_G_Pin F_Pin
                           G_Pin B_Pin A_Pin RGB1_R_Pin
                           RGB2_R_Pin RGB3_R_Pin */
  GPIO_InitStruct.Pin = RGB4_R_Pin|RGB1_G_Pin|RGB2_G_Pin|F_Pin
                          |G_Pin|B_Pin|A_Pin|RGB1_R_Pin
                          |RGB2_R_Pin|RGB3_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : fotores1_Pin fotores2_Pin */
  GPIO_InitStruct.Pin = fotores1_Pin|fotores2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : fotores3_Pin fotores4_Pin */
  GPIO_InitStruct.Pin = fotores3_Pin|fotores4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RGB3_G_Pin LD2_Pin E_Pin C_Pin
                           D_Pin */
  GPIO_InitStruct.Pin = RGB3_G_Pin|LD2_Pin|E_Pin|C_Pin
                          |D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : RGB4_G_Pin */
  GPIO_InitStruct.Pin = RGB4_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RGB4_G_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// I2C escucha continua
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {
    HAL_I2C_EnableListen_IT(hi2c);
}

// Envia valor de estado LED por I2C
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle) {
    GPIO_PinState led_state = HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);
    aTxBuffer[0] = (led_state == GPIO_PIN_SET) ? 1 : 0;
}

// Recibe comando I2C y actúa si recibe 'S'
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHnadle) {
    HAL_UART_Transmit(&huart2, aRxBuffer, 4, 1000);
    if (aRxBuffer[0] == 'S') {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}

// Lógica secuencial según dirección y operación I2C
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
        if (HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, (uint8_t*) aRxBuffer, RXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
            Error_Handler();
        }
    } else if (TransferDirection == I2C_DIRECTION_RECEIVE) {
        if (HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, (uint8_t*) aTxBuffer, 4, I2C_FIRST_AND_LAST_FRAME) != HAL_OK) {
            Error_Handler();
        }
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
