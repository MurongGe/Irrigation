//
// Created by Zhou 2024/1/7
//

#include "lcd_fsmc_io.h"
#include <stdio.h>

/* LCD Write byte DATA */
__attribute__((optimize("O0")))  void LCD_WR_DATAS(uint8_t *pBuff, uint16_t size) {
  uint16_t i = 0;
  for (i = 0; i < size ; i++) {
    LCD_WR_DATA(*(__IO uint16_t *) pBuff);
    pBuff += 2;
  }
}

/* LCD Write byte DATA */
__attribute__((optimize("O0")))  void LCD_WR_DATA(__IO uint16_t dat) {
  dat = dat;
  *(__IO uint16_t *) FSMC_LCD_RAM = dat;
}

/* LCD Write CMD byte */
__attribute__((optimize("O0"))) void LCD_WR_REG(__IO uint16_t cmd) {
  cmd = cmd;
  *(__IO uint16_t *) FSMC_LCD_REG = cmd;
}

/* LCD Read CMD byte */
__attribute__((optimize("O0")))  __IO uint16_t LCD_RD_DATA(void) {
  return *(__IO uint16_t *) FSMC_LCD_RAM;
}
