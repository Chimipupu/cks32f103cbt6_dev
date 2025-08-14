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
#include "dbg_com.hpp"
#include "app_main.hpp"

static xTaskHandle s_xTaskMain;

#ifdef BTN_USE
// static xTaskHandle s_xTaskButton;
// static uint8_t s_led_val = 1;
static bool s_is_btn_on = false;

void buttonISR(void)
{
    s_is_btn_on = true;
}

void vTaskButton(void *p_parameter)
{
    while (1)
    {
        // TODO: ボタンタスク処理
        if(s_is_btn_on != false) {
            Serial.printf("vTaskButton\n");
            s_is_btn_on = false;
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
#endif // BTN_USE

void vTaskMain(void *p_parameter)
{
    app_main_init();

    while (1)
    {
        // TODO: メインタスク処理
#if 0
        Serial.printf("vTaskMain\n");
        Serial.printf("Serial & LED Test(val = %d)\n", s_led_val);
        digitalWrite(OB_LED_PIN, s_led_val);
        s_led_val = !s_led_val;
#else
        app_main();
#endif
        vTaskDelay(3 / portTICK_PERIOD_MS);
    }
}

void app_freertos_init()
{
    // FreeRTOSタスク生成
#ifdef BTN_USE
    xTaskCreate(vTaskButton,       // コールバック関数ポインタ
                "vTaskButton",     // タスク名
                256,               // スタック
                nullptr,           // パラメータ
                1,                 // 優先度(0～7、7が最優先)
                &s_xTaskButton     // タスクハンドル
                );
#endif

    xTaskCreate(vTaskMain,         // コールバック関数ポインタ
                "vTaskMain",       // タスク名
                512,               // スタック
                nullptr,           // パラメータ
                6,                 // 優先度(0～7、7が最優先)
                &s_xTaskMain       // タスクハンドル
                );

    // FreeRTOS スケジュール開始
    vTaskStartScheduler();
}