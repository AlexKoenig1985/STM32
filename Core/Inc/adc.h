/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.h
 * @brief   This file contains all the function prototypes for
 *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */

  extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#ifndef TRUE
#define TRUE UINT8_C(1)
#endif
#ifndef FALSE
#define FALSE UINT8_C(0)
#endif
  typedef enum
  {
    Temp = 0,
    UExt = 1,
    URef = 2,
    UBatt = 3
  } ADC_enuSensorType;
  /* USER CODE END Private defines */

  void MX_ADC1_Init(void);

  /* USER CODE BEGIN Prototypes */
  void ADC_ReadValue(void);
  uint16_t ADC_GetSensorValues(ADC_enuSensorType SensorType);
  /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
