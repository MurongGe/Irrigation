//
// Created by Zhou 2024/1/7
//

#include "lcd.h"
#include "lcd_io.h"
#include "FONT.H"

__IO uint16_t BACKCOLOR = BLACK;
__IO uint16_t POINT = WHITE;

/* LCD Init */
void lcd_init(void) ////ST7789V2
{
#ifndef USE_HAL_INTFC
  //SPI_SCK_1;			//特别注意！！
  LCD_SCL_HIGH;
#endif
  LCD_DC_HIGH;
  LCD_BLK_LOW;
  LCD_RES_LOW;
  lcd_delay(120);
  LCD_RES_HIGH;
  lcd_delay(120);
  LCD_WR_REG(0x11);            //Sleep Out
  lcd_delay(120);               //DELAY120ms

  LCD_WR_REG(0x36); // set display direction
#if (LCD_DIRECTION == LCD_DIRECT_0DEG)
  LCD_WR_DATA8(0x00);
#elif (LCD_DIRECTION == LCD_DIRECT_90DEG)
  LCD_WR_DATA8(0x70);
#elif (LCD_DIRECTION == LCD_DIRECT_180DEG)
  LCD_WR_DATA8(0xC0);
#else
  LCD_WR_DATA8(0xa0);
#endif

  /** -----------------------ST7789V Frame rate setting----------------- **/
  LCD_WR_REG(0x3A);
  LCD_WR_DATA8(0x05);

  LCD_WR_REG(0xB2);
  LCD_WR_DATA8(0x0C);
  LCD_WR_DATA8(0x0C);
  LCD_WR_DATA8(0x00);
  LCD_WR_DATA8(0x33);
  LCD_WR_DATA8(0x33);

  LCD_WR_REG(0xB7);
  LCD_WR_DATA8(0x35);

  LCD_WR_REG(0xBB);
  LCD_WR_DATA8(0x32); //Vcom=1.35V

  LCD_WR_REG(0xC2);
  LCD_WR_DATA8(0x01);

  LCD_WR_REG(0xC3);
  LCD_WR_DATA8(0x15); //GVDD=4.8V  颜色深度

  LCD_WR_REG(0xC4);
  LCD_WR_DATA8(0x20); //VDV, 0x20:0v

  LCD_WR_REG(0xC6);
  LCD_WR_DATA8(0x0F); //0x0F:60Hz

  LCD_WR_REG(0xD0);
  LCD_WR_DATA8(0xA4);
  LCD_WR_DATA8(0xA1);

  LCD_WR_REG(0xE0);
  LCD_WR_DATA8(0xD0);
  LCD_WR_DATA8(0x08);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x09);
  LCD_WR_DATA8(0x09);
  LCD_WR_DATA8(0x05);
  LCD_WR_DATA8(0x31);
  LCD_WR_DATA8(0x33);
  LCD_WR_DATA8(0x48);
  LCD_WR_DATA8(0x17);
  LCD_WR_DATA8(0x14);
  LCD_WR_DATA8(0x15);
  LCD_WR_DATA8(0x31);
  LCD_WR_DATA8(0x34);

  LCD_WR_REG(0xE1);
  LCD_WR_DATA8(0xD0);
  LCD_WR_DATA8(0x08);
  LCD_WR_DATA8(0x0E);
  LCD_WR_DATA8(0x09);
  LCD_WR_DATA8(0x09);
  LCD_WR_DATA8(0x15);
  LCD_WR_DATA8(0x31);
  LCD_WR_DATA8(0x33);
  LCD_WR_DATA8(0x48);
  LCD_WR_DATA8(0x17);
  LCD_WR_DATA8(0x14);
  LCD_WR_DATA8(0x15);
  LCD_WR_DATA8(0x31);
  LCD_WR_DATA8(0x34);
  LCD_WR_REG(0x21);
	/** end */

  lcd_fill(0, LCD_CURRENT_COLUMNS, 0, LCD_CURRENT_LINES, BLACK);

  LCD_BLK_HIGH;
  LCD_WR_REG(0x29);         //开启显示
}

void lcd_fill(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t color)
{
  uint32_t i, j;
  uint16_t height = x2 - x1 + 1;
  uint16_t width = y1 - y2 + 1;
  (height >= LCD_CURRENT_LINES) ? (height = LCD_CURRENT_LINES) : (height = height);
  (width >= LCD_CURRENT_COLUMNS) ? (width = LCD_CURRENT_COLUMNS) : (width = width);
  for (i = 0; i < width; i++) {
    lcdbuffer[2 * i] = color >> 8;
    lcdbuffer[2 * i + 1] = color & 0xff;
  }
  /* set area window */
  LCD_WR_REG(0x2a);
  LCD_WR_DATA16(x1 + TFTLCD_COLUMN_OFFSET);
  LCD_WR_DATA16(x1 + width + TFTLCD_COLUMN_OFFSET);
  LCD_WR_REG(0x2b);
  LCD_WR_DATA16(y1 + TFTLCD_LINE_OFFSET);
  LCD_WR_DATA16(y1 + height + TFTLCD_LINE_OFFSET);
  LCD_WR_REG(0x2C);   //memory write
  for (j = 0; j < height; j++) {
    LCD_WR_DATAS(lcdbuffer, width * 2);
  }
}

void lcd_setpos(uint16_t x, uint16_t y)
{
  LCD_WR_REG(0x2a);  // Column address set
  x += TFTLCD_COLUMN_OFFSET;
  LCD_WR_DATA8(x >> 8); // start column
  LCD_WR_DATA8(x);

  LCD_WR_REG(0x2b);  // Row address set
  y += TFTLCD_LINE_OFFSET;
  LCD_WR_DATA8(y >> 8); // start row
  LCD_WR_DATA8(y);

  LCD_WR_REG(0x2C);
}

void lcd_fast_point(uint16_t x, uint16_t y, uint16_t color)
{
  LCD_WR_REG(0x2a);
  LCD_WR_DATA16(x + TFTLCD_COLUMN_OFFSET);
  LCD_WR_REG(0x2b);
  LCD_WR_DATA16(y + TFTLCD_LINE_OFFSET);
  LCD_WR_REG(0x2C);
  LCD_WR_DATA16(color);
}

void lcd_showchar(uint16_t x, uint16_t y, uint8_t size, uint8_t mode, unsigned char ch)
{
  const unsigned char *pdata;
  uint8_t i, j;
  uint16_t x0 = x;
  uint8_t Bsize = (size == 12 || size == 16) ? size : size << 1;
  ch -= ' ';
  if (size == 12)
    pdata = &ASCII1206[ch][0];
  else if (size == 16)
    pdata = &ASCII1608[ch][0];
  else if (size == 24)
    pdata = &ASCII2412[ch][0];
  else if (size == 32)
    pdata = &ASCII3216[ch][0];
  else
    return;
  for (i = 0; i < Bsize; i++) {
    for (j = 0; j < 8; j++) {
      if (pdata[i] & (0x01 << j))
        lcd_fast_point(x, y, POINT);
      else if (mode == 0)
        lcd_fast_point(x, y, BACKCOLOR);
      x++;
      if ((x - x0) == size / 2) {
        y++;
        x = x0;
        break;
      }
    }
  }
}

void lcd_showstr(uint16_t x, uint16_t y, uint8_t size, uint8_t mode, unsigned char *str)
{
  uint16_t x0;
  x0 = x;
  while (*str != '\0') {
    lcd_showchar(x, y, size, mode, *str++);
    x += size / 2;
    if (x >= LCD_CURRENT_COLUMNS) {
      x = x0;
      y += size;
      if (y >= LCD_CURRENT_LINES)
        return;
    }
  }
}

void lcd_scanchar(uint16_t x, uint16_t y, uint8_t size, unsigned char ch)
{
  const unsigned char *pdata;
  uint8_t i, j;
  uint16_t pox;
  uint8_t Bsize = (size == 12 || size == 16) ? size : size << 1;
  ch -= ' ';
  if (size == 12)
    pdata = &ASCII1206[ch][0];
  else if (size == 16)
    pdata = &ASCII1608[ch][0];
  else if (size == 24)
    pdata = &ASCII2412[ch][0];
  else if (size == 32)
    pdata = &ASCII3216[ch][0];
  else
    return;
  /* set area window */
  LCD_WR_REG(0x2a);
  x += TFTLCD_COLUMN_OFFSET;
  LCD_WR_DATA16(x);
  LCD_WR_DATA16((x - 1 + size / 2));

  LCD_WR_REG(0x2b);
  y += TFTLCD_LINE_OFFSET;
  LCD_WR_DATA16(y);
  LCD_WR_DATA16((y + size - 1));

  pox = 0;
  for (i = 0; i < Bsize; i++) {
    for (j = 0; j < 8; j++) {
      if (pdata[i] & (0x01 << j)) {
        lcdbuffer[2 * pox] = POINT >> 8;
        lcdbuffer[2 * pox + 1] = POINT;
      } else {
        lcdbuffer[2 * pox] = BACKCOLOR >> 8;
        lcdbuffer[2 * pox + 1] = BACKCOLOR;
      }
      pox++;
      if ((pox % (size / 2)) == 0)   /* end of the byte scan */
      {
        break;
      }
    }
  }
  /* start send pox */
  LCD_WR_REG(0x2C);
  LCD_WR_DATAS(lcdbuffer, size * size);
}

void lcd_scanstr(uint16_t x, uint16_t y, uint8_t size, unsigned char *str)
{
  uint16_t x0;
  x0 = x;
  while (*str != '\0') {
    lcd_scanchar(x, y, size, *str++);
    x += size / 2;
    if (x >= LCD_CURRENT_COLUMNS) {
      x = x0;
      y += size;
      if (y >= LCD_CURRENT_LINES)
        return;
    }
  }
}

void lcd_fill_color(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, uint8_t *color)
{
  uint16_t i;
  uint16_t times;
  uint16_t left;
  uint16_t height = ey - sy + 1;
  uint16_t width = ex - sx + 1;
  times = height * width / DMATRANS_MAX_NUM;
  left = (height * width) % DMATRANS_MAX_NUM;
  /* set area window */
  LCD_WR_REG(0x2a);
  LCD_WR_DATA16(sx + TFTLCD_COLUMN_OFFSET);
  LCD_WR_DATA16(ex + TFTLCD_COLUMN_OFFSET);
  LCD_WR_REG(0x2b);
  LCD_WR_DATA16(sy + TFTLCD_LINE_OFFSET);
  LCD_WR_DATA16(ey + TFTLCD_LINE_OFFSET);

  LCD_WR_REG(0x2C);   //memory write
  for (i = 0; i < times; i++) {
    LCD_WR_DATAS(color + DMATRANS_MAX_NUM * i, DMATRANS_MAX_NUM * 2);
  }
  LCD_WR_DATAS(color + DMATRANS_MAX_NUM * i, left * 2);
}

