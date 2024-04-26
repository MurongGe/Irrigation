//
// Created by Zhou 2024/1/7
//

#include "lcd.h"
#include "lcd_fsmc_io.h"
#include "FONT.H"
#include "stdio.h"

__IO uint16_t BACKCOLOR = BLACK;
__IO uint16_t POINT = WHITE;

/* LCD Init */
void lcd_init(void) ////ST7789V2
{
  LCD_BLK_LOW;
  LCD_WR_REG(0x11);       //Sleep Out
  lcd_delay(120);               //DELAY120ms

  LCD_WR_REG(0x36); // set display direction
#if (LCD_DIRECTION == LCD_DIRECT_0DEG)
  LCD_WR_DATA(0x00);
#elif (LCD_DIRECTION == LCD_DIRECT_90DEG)
  LCD_WR_DATA(0x70);
#elif (LCD_DIRECTION == LCD_DIRECT_180DEG)
  LCD_WR_DATA(0xC0);
#else
  LCD_WR_DATA(0xa0);
#endif

  /** -----------------------ST7789V Frame rate setting----------------- **/
  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x05);

  LCD_WR_REG(0xB2);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x33);
  LCD_WR_DATA(0x33);

  LCD_WR_REG(0xB7);
  LCD_WR_DATA(0x71);

  LCD_WR_REG(0xBB);
  LCD_WR_DATA(0x3B); //Vcom=1.35V

  LCD_WR_REG(0xC0);
  LCD_WR_DATA(0x2C);

  LCD_WR_REG(0xC2);
  LCD_WR_DATA(0x01);

  LCD_WR_REG(0xC3);
  LCD_WR_DATA(0x13); //GVDD=4.8V  颜色深度

  LCD_WR_REG(0xC4);
  LCD_WR_DATA(0x20); //VDV, 0x20:0v

  LCD_WR_REG(0xC6);
  LCD_WR_DATA(0x0F); //0x0F:60Hz

  LCD_WR_REG(0xD0);
  LCD_WR_DATA(0xA4);
  LCD_WR_DATA(0xA1);

  LCD_WR_REG(0xE0);
  LCD_WR_DATA(0xD0);
  LCD_WR_DATA(0x08);
  LCD_WR_DATA(0x0A);
  LCD_WR_DATA(0x0D);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x07);
  LCD_WR_DATA(0x21);
  LCD_WR_DATA(0x33);
  LCD_WR_DATA(0x39);
  LCD_WR_DATA(0x39);
  LCD_WR_DATA(0x16);
  LCD_WR_DATA(0x16);
  LCD_WR_DATA(0x1F);
  LCD_WR_DATA(0x3C);

  LCD_WR_REG(0xE1);
  LCD_WR_DATA(0xD0);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x01);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x21);
  LCD_WR_DATA(0x32);
  LCD_WR_DATA(0x38);
  LCD_WR_DATA(0x16);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x20);
  LCD_WR_DATA(0x3D);

  LCD_WR_REG(0x21);

  /** end */
  lcd_clear(BLACK);

  LCD_BLK_HIGH;
  LCD_WR_REG(0x29);         //开启显示
}

void lcd_clear(uint16_t color)
{
  lcd_set_window(0,LCD_CURRENT_COLUMNS-1,0,LCD_CURRENT_LINES-1);
  lcd_wr_gram();
  for(uint32_t i = 0; i< LCD_CURRENT_LINES * LCD_CURRENT_COLUMNS; i++)
    LCD_WR_DATA(color);
}

void lcd_fill(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, uint16_t color)
{
  uint16_t width = ex - sx + 1;
  uint16_t height = ey - sy + 1;
  (height >= LCD_CURRENT_LINES) ? (height = LCD_CURRENT_LINES) : (height = height);
  (width >= LCD_CURRENT_COLUMNS) ? (width = LCD_CURRENT_COLUMNS) : (width = width);

  /* set area window */
  lcd_set_window(sx, ex, sy, ey);
  lcd_wr_gram();
  for(uint32_t i = 0; i< height*width; i++)
  {
    LCD_WR_DATA(color);
  }
}

void lcd_set_point(uint16_t x, uint16_t y)
{
  lcd_set_window(x,x,y,y);
}

void lcd_set_window(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey)
{
  LCD_WR_REG(0x2A);
  LCD_WR_DATA(sx >> 8);   // start Xpos
  LCD_WR_DATA(sx & 0xff);

  LCD_WR_DATA(ex >> 8);    // end Xpos
  LCD_WR_DATA(ex & 0xff);

  LCD_WR_REG(0x2B);
  LCD_WR_DATA(sy >> 8);   // start Ypos
  LCD_WR_DATA(sy & 0xff);

  LCD_WR_DATA(ey >> 8);   // end Ypos
  LCD_WR_DATA(ey & 0xff);
}

void lcd_wr_gram(void)
{
  LCD_WR_REG(0x2c);
}

void lcd_fast_point(uint16_t x, uint16_t y, uint16_t color)
{
  lcd_set_point(x, y);
  lcd_wr_gram();
  LCD_WR_DATA(color);
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

void lcd_fill_color(uint16_t sx, uint16_t ex, uint16_t sy, uint16_t ey, uint16_t *color)
{
  /* set area window */
  lcd_set_window(sx, ex, sy, ey);
  lcd_wr_gram();
  for(uint32_t i = 0 ; i<(ex-sx+1)*(ey-sy+1); i++)
    LCD_WR_DATA(color[i]);
}
