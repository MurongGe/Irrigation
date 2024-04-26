//
// Created by Zhou 2024/1/7
//
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

extern __IO uint16_t BACKCOLOR;
extern __IO uint16_t POINT;

#define DMATRANS_MAX_NUM 		0x6000
#define TFTLCD_HOR_RES  		(uint16_t)240
#define TFTLCD_VER_RES  		(uint16_t)320
#define TFTLCD_HOR_OFFSET  	0
#define TFTLCD_VER_OFFSET  	0
/**** HOR ****
 *
 *
 *
 * VER
 *
 *
 * ***********/

/* TFT 1.69LCD display direction configuration */
#define LCD_DIRECT_0DEG     0
#define LCD_DIRECT_90DEG    1
#define LCD_DIRECT_180DEG   2
#define LCD_DIRECT_270DEG   3
#define LCD_DIRECTION LCD_DIRECT_90DEG

#if(LCD_DIRECTION == LCD_DIRECT_0DEG || LCD_DIRECTION == LCD_DIRECT_180DEG)
#define TFTLCD_LINE_OFFSET 		TFTLCD_VER_OFFSET
#define TFTLCD_COLUMN_OFFSET 	TFTLCD_HOR_OFFSET
#define LCD_CURRENT_LINES 		TFTLCD_VER_RES
#define LCD_CURRENT_COLUMNS 	TFTLCD_HOR_RES
#else
#define TFTLCD_LINE_OFFSET 		TFTLCD_HOR_OFFSET
#define TFTLCD_COLUMN_OFFSET 	TFTLCD_VER_OFFSET
#define LCD_CURRENT_LINES 		TFTLCD_HOR_RES
#define LCD_CURRENT_COLUMNS 	TFTLCD_VER_RES
#endif

// 画笔颜色
#define WHITE 	0xFFFF
#define BLACK 	0x0000
#define BLUE 		0x001F
#define BRED 		0XF81F
#define GRED 		0XFFE0
#define GBLUE 	0X07FF
#define RED 		0xF800
#define MAGENTA 0xF81F
#define GREEN 	0x07E0
#define CYAN 		0x7FFF
#define YELLOW 	0xFFE0
#define BROWN 	0XBC40 // 棕色
#define BRRED 	0XFC07 // 棕红色
#define GRAY 		0X8430 // 灰色

// GUI颜色
#define DARKBLUE 	0X01CF // 深蓝色
#define LIGHTBLUE 0X7D7C // 浅蓝色
#define GRAYBLUE 	0X5458 // 灰蓝色

// 以上三色为PANEL的颜色
#define LIGHTGREEN 	0X841F // 浅绿色
#define LIGHTGRAY   0XEF5B // 浅灰色(PANNEL)
#define LGRAY 			0XC618 // 浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE 	0XA651 // 浅灰蓝色(中间层颜色)
#define LBBLUE 			0X2B12 // 浅棕蓝色(选择条目的反色)

void lcd_init(void);

void lcd_fill(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t color);

void lcd_setpos(uint16_t x, uint16_t y);

void lcd_showchar(uint16_t x, uint16_t y, uint8_t size, uint8_t mode, unsigned char ch);

void lcd_showstr(uint16_t x, uint16_t y, uint8_t size, uint8_t mode, unsigned char *str);

void lcd_scanchar(uint16_t x, uint16_t y, uint8_t size, unsigned char ch);

void lcd_scanstr(uint16_t x, uint16_t y, uint8_t size, unsigned char *str);

void lcd_fill_color(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, uint8_t* color);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H__ */

