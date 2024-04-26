file(GLOB LVGL_SRC
        asset/*.c
        src/core/*.c
        src/draw/*.c
        src/draw/swm341_dma2d/*.c
        src/draw/sw/*.c
        src/font/*.c
        src/hal/*.c
        src/misc/*.c
        src/widgets/*.c
        examples/porting/*.c
        src/extra/*.c
        src/extra/layouts/flex/*.c
        src/extra/layouts/grid/*.c
        src/extra/libs/fsdrv/*.c
        src/extra/others/fragment/*.c
        src/extra/others/gridnav/*.c
        src/extra/others/ime/*.c
        src/extra/others/imgfont/*.c
        src/extra/others/monkey/*.c
        src/extra/others/msg/*.c
        src/extra/others/snapshot/*.c
        src/extra/themes/basic/*.c
        src/extra/themes/default/*.c
        src/extra/themes/mono/*.c
        src/extra/widgets/animimg/*.c
        src/extra/widgets/calendar/*.c
        src/extra/widgets/chart/*.c
        src/extra/widgets/colorwheel/*.c
        src/extra/widgets/imgbtn/*.c
        src/extra/widgets/keyboard/*.c
        src/extra/widgets/led/*.c
        src/extra/widgets/list/*.c
        src/extra/widgets/menu/*.c
        src/extra/widgets/meter/*.c
        src/extra/widgets/msgbox/*.c
        src/extra/widgets/span/*.c
        src/extra/widgets/spinbox/*.c
        src/extra/widgets/spinner/*.c
        src/extra/widgets/tabview/*.c
        src/extra/widgets/tileview/*.c
        src/extra/widgets/win/*.c
        )

# message(${LVGL_SRC})
# 这句命令的位置不对会报错
add_library(lvgl ${LVGL_SRC})

target_include_directories(lvgl PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../Fatfs
        examples/porting
        )