


cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
set(FREERTOS_KERNEL_PATH "C:/FreeRTOS-Kernel")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(mpu6050_datalogger C CXX ASM)
pico_sdk_init()


include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/include) # Adiciona o diretório 'libs/include' à lista global de diretórios de headers 
include_directories(${CMAKE_SOURCE_DIR}/lib/include/tasks) # Adiciona o diretório 'libs/include' à lista global de diretórios de headers 
include_directories(${CMAKE_SOURCE_DIR}/lib/include/ws2812) # Adiciona o diretório 'libs/include' à lista global de diretórios de headers 
include_directories(${CMAKE_SOURCE_DIR}/lib/include/ssd1306) # Adiciona o diretório 'libs/include' à lista global de diretórios de headers 
include_directories(${CMAKE_SOURCE_DIR}/lib/include/mpu6050) # Adiciona o diretório 'libs/include' à lista global de diretórios de headers 
add_subdirectory(lib/FatFs_SPI)  


add_executable(${PROJECT_NAME}  
        mpu6050_datalogger.c 
        lib/source/mpu6050/mpu6050.c
        lib/source/ssd1306/ssd1306.c # Biblioteca para o display OLED
        lib/source/ws2812/ws2812.c # Biblioteca para a matriz 5x5
        lib/source/setup.c
        lib/source/buzzer.c
        lib/source/sd_utils.c
        lib/source/hw_config.c
        lib/source/tasks/sd_control_panel_task.c
        lib/source/tasks/monitoring_screen_task.c
        lib/source/tasks/task_entrada.c
        lib/source/tasks/task_saida.c
        lib/source/tasks/task_leds.c
        lib/source/tasks/sd_action_task.c
        lib/source/tasks/task_matrix.c
        lib/source/tasks/mpu6050_task.c
        )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

pico_generate_pio_header(${PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/source/ws2812/ws2812.pio 
    OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated
)

target_link_libraries(${PROJECT_NAME} 
        hardware_timer
        pico_stdlib 
        hardware_gpio
        hardware_i2c
        hardware_adc
        hardware_pwm
        hardware_pio
        hardware_clocks
        pico_bootrom
        FatFs_SPI
        FreeRTOS-Kernel 
        FreeRTOS-Kernel-Heap4
        )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})




