//
// Created by ShunX on 2024/4/13.
//

#ifndef BISHE_DS1302_H
#define BISHE_DS1302_H

#include "main.h"
/******************************************************************************************/

#define OUT   0x01
#define IN    0x00

#define DS1302_CLK_PORT               DS1302_CLK_GPIO_Port
#define DS1302_CLK_PIN                DS1302_CLK_Pin

#define DS1302_RST_PORT               DS1302_RST_GPIO_Port
#define DS1302_RST_PIN                DS1302_RST_Pin

#define DS1302_DAT_PORT               DS1302_IO_GPIO_Port
#define DS1302_DAT_PIN                DS1302_IO_Pin

/* IO操作 */
#define RES_Set_1 HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_SET)
#define IO_Set_1  HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_SET)
#define CLK_Set_1  HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_SET)

#define RES_Reset_0 HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET)
#define IO_Reset_0  HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_RESET)
#define CLK_Reset_0  HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET)

#define IO_Read HAL_GPIO_ReadPin(DS1302_DAT_PORT, DS1302_DAT_PIN)

#define Time_24_Hour    0x00    //24小时制
#define Time_Start     0x00    //开始走时
//DS1302寄存器操作指令定义时间地址
#define ds1302_sec_addr   0x80       //秒
#define ds1302_min_addr   0x82       //分
#define ds1302_hour_addr  0x84       //时
#define ds1302_day_addr   0x86       //日
#define ds1302_month_addr 0x88       //月
#define ds1302_year_addr  0x8c       //年
/********************/
void DS1302_GPIOInit(void);
void DS1302_IO_GPIO(uint8_t FLAG);//配置IO的方向
void DS1302_delay(uint8_t dd);
void DS1302_Write(uint8_t add,uint8_t dat);
uint8_t DS1302_Read(uint8_t add);
void DS1302_SetTime(uint8_t *ad);
void DS1302_OFF(void);
void DS1302_ON(void);
void DS1302_init(void);
void DS1302_Readtime(void);


#endif //BISHE_DS1302_H
