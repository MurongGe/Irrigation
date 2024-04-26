
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif
/** Includes ------------------------------------------------------------------*/
#include "main.h"

//#define LED_BULE 0
#define LED_GREEN 1
#define LED_RED 2

void led_light(uint8_t led, uint8_t led_state);
void led_off(void );

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
