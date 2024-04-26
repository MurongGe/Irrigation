file(GLOB BSP_SRC
        key/*.c
        led/*.c
        sys/*.c
        w25qxx/*.c
        myiic/*.c
        24cxx/*.c
        touch/ctiic.c
        touch/gt9xxx.c
        lcd_fsmc_st7789/*.c
        dht11/*.c
        sensor_adc/*.c
        ds1302/*.c
        ws2812/*.c
        pump/*.c
        beep/*.c
        cJson/*.c
        )

# 这句命令的位置不对会报错
add_library(bsp ${BSP_SRC})

target_include_directories(bsp PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../FreRTOS/include
        key
        lcd_fsmc_st7789/Inc
        w25qxx
        myiic
        24cxx
        sys
        touch
        led
        dht11
        sensor_adc
        ds1302
        ws2812
        beep
        pump
        cJson
        )