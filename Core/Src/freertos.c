/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "sensor.h"
#include "bme280.h"
#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
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
uint16_t timer_val = 0;
HAL_StatusTypeDef rslt;
strFdbkSensor TempHumPresSensor = {.humidity = 0, .pressure = 0, .temperature = 0, .rslt = BME280_OK};
char myText[50] = {'\0'};

/* USER CODE END Variables */
/* Definitions for ReadSensorData */
osThreadId_t ReadSensorDataHandle;
const osThreadAttr_t ReadSensorData_attributes = {
    .name = "ReadSensorData",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for DisplayData */
osThreadId_t DisplayDataHandle;
const osThreadAttr_t DisplayData_attributes = {
    .name = "DisplayData",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityBelowNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartReadData(void *argument);
void StartDisplayData(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  rslt = InitSensors();
  rslt = InitDisplay();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  HAL_TIM_Base_Start(&htim17);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ReadSensorData */
  ReadSensorDataHandle = osThreadNew(StartReadData, NULL, &ReadSensorData_attributes);

  /* creation of DisplayData */
  DisplayDataHandle = osThreadNew(StartDisplayData, NULL, &DisplayData_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartReadData */
/**
 * @brief  Function implementing the ReadSensorData thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadData */
void StartReadData(void *argument)
{
  /* USER CODE BEGIN StartReadData */
  /* Infinite loop */

  for (;;)
  {
    TempHumPresSensor = ReadSensor();
    ReadAdcValue();
    osDelay(100);
  }
  /* USER CODE END StartReadData */
}

/* USER CODE BEGIN Header_StartDisplayData */
/**
 * @brief Function implementing the DisplayData thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDisplayData */
void StartDisplayData(void *argument)
{
  /* USER CODE BEGIN StartDisplayData */
  /* Infinite loop */
  for (;;)
  {
    strcpy(myText, "Hello World from Task 2 \n \r");
    ADC_enuSensorType SensorType = Temp;
    uint16_t SensorValue = ADC_GetSensorValues(SensorType);
    UART_SendSensorData("Temperature", SensorValue, "C");
    SensorType = UExt;
    SensorValue = ADC_GetSensorValues(SensorType);
    UART_SendSensorData("External Voltage", SensorValue, "mV");
    SensorType = URef;
    SensorValue = ADC_GetSensorValues(SensorType);
    UART_SendSensorData("Reference Voltage", SensorValue, "mV");
    SensorType = UBatt;
    SensorValue = ADC_GetSensorValues(SensorType);
    UART_SendSensorData("Battery Voltage", SensorValue, "mV");
    DisplaySensorData(TempHumPresSensor);
    osDelay(1200);
  }
  /* USER CODE END StartDisplayData */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
