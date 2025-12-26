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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "uart_text.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_PORT GPIOA
#define LED_PIN GPIO_PIN_3     // LED接在PA3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
UartTextManager_t uartManager;       // 串口文本管理器
char rxBuffer[UART_MAX_TEXT_LEN];    // 接收缓冲区
char txBuffer[UART_MAX_TEXT_LEN];    // 发送缓冲区
uint8_t ledState = 0;                // LED状态标记
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Error_Handler(void);
void UpdateOLEDDisplay(void);
void ProcessReceivedData(const char* data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief UART接收完成回调函数
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        static uint8_t rxByte;  // 静态变量存储接收字节
        uint8_t packetReceived = 0;

        // 读取接收的字节（从缓冲区获取）
        rxByte = *huart->pRxBuffPtr;
        
        // 处理接收到的字节
        packetReceived = UART_ProcessTextByte(&uartManager, rxByte, rxBuffer);

        if (packetReceived) {
            // 1. 处理接收的数据（控制LED）
            ProcessReceivedData(rxBuffer);
            
            // 2. 回发接收到的数据
            strncpy(txBuffer, rxBuffer, UART_MAX_TEXT_LEN - 1);
            txBuffer[UART_MAX_TEXT_LEN - 1] = '\0';  // 确保字符串结束
            UART_SendTextPacket(&huart1, txBuffer);
            
            // 3. 更新OLED显示
            UpdateOLEDDisplay();
        }

        // 重新启动接收中断（关键：确保持续接收）
        HAL_UART_Receive_IT(&huart1, &rxByte, 1);
    }
}

/**
  * @brief 处理接收到的数据，控制LED
  * @param data: 接收到的字符串
  */
void ProcessReceivedData(const char* data) {
    if (data == NULL) return;
    
    // 严格比较字符串（忽略大小写问题）
    if (strcmp(data, "LED_YES") == 0) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);  // LED亮
        ledState = 1;
    } else if (strcmp(data, "LED_NO") == 0) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);  // LED灭
        ledState = 0;
    }
}

/**
  * @brief 更新OLED显示内容
  */
void UpdateOLEDDisplay(void) {
    // 清除原有显示（防止字符残留）
    OLED_ShowString(2, 1, "                ");  // 清除第2行
    OLED_ShowString(4, 1, "                ");  // 清除第4行
    
    // 显示发送内容
    OLED_ShowString(1, 1, "Tx:");
    OLED_ShowString(2, 1, txBuffer);
    
    // 显示接收内容
    OLED_ShowString(3, 1, "Rx:");
    OLED_ShowString(4, 1, rxBuffer);
}

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  // 初始化OLED
  OLED_Init();
  OLED_Clear();
  
  // 初始化LED（确保初始状态为灭）
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
  
  // 初始化串口文本管理器
  UART_InitTextManager(&uartManager);
  
  // 初始显示
  OLED_ShowString(1, 1, "TxPacket:");
  OLED_ShowString(3, 1, "RxPacket:");
  OLED_ShowString(2, 1, "Ready...");
  
  // 启动串口接收中断（关键步骤）
  static uint8_t rxByte;
  if (HAL_UART_Receive_IT(&huart1, &rxByte, 1) != HAL_OK) {
      OLED_ShowString(4, 1, "UART Init Error");
      Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  while(1)
  {
    // 错误时可以让LED闪烁提示
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    HAL_Delay(500);
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
