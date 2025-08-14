/**
 * @file app_freertos.hpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief FreeRTOS アプリヘッダー
 * @version 0.1
 * @date 2025-08-14
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef APP_FREERTOS_HPP
#define APP_FREERTOS_HPP

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <STM32FreeRTOS.h>

void buttonISR(void);
void app_freertos_init();

#endif /* APP_FREERTOS_HPP */