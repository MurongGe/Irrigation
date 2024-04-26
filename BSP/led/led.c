#include "led.h"

void led_light(uint8_t led, uint8_t led_state)
{
//  if(led == LED_BULE)
//  {
//    led_state ? (HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_RESET)):HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_SET);
//  } else 
  if(led == LED_GREEN){
    led_state ? (HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET)):HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
  } else if(led == LED_RED){
    led_state ? (HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET)):HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
  }
}
void led_off(void )
{
  HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
  //HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_SET);
}

