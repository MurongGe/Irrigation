//
// Created by Zhou 2024/1/8
//

#include "key.h"
#include <stdio.h>

volatile uint8_t key_table[KEY_NUM] = {0};

static void update_key_sta(uint8_t key, uint8_t *flag, uint32_t *ticks);
static uint8_t read_key(uint8_t key_id);

/**
 * @brief 得到被按下按键的id(0~3) -1表示没有按键被按下
 * @note 该函数需要扫描
 * @retval key_id
 */
void update_key(void)
{
  uint8_t i;
  static uint32_t ticks[KEY_NUM] = {0};
  static uint8_t flag[KEY_NUM] = {0}; // 0, 未触发屏蔽， 1屏蔽中， 2屏蔽后
  for (i = 0; i < KEY_NUM; i++)
    update_key_sta(i, flag + i, ticks + i);
}

static void update_key_sta(uint8_t key, uint8_t *flag, uint32_t *ticks)
{
  switch (*flag)
  {
  case 0:
    if (read_key(key) != key_table[key])
    {
      *flag = 1;
      *ticks = HAL_GetTick(); // 获取 tick 值等待解锁
    }
    break;
  case 1:
    if (HAL_GetTick() >= (*ticks + 20))
    {
      *flag = 2;
    }
    break;
  case 2:
    key_table[key] = read_key(key);
    *flag = 0;
    break;
  default:
    break;
  }
}

static uint8_t read_key(uint8_t key_id)
{
  uint8_t res = 0;
  switch (key_id)
  {
  case 0:
    res = READ_KEY0;
    break;
  case 1:
    res = READ_KEY1;
    break;
  case 2:
    res = READ_KEY2;
    break;
  case 3:
    res = READ_KEY3;
    res = !res; //key3 按下为高电平，这里统一做低电平处理
    break;
  default:
    break;
  }
  return res;
}

