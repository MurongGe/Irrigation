/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25qxx.h"
#include <stdio.h>
#include "sys.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "lv_conf.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "fsmain.h"
#include "gt9xxx.h"
#include "gui.h"
#include "dht11.h"
#include "sensor_adc.h"
#include "myiic.h"
#include "ds1302.h"
#include "iot_mqtt.h"
#include "cJSON.h"
#include "beep.h"
#include "ws2812.h"
#include "pump.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
extern void myLvglTimerTask(lv_timer_t *tmr);
bool alert_cmp(uint8_t hur, uint8_t min, uint8_t hold_min, uint8_t hold_sec);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
SysInfoType SysInfo;
__IO uint16_t tick = 0;
__IO uint8_t time_data[6];
__IO uint8_t start_connect_wifi = 0;
__IO uint8_t wifi_connected_flag = 0; //0默认未连接，1连接成功2连接失败3已经连接
__IO uint8_t LvglDataUpdateFlag = 0;
__IO uint8_t IsReceiveMQTTMsg = 0;
__IO uint8_t dht11_rety_flag = 0;
__IO uint8_t IsTimeUpdate = 0;
__IO uint8_t IsIoTControl = 0;
const char *got_wifi_name = NULL;
const char *got_wifi_psd = NULL;
extern struct __mqtt_receive_info mqtt_recv_info;
extern struct __Head_alert_Info Head_alert_info;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_FSMC_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  iic_init();
  gt9xxx_init();
  led_off();
  WQX_Init();
  lcd_init();
  fs_init();
  DS1302_init();
  if (dht11_init() == 0) {
    printf("dht11 OK\r\n");
  } else
    printf("dht11 error\r\n");

  /// 初始化系统参数
  beep(0);
  pump(0);
  ws2812_led(0);
  SysInfo.sys_mode = 0;
  SysInfo.air_temp_up = 30;
  SysInfo.light_down = 40;
  SysInfo.soil_humid_down = 10;

  /// 锁存ESP模块RST和GPIO0引脚
  HAL_GPIO_LockPin(ESP01_GPIO0_GPIO_Port, ESP01_GPIO0_Pin);
  HAL_GPIO_LockPin(ESP01_RST_GPIO_Port, ESP01_RST_Pin);

  /// 开启串口中断，自动搬运数据至接收缓冲区
  HAL_UART_Receive_IT(&huart3, ut_rx, 1);

  /// LVGL输入输出设备初始化
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  /// lvgl 定时任务
  HAL_TIM_Base_Start_IT(&htim3); //5ms

  /// lvgl 数据刷新任务
  lv_timer_create(myLvglTimerTask, 100, NULL);

  /// Gui界面初始化
  gui_init();

  /// cJson使用 lvgl内存管理函数
  cJSON_Hooks hooks = {lv_mem_alloc, lv_mem_free};
  cJSON_InitHooks(&hooks);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    delay_ms(1);

    /// 扫描键盘
    if (tick % 100 == 0) {
      /// uint32_t st = HAL_GetTick();
      update_key();
      /// printf("getkey polling time:%ld\r\n", HAL_GetTick()-st);
    }

    /// 读取DHT11温湿度数据
    if (tick % 500 == 100) {
      ///  dht11_rety_flag ^= 0x01;
      /// uint32_t st = HAL_GetTick();
      dht11_read_data(&SysInfo.air_temp, &SysInfo.air_humid);
      /// printf("temp:%d  humi:%d\r\n", SysInfo.air_temp, SysInfo.air_humid);
      /// printf("dht11 polling time:%ld\r\n", HAL_GetTick()-st);
    }

    /// 读取AD转换值
    if (tick % 500 == 300) {
      /// uint32_t st = HAL_GetTick();
      extern uint8_t ADConver_Res[4];
      PCF_Get_ADC();
      SysInfo.air_light = (256 - ADConver_Res[2]) / 256.0f * 100.0f;
      SysInfo.soil_humid = (256 - ADConver_Res[0]) / 256.0f * 100.0f;
      /// printf("pcf polling time:%ld\r\n", HAL_GetTick()-st);
      /// printf("%d %d %d %d\r\n", ADConver_Res[0], ADConver_Res[1], ADConver_Res[2], ADConver_Res[3]);
      /// printf("light:%d humi:%d\r\n", SysInfo.air_light, SysInfo.soil_humid);
    }

    /// 读取实时时间
    if (tick % 500 == 200) {
      /// uint32_t st = HAL_GetTick();
      DS1302_Readtime();
      SysInfo.hur = time_data[3];
      SysInfo.min = time_data[4];
      SysInfo.sec = time_data[5];
      /// 给LVGL 信号量刷新时
      IsTimeUpdate = 1;
      /// printf("ds1302 polling time:%ld\r\n", HAL_GetTick()-st);
      /// printf("%d %d %d %d %d %d \r\n", time_data[0],time_data[1],time_data[2],time_data[3],time_data[4],time_data[5]);
    }

    /// 开始连接Wi-Fi
    if (start_connect_wifi) {
      start_connect_wifi = 0;
      uint8_t res = WiFi_connect(got_wifi_name, got_wifi_psd);
      if (res == 0) {
        res = mqtt_connect();
        if (res == 0) {
          wifi_connected_flag = 1;
        } else {
          wifi_connected_flag = 2;
          printf("mqtt connect error: %d\r\n", res);
        }
      } else {
        wifi_connected_flag = 2;
        printf("wifi connect error :%d\r\n", res);
      }
    }

    /// 如果 WiFi 连接成功 监听 订阅数据
    if (wifi_connected_flag == 3 && tick % 500 == 100) {
      mqtt_monitor();
    }

    /// 无条件向云端上传数据
     if (tick == 330 && wifi_connected_flag == 3) {
       mqtt_data_update(SysInfo.air_temp, SysInfo.soil_humid, SysInfo.air_light);
     }
     if (tick == 660 && wifi_connected_flag == 3) {
       mqtt_sta_update(SysInfo.sta_pump, SysInfo.sta_bulb, SysInfo.sta_beep);
     }
     if (tick == 990 && wifi_connected_flag == 3) {
       mqtt_threshold_update(SysInfo.air_temp_up, SysInfo.soil_humid_down, SysInfo.light_down);
     }

    /// 自动模式 控制
    if (SysInfo.sys_mode == 0) {
      /// 给lvgl 数据刷新任务 信号
      if (tick % 250 == 0) {
        LvglDataUpdateFlag = 1;
      }
      if (IsReceiveMQTTMsg) {
        IsReceiveMQTTMsg = 0;
        printf("rev mqtt data\r\n");

        /// 自动模式可以接收 网络阈值
        if (mqtt_recv_info.IsAirTempUpData) {
          SysInfo.air_temp_up = mqtt_recv_info.AirTempUp;
          mqtt_recv_info.IsAirTempUpData = false;
        }
        if (mqtt_recv_info.IsSoilHumidDownData) {
          SysInfo.soil_humid_down = mqtt_recv_info.SoilHumidDown;
          mqtt_recv_info.IsSoilHumidDownData = false;
        }
        if (mqtt_recv_info.IsLightDownData) {
          SysInfo.light_down = mqtt_recv_info.LightDown;
          mqtt_recv_info.IsLightDownData = false;
        }
      }

      /// 温度控制
      if (SysInfo.air_temp_up < SysInfo.air_temp) {
        /// 供水 提示
        SysInfo.sta_beep = 1;
      } else {
        SysInfo.sta_beep = 0;
      }
      /// 光照控制
      if (SysInfo.light_down > SysInfo.air_light) {
        SysInfo.sta_bulb = 1;
      } else {
        SysInfo.sta_bulb = 0;
      }
      /// 湿度控制
      if (SysInfo.soil_humid < SysInfo.soil_humid_down) {
        SysInfo.sta_pump = 1;
      } else {
        SysInfo.sta_pump = 0;
      }

      /// 闹钟控制 优先级大于 自动规则
      if (Head_alert_info.num_alert) {
        Alert_Body_Type *opt = Head_alert_info.first;
        for (int i = 0; i < Head_alert_info.num_alert; i++) {
          if (opt->event_alerting) {
            if (alert_cmp(opt->time_alert.hur, opt->time_alert.min, opt->time_hold.min, opt->time_hold.sec)) {
              SysInfo.sta_beep = 1;
            } else {
              SysInfo.sta_beep = 0;
            }
          } else if (opt->event_lighting) {
            if (alert_cmp(opt->time_alert.hur, opt->time_alert.min, opt->time_hold.min, opt->time_hold.sec)) {
              SysInfo.sta_bulb = 1;
            } else {
              SysInfo.sta_bulb = 0;
            }
          } else if (opt->event_pumping) {
            if (alert_cmp(opt->time_alert.hur, opt->time_alert.min, opt->time_hold.min, opt->time_hold.sec)) {
              SysInfo.sta_pump = 1;
            } else {
              SysInfo.sta_pump = 0;
            }
          }
          opt = opt->next;
        }
      }
    } else {

      /// 手动模式才能网络控制
      if (mqtt_recv_info.IsIotControl && IsReceiveMQTTMsg) {
        IsReceiveMQTTMsg = 0;
        IsIoTControl = mqtt_recv_info.IotControl;
        mqtt_recv_info.IsIotControl = false;
      }

      /// 手动控制
      if (IsIoTControl) {
        if (mqtt_recv_info.IsStatePumpData) {
          SysInfo.sta_pump = mqtt_recv_info.StatePump;
          mqtt_recv_info.IsStatePumpData = false;
        }
        if (mqtt_recv_info.IsStateBeepData) {
          SysInfo.sta_beep = mqtt_recv_info.StateBeep;
          mqtt_recv_info.IsStateBeepData = false;
        }
        if (mqtt_recv_info.IsStateBulbData) {
          SysInfo.sta_bulb = mqtt_recv_info.StateBulb;
          mqtt_recv_info.IsStateBulbData = false;
        }
      }
    }

    SysInfo.sta_pump ? pump(1) : pump(0);
    SysInfo.sta_bulb ? ws2812_led(250) : ws2812_led(0);
    SysInfo.sta_beep ? beep(1) : beep(0);

    tick++;
    if (tick == 1000) {
      tick = 0;
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/// 检查 此时是否在于闹钟时间
bool alert_cmp(uint8_t hur, uint8_t min, uint8_t hold_min, uint8_t hold_sec) {
  uint32_t sec_s = hur * 3600 + min * 60;
  uint32_t sec_e = sec_s + hold_min * 60 + hold_sec;
  uint32_t sec_crt = SysInfo.hur * 3600 + SysInfo.min * 60 + SysInfo.sec;
  if (sec_crt >= sec_s && sec_crt <= sec_e) {
    return true;
  } else return false;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
