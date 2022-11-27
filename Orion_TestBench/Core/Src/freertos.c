/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "ADS1113.h"
#include "BMP280.hpp"
#include <stdbool.h>
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
/* USER CODE BEGIN Variables */
ads1113_t voltmeter;
/* USER CODE END Variables */
/* Definitions for blink01 */
osThreadId_t blink01Handle;
const osThreadAttr_t blink01_attributes = {
  .name = "blink01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for adsTask */
osThreadId_t adsTaskHandle;
const osThreadAttr_t adsTask_attributes = {
  .name = "adsTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for gpioTask */
osThreadId_t gpioTaskHandle;
const osThreadAttr_t gpioTask_attributes = {
  .name = "gpioTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBlink01(void *argument);
void StartAdsTask(void *argument);
void StartGPIOTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of blink01 */
  blink01Handle = osThreadNew(StartBlink01, NULL, &blink01_attributes);

  /* creation of adsTask */
  adsTaskHandle = osThreadNew(StartAdsTask, NULL, &adsTask_attributes);

  /* creation of gpioTask */
  gpioTaskHandle = osThreadNew(StartGPIOTask, NULL, &gpioTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartBlink01 */
/**
  * @brief  Function implementing the blink01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBlink01 */
void StartBlink01(void *argument)
{
  /* USER CODE BEGIN StartBlink01 */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartBlink01 */
}

/* USER CODE BEGIN Header_StartAdsTask */
/**
* @brief Function implementing the adsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAdsTask */
void StartAdsTask(void *argument)
{
  /* USER CODE BEGIN StartAdsTask */
	bool success = ADS1113_init(&voltmeter, &hi2c1, ADS_ADDR_GND);
	ADSsetGain(&voltmeter, GAIN_ONE);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
    if (success) {
		int16_t val = ADSreadADC_Differential_0_1(&voltmeter);
		printf("Differential value %d \n", val);
	}
  }
  /* USER CODE END StartAdsTask */
}

/* USER CODE BEGIN Header_StartGPIOTask */
/**
* @brief Function implementing the gpioTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGPIOTask */
void StartGPIOTask(void *argument)
{
  /* USER CODE BEGIN StartGPIOTask */
  BMP280 bmp(&hspi1);
  bmp.initialize();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartGPIOTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//void StartBlink02(void *argument)
//{
//  /* USER CODE BEGIN StartBlink02 */
//  bool success = ADS1113_init(&voltmeter, &hi2c1, ADS_ADDR_GND);
//  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, ADS_ADDR_GND<<1, 10, 100);
//  ADSsetGain(&voltmeter, GAIN_ONE);
//  /* Infinite loop */
//  for(;;)
//  {
//	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
//    osDelay(600);
//    status = HAL_I2C_IsDeviceReady(&hi2c1, ADS_ADDR_GND<<1, 10, 100);
////    printf("Differential value %d \n", ADSreadADC_Differential_0_1(&hi2c1));
//    if (success) {
//    	status = HAL_I2C_IsDeviceReady(&hi2c1, ADS_ADDR_GND<<1, 10, 100);
//		int16_t val = ADSreadADC_Differential_0_1(&voltmeter);
//		printf("Differential value %d \n", val);
//    }
////    printf("Single ended value %d \n", ADSreadADC_SingleEnded(&hi2c1));
//  }
//  /* USER CODE END StartBlink02 */
//}
/* USER CODE END Application */

