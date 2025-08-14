/**
 * @file app_freertos.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief FreeRTOS アプリ
 * @version 0.1
 * @date 2025-08-14
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "mcu_board.hpp"
#include "app_freertos.hpp"

static xTaskHandle s_xTaskButton;
static xTaskHandle s_xTaskMain;
static uint8_t s_led_val = 1;

void buttonISR(void)
{
    // s_led_val = !s_led_val;
}

void vTaskButton(void *p_parameter)
{
    while (1)
    {
        // TODO: ボタンタスク処理
        Serial.printf("vTaskButton\n");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void vTaskMain(void *p_parameter)
{
    while (1)
    {
        // TODO: メインタスク処理
        Serial.printf("vTaskMain\n");
        Serial.printf("Serial & LED Test(val = %d)\n", s_led_val);
        digitalWrite(OB_LED_PIN, s_led_val);
        s_led_val = !s_led_val;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_freertos_init()
{
    // FreeRTOSタスク生成
    xTaskCreate(vTaskButton,       // コールバック関数ポインタ
                "vTaskButton",     // タスク名
                256,               // スタック
                nullptr,           // パラメータ
                3,                 // 優先度(0～7、7が最優先)
                &s_xTaskButton     // タスクハンドル
                );

    xTaskCreate(vTaskMain,         // コールバック関数ポインタ
                "vTaskMain",       // タスク名
                512,               // スタック
                nullptr,           // パラメータ
                7,                 // 優先度(0～7、7が最優先)
                &s_xTaskMain       // タスクハンドル
                );

    // FreeRTOS スケジュール開始
    vTaskStartScheduler();
}