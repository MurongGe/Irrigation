//
// Created by ShunX on 2024/4/19.
//

#include "ws2812.h"
#include "spi.h"
#include "stdio.h"
#include "sys.h"



#define	WS2812_0	0xC0
#define	WS2812_1	0xF0
#define	WS2812_RST	0x00


uint8_t RGB[24] = {0};


void ws2812_oneled(uint8_t R, uint8_t G, uint8_t B) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    (G & (0x80 >> i)) ? (RGB[i] = WS2812_1) : (RGB[i] = WS2812_0);
  }
  for (i = 0; i < 8; i++) {
    (R & (0x80 >> i)) ? (RGB[i + 8] = WS2812_1) : (RGB[i + 8] = WS2812_0);
  }
  for (i = 0; i < 8; i++) {
    (B & (0x80 >> i)) ? (RGB[i + 16] = WS2812_1) : (RGB[i + 16] = WS2812_0);
  }
  HAL_SPI_Transmit_DMA(&hspi1, RGB, 24);
}

void ws2812_led(uint8_t val)
{
  ws2812_oneled(val, val, val);
}