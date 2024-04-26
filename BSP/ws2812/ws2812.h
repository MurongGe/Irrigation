//
// Created by ShunX on 2024/4/19.
//

#ifndef BISHE_WS2812_H
#define BISHE_WS2812_H

#include "main.h"

#define WS2812_T0H 34
#define WS2812_T1H 72

void ws2812_oneled(uint8_t R, uint8_t G, uint8_t B);
void ws2812_led(uint8_t sta);

#endif //BISHE_WS2812_H
