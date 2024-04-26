//
// Created by ShunX on 2024/4/16.
//

#ifndef BISHE_IOT_MQTT_H
#define BISHE_IOT_MQTT_H

#include "main.h"
#include "usart.h"
#include "stdbool.h"
#include "string.h"
#include "sys.h"
#include "stdio.h"

#define MAX_RX_BUFF 512
#define MAX_AT_BUFF 256
extern uint8_t ut_rx[MAX_RX_BUFF];

struct __mqtt_receive_info {
    bool IsAirTempData;
    bool IsSoilHumidData;
    bool IsAirLightData;
    bool IsStateBulbData;
    bool IsStateBeepData;
    bool IsStatePumpData;
    bool IsLightDownData;
    bool IsSoilHumidDownData;
    bool IsAirTempUpData;
    bool IsIotControl;
    bool IotControl;
    uint8_t AirTemp;
    uint8_t SoilHumid;
    uint8_t AirLight;
    uint8_t StateBulb;
    uint8_t StateBeep;
    uint8_t StatePump;
    uint8_t LightDown;
    uint8_t SoilHumidDown;
    uint8_t AirTempUp;
};


uint8_t WiFi_connect(const char *ssid, const char *psd);

bool ut_AT_send(uint8_t *ptr, uint16_t size, uint16_t wait_time);

uint8_t mqtt_connect(void);

void mqtt_data_update(uint8_t airtemp, uint8_t soilhumid, uint8_t airlight);

void mqtt_sta_update(uint8_t sta_pump, uint8_t sta_bulb, uint8_t sta_beep);

void mqtt_threshold_update(uint8_t airtempup, uint8_t soilhumiddown, uint8_t lightdown);

void mqtt_monitor(void);

void decode_mqtt_json(char *str);

#endif //BISHE_IOT_MQTT_H
