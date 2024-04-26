//
// Created by ShunX on 2024/4/19.
//

#include "pump.h"


void pump(uint8_t val)
{
  val ? HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PIN_RESET);
}