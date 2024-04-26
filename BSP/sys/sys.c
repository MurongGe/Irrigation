//
// Created by ShunX on 2023/7/2.
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include <string.h>

#define EXTRA_OS_MHZ 168
uint8_t uart_msg_buff[128];

/**
 * @brief       延时nus
 * @param       nus: 要延时的us数.
 * @note        nus取值范围 : 0~190887435(最大值即 2^32 / fac_us @fac_us = 1)
 * @retval      无
 */
void delay_us(uint32_t nus)
{
  uint32_t ticks;
  uint32_t told, tnow, tcnt = 0;

  uint32_t reload = SysTick->LOAD; /* LOAD的值 */

  ticks = nus * EXTRA_OS_MHZ;    /* 需要的节拍数, 168个tick一微秒 */
  told = SysTick->VAL; /* 刚进入时的计数器值 */

  while (1) {
    tnow = SysTick->VAL;
    if (tnow != told) {
      if (tnow < told) {
        tcnt += told - tnow; /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
      } else {
        tcnt += ((reload - tnow) + told);
      }
      told = tnow;
      if (tcnt >= ticks) {
        break; /* 时间超过/等于要延迟的时间,则退出 */
      }
    }
  }
}

void delay_ms(uint32_t xms)
{
  uint32_t i;
  for (i = xms; i > 0; i--) {
    delay_us(1000);
  }
}

/*====================================== END =========================================*/
#include <stdio.h>
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  while ((USART1->SR & 0X40) == 0);
  USART1->DR = (uint8_t) ch;
  return ch;
}

#ifdef __GNUC__

int _write(int file, char *ptr, int len)
{
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++) {
    __io_putchar(*ptr++);
  }
  return len;
}

#endif
/*====================================== END =========================================*/

/* ############################### Uart CMD #################################*/
#define USE_UART_CMD 0
//#define UART_TRANSE_W25QXX

// '\r' = 13(0x0d) '\n' = 10(0x0a)
// 命令接受
// "CMD+'A'(B,C,D)\r\n"

extern __IO uint8_t uart_cmd; // main.c

#if USE_UART_CMD

void uart_cmd_hander(void)
{
  uint8_t i;
  uint8_t temp[5] = {0};
  for (i = 0; i < 4; i++)
    temp[i] = uart_msg_buff[i];

  uart_cmd = uart_msg_buff[4];
  if (strcmp((char *)temp, "CMD+") == 0)
  {
    uart_cmd = uart_msg_buff[4];
  }
  else
    uart_cmd = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  static __IO uint8_t RxCNT = 0;

  uint8_t i;
  if (huart->Instance == USART1)
  {
    if (uart_msg_buff[RxCNT] == '\n' && RxCNT && uart_msg_buff[RxCNT - 1] == '\r')
    {
      printf("%s\r\n", uart_msg_buff);
      uart_cmd_hander();

      for (i = 0; i < RxCNT + 1; i++)
        uart_msg_buff[i] = 0;
      RxCNT = 0;
      HAL_UART_Receive_IT(&huart1, uart_msg_buff, 1);
    }
    else
    {
      RxCNT++;
      if (RxCNT == 100)
      {
        RxCNT = 0;
        for (i = 0; i < 100; i++)
          uart_msg_buff[i] = 0;
      }
      HAL_UART_Receive_IT(&huart1, &uart_msg_buff[RxCNT], 1);
    }
  }
}

#endif

#ifdef UART_TRANSE_W25QXX

__IO uint16_t times = 0;
__IO uint8_t rx_flag = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) {
    rx_flag = 1;
    printf("writing 1024k data...\r\n");

    times++;

    if (times == 596)
      printf("All data Received\r\n");
    else
      HAL_UART_Receive_IT(&huart1, uart_msg_buff, 1024);

  }
}

#endif

