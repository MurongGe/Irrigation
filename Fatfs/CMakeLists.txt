file(GLOB FAT_SRC
        *.c
        )

# 这句命令的位置不对会报错
add_library(fatfs ${FAT_SRC})

target_include_directories(fatfs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
