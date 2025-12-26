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
// 先在cubemx中配置OLED和串口还有按键的引脚和参数
// 然后添加OLED相关文件
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "uart_hex.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEY_PRESSED 0  // 假设按键按下为低电平
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
UartReceiveManager_t uartManager;
HexPacket_t rxPacket, txPacket;
uint8_t dataBuffer[4] = {0x01, 0x02, 0x03, 0x04};  // 四个数据
uint8_t keyState = 0;          // 按键状态
uint8_t lastKeyState = 0;      // 上一次按键状态
uint32_t keyDebounceTime = 0;  // 按键防抖时间
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Error_Handler(void);
void UpdateOLEDDisplay(void);
void CheckKeyPress(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief UART接收完成回调函数
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        uint8_t rxByte = huart->pRxBuffPtr[0];
        uint8_t packetReceived = 0;

        // 处理接收到的字节
        packetReceived = UART_ProcessReceivedByte(&uartManager, rxByte, &rxPacket);

        if (packetReceived) {
            // 接收到完整数据包，更新OLED显示
            UpdateOLEDDisplay();
        }

        // 重新启动接收中断
        uint8_t newRxByte;
        HAL_UART_Receive_IT(&huart1, &newRxByte, 1);
    }
}

/**
  * @brief 更新OLED显示内容
  */
void UpdateOLEDDisplay(void) {
    char str[32];
    
    // 显示发送数据
    sprintf(str, "%02X %02X %02X %02X", 
            dataBuffer[0], dataBuffer[1], 
            dataBuffer[2], dataBuffer[3]);
    OLED_ShowString(2, 1, str);
    
    // 显示接收数据
    if (rxPacket.length > 0) 
	{
        // 最多显示4个字节
        uint8_t showLen = rxPacket.length > 4 ? 4 : rxPacket.length;
        memset(str, 0, sizeof(str));
        
        for (uint8_t i = 0; i < showLen; i++) 
		{
            sprintf(str + i * 3, "%02X ", rxPacket.data[i]);
        }
        OLED_ShowString(4, 1, str);
    } 
	else 
	{
	
        OLED_ShowString(4, 1, "No data");
    }
}

/**
  * @brief 检查按键状态并处理
  */
void CheckKeyPress(void) {
    uint8_t currentState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
    
    // 按键防抖处理
    if (currentState != lastKeyState) {
        keyDebounceTime = HAL_GetTick();
    }
    
    // 稳定状态且与上次状态不同
    if (HAL_GetTick() - keyDebounceTime > 50) {  // 50ms防抖
        if (currentState == KEY_PRESSED && currentState != keyState) {
            // 按键按下，四个数据自增
            for (uint8_t i = 0; i < 4; i++) {
                dataBuffer[i]++;
            }
            
            // 更新发送数据包
            txPacket.length = 4;
            memcpy(txPacket.data, dataBuffer, 4);
            
            // 发送更新后的数据
            UART_SendHexPacket(&huart1, &txPacket);
            
            // 更新显示
            UpdateOLEDDisplay();
        }
        keyState = currentState;
    }
    
    lastKeyState = currentState;
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
  // 模块初始化
  OLED_Init();        // OLED初始化
  OLED_Clear();       // OLED清屏
  
  // OLED显示静态字符
  OLED_ShowString(1, 1, "TxPacket:");
  OLED_ShowString(3, 1, "RxPacket:");
  
  // 初始化USART1接收管理器
  UART_InitReceiveManager(&uartManager);

  // 初始化发送数据包
  txPacket.length = 4;
  memcpy(txPacket.data, dataBuffer, 4);
  
  // 启动USART1接收中断（一次接收1个字节）
  uint8_t rxByte;
  if (HAL_UART_Receive_IT(&huart1, &rxByte, 1) != HAL_OK) 
  {
      Error_Handler();
  }
  
  // 初始显示
  UpdateOLEDDisplay();
  
  // 发送初始数据
  UART_SendHexPacket(&huart1, &txPacket);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* 检查按键状态 */
    CheckKeyPress();
    
    /* 短暂延时，降低CPU占用 */
    HAL_Delay(10);
    
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
  // 错误状态下，OLED显示错误信息
  OLED_Clear();
  OLED_ShowString(2, 1, "Error Occurred");
  
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
