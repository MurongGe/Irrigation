//
// Created by Zhou 2024/1/7
//

#ifndef __LCD_FSMC_IO_H
#define __LCD_FSMC_IO_H

#include "main.h"

#define lcd_delay(x) HAL_Delay(x)

#define FSMC_LCD_REG (uint32_t)0x6C000000
#define FSMC_LCD_RAM (uint32_t)0x6C000080

#define LCD_BLK_LOW 	HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET)
#define LCD_BLK_HIGH 	HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET)

void LCD_WR_DATA(__IO uint16_t dat);
void LCD_WR_REG(__IO uint16_t cmd);
__IO uint16_t LCD_RD_DATA(void);
void LCD_WR_DATAS(uint8_t *pBuff, uint16_t size);

#endif //__LCD_FSMC_IO_H
