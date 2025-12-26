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
#include "dma.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
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

/* USER CODE BEGIN PV */
uint8_t DataA[] = {0x01,0x02,0x03,0x04};//定义测试数组DataA，为数据源
uint8_t DataB[] = {0, 0, 0, 0};		  //定义测试数组DataB，为数据目的地
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */
  // 现在cubemx中配置OLED相关引脚和开启DMA1 CHANNEL1
  // 按照之前的程序中的步骤添加和配置OLED相关文件
  // 模块初始化
  OLED_Init();      // OLED初始化
  OLED_Clear();
  OLED_ShowString(1,1,"DataA:");// 显示静态名称
  OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);// 显示数组的首地址
  OLED_ShowString(3,1,"DataB:");// 显示静态名称 
  OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);// 显示数组的首地址
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  DataA[0] ++;		//变换测试数据
	  DataA[1] ++;
	  DataA[2] ++;
	  DataA[3] ++;
	  OLED_ShowHexNum(2, 1, DataA[0], 2);		//显示数组DataA
	  OLED_ShowHexNum(2, 4, DataA[1], 2);
	  OLED_ShowHexNum(2, 7, DataA[2], 2);
	  OLED_ShowHexNum(2, 10, DataA[3], 2);
	  OLED_ShowHexNum(4, 1, DataB[0], 2);		//显示数组DataB
	  OLED_ShowHexNum(4, 4, DataB[1], 2);
	  OLED_ShowHexNum(4, 7, DataB[2], 2);
	  OLED_ShowHexNum(4, 10, DataB[3], 2);
	  HAL_Delay(1000); //延时1s，观察转运前的现象
	  
	  // 启动DMA
	  HAL_DMA_Start(&hdma_memtomem_dma1_channel1,(uint32_t)DataA,(uint32_t)DataB,4);
	  // 等待传输完成
	  HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1,HAL_DMA_FULL_TRANSFER,HAL_MAX_DELAY);
	  
	  OLED_ShowHexNum(2, 1, DataA[0], 2);		//显示数组DataA
	  OLED_ShowHexNum(2, 4, DataA[1], 2);
	  OLED_ShowHexNum(2, 7, DataA[2], 2);
	  OLED_ShowHexNum(2, 10, DataA[3], 2);
	  OLED_ShowHexNum(4, 1, DataB[0], 2);		//显示数组DataB
	  OLED_ShowHexNum(4, 4, DataB[1], 2);
      OLED_ShowHexNum(4, 7, DataB[2], 2);
	  OLED_ShowHexNum(4, 10, DataB[3], 2);

	  HAL_Delay(1000);//延时1s，观察转运后的现象	
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
