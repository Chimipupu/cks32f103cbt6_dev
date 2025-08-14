/**
 * @file cks32f103cb_dev.ino
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief Arduino IDEのメイン関数
 * @version 0.1
 * @date 2025-08-14
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "mcu_board.hpp"
#include "app_freertos.hpp"

void setup(void)
{
    // GPIO 初期化
    pinMode(OB_LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, buttonISR, CHANGE);

    // Serial(USB-CDC) 初期化
    Serial.begin(115200);

    // FreeRTOS 初期化
    app_freertos_init();
}

void loop(void)
{
    // NOP
}