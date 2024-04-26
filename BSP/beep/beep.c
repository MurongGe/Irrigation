//
// Created by ShunX on 2024/4/19.
//

#include "beep.h"

// 高频重入
void beep(uint8_t voice) {
  static uint32_t st = 0;
  static uint8_t state = 0;

  if (!voice) {
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    state = 0;
    st = 0;
    return;
  }
  switch (state) {
    case 0:
      HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
      st = HAL_GetTick();
      state = 1;
      break;
    case 1:
      if ((HAL_GetTick() - st) > 100) {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        st = HAL_GetTick();
        state = 2;
      }
      break;
    case 2:
      if ((HAL_GetTick() - st) > 100) {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        st = HAL_GetTick();
        state = 3;
      }
      break;
    case 3:
      if ((HAL_GetTick() - st) > 100) {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        st = HAL_GetTick();
        state = 4;
      }
      break;
    case 4:
      if ((HAL_GetTick() - st) > 500) {
        st = 0;
        state = 0;
      }
      break;
    default:
      return;
  }
}
