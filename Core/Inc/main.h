/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
struct __SysInfo {
    bool sta_wifi;
    uint8_t air_temp;
    uint8_t air_humid;
    uint8_t air_light;
    uint8_t soil_humid;
    uint8_t sta_bulb;
    uint8_t sta_beep;
    uint8_t sta_pump;
    uint8_t air_temp_up;
    uint8_t soil_humid_down;
    uint8_t light_down;
    uint8_t sys_mode;
    __IO uint8_t hur;
    __IO uint8_t min;
    __IO uint8_t sec;
};

typedef struct __SysInfo SysInfoType;

// mqtt



/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define GTX_RST_Pin GPIO_PIN_13
#define GTX_RST_GPIO_Port GPIOC
#define ESP01_RST_Pin GPIO_PIN_6
#define ESP01_RST_GPIO_Port GPIOF
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOF
#define LED_RED_Pin GPIO_PIN_9
#define LED_RED_GPIO_Port GPIOF
#define LED_GREEN_Pin GPIO_PIN_10
#define LED_GREEN_GPIO_Port GPIOF
#define ESP01_GPIO0_Pin GPIO_PIN_0
#define ESP01_GPIO0_GPIO_Port GPIOC
#define KEY3_Pin GPIO_PIN_0
#define KEY3_GPIO_Port GPIOA
#define PCF_I2C_SDA_Pin GPIO_PIN_2
#define PCF_I2C_SDA_GPIO_Port GPIOA
#define PCF_I2C_SCL_Pin GPIO_PIN_3
#define PCF_I2C_SCL_GPIO_Port GPIOA
#define GTX_I2C_SCL_Pin GPIO_PIN_0
#define GTX_I2C_SCL_GPIO_Port GPIOB
#define GTX_INT_Pin GPIO_PIN_1
#define GTX_INT_GPIO_Port GPIOB
#define GTX_I2C_SDA_Pin GPIO_PIN_11
#define GTX_I2C_SDA_GPIO_Port GPIOF
#define WQX_CS2_Pin GPIO_PIN_14
#define WQX_CS2_GPIO_Port GPIOB
#define LCD_BLK_Pin GPIO_PIN_15
#define LCD_BLK_GPIO_Port GPIOB
#define PUMP_Pin GPIO_PIN_3
#define PUMP_GPIO_Port GPIOG
#define DS1302_CLK_Pin GPIO_PIN_6
#define DS1302_CLK_GPIO_Port GPIOC
#define DS1302_IO_Pin GPIO_PIN_7
#define DS1302_IO_GPIO_Port GPIOC
#define DS1302_RST_Pin GPIO_PIN_8
#define DS1302_RST_GPIO_Port GPIOC
#define DHT11_DQ_Pin GPIO_PIN_9
#define DHT11_DQ_GPIO_Port GPIOG
#define WQX_CS1_Pin GPIO_PIN_15
#define WQX_CS1_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
