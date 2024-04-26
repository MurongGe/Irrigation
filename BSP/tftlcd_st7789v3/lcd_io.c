//
// Created by Zhou 2024/1/7
//

#include "lcd_io.h"
#include <stdio.h>

#define DMA_TIMEOUT 0xffffff
uint8_t __ALIGNED(4) lcdbuffer[1024];

/** spi flag */
static __IO uint8_t dma_cplt = 1;
static __IO uint8_t dma_error = 0;

// ģ��spi
// void LCD_Writ_Bus(uint8_t dat)
//{
//  uint8_t i;
//  LCD_CS_LOW;
//  for (i = 0; i < 8; i++)
//  {
//    LCD_SCL_LOW;
//    if (dat & 0x80)
//    {
//      LCD_SDA_HIGH;
//    }
//    else
//    {
//      LCD_SDA_LOW;
//    }
//    LCD_SCL_HIGH;
//    dat <<= 1;
//  }
//  LCD_CS_HIGH;
//}

void LCD_Writ_Bus(uint8_t dat)
{
  LCD_CS_LOW;
#ifdef USE_SPI_DMA_TRANS

  uint32_t dma_timeout = DMA_TIMEOUT;
  dma_cplt = 0;

  HAL_SPI_Transmit_DMA(&LCD_SPI_INTERFACE, &dat, 1);
  while (dma_cplt != 1 && dma_error != 1)
  {
    dma_timeout--;
    if (dma_timeout == 0)
    {
      printf("dma trans timeout\r\n");
      break;
    }
  }
  if (dma_error == 1)
  {
    printf("dma error\r\n");
    dma_error = 0;
  }

#else
  HAL_SPI_Transmit(&LCD_SPI_INTERFACE, &dat, 1, SPI_TRANS_TIMEOUT);
#endif
  LCD_CS_HIGH;
}

void LCD_WR_DATAS(uint8_t *pBuff, uint16_t size)
{
  LCD_CS_LOW;

#ifdef USE_SPI_DMA_TRANS

  uint32_t dma_timeout = DMA_TIMEOUT;
  dma_cplt = 0;

  HAL_SPI_Transmit_DMA(&LCD_SPI_INTERFACE, pBuff, size);
  while (dma_cplt != 1 && dma_error != 1)
  {
    dma_timeout--;
    if (dma_timeout == 0)
    {
      printf("dma trans timeout\r\n");
      break;
    }
  }
  if (dma_error == 1)
  {
    printf("dma error\r\n");
    dma_error = 0;
  }
#else
  HAL_SPI_Transmit(&LCD_SPI_INTERFACE, pBuff, size, SPI_TRANS_TIMEOUT);
#endif

  LCD_CS_HIGH;
}

/* LCD Write byte DATA */
void LCD_WR_DATA8(uint8_t dat)
{
  LCD_Writ_Bus(dat);
}

/* LCD Write 2bytes DATA */
void LCD_WR_DATA16(uint16_t dat)
{
  uint8_t tx[2] = {dat >> 8, dat & 0xff};
//  LCD_Writ_Bus(dat >> 8);
//  LCD_Writ_Bus(dat);
  LCD_WR_DATAS(tx, 2);
}

/* LCD Write CMD byte */
void LCD_WR_REG(uint8_t cmd)
{
  LCD_DC_LOW;
  LCD_Writ_Bus(cmd);
  LCD_DC_HIGH;
}

#ifdef USE_SPI_DMA_TRANS

/* lcd spi dma trans complete callback */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == LCD_SPI_INTERFACE.Instance)
  {
    dma_cplt = 1;
  }
}

/** lcd spi dma trans error callback **/
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == LCD_SPI_INTERFACE.Instance)
  {
    dma_error = 1;
  }
}

#endif
