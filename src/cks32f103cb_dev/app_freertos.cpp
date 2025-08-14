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

#ifdef _WDT_ENABLE_
static void app_wdt_init(void);

static void app_wdt_init(void)
{
    // WDTでリセットしたか確認
    if (IWatchdog.isReset(true)) {
        Serial.printf("System WDT Reseted!\r\n");
    }

    // WDT初期化
    IWatchdog.begin(_WDT_OVF_TIME_MS_);

    if (!IWatchdog.isEnabled()) {
        while (1)
        {
            digitalWrite(OB_LED_PIN, HIGH);
            delay(100);
            digitalWrite(OB_LED_PIN, LOW);
            delay(100);
        }
    }
}
#endif // _WDT_ENABLE_

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
#ifdef _WDT_ENABLE_
    app_wdt_init();
#endif

    // アプリメイン初期化
    app_main_init();

    while (1)
    {
        app_main();

#ifdef _WDT_ENABLE_
        _WDT_CNT_RST();
#endif // _WDT_ENABLE_

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