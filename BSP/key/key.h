//
// Created by Zhou 2024/1/8
//

#ifndef __KEY_H_
#define __KEY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define KEY_NUM 4

extern volatile uint8_t key_table[KEY_NUM];

#define READ_KEY0     HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)
#define READ_KEY1     HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define READ_KEY2     HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)
#define READ_KEY3     HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)



void update_key(void);

#ifdef __cplusplus
}
#endif

#endif













