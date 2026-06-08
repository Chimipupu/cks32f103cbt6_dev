/**
 * @file app_main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン
 * @version 0.1
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#include "app_main.h"
#include "stm32f1xx_ll_utils.h"
#ifdef USE_DBG_COM
#include "dbg_com.h"
#endif // USE_DBG_COM
#include "main.h"

void app_main_init(void)
{
#ifdef USE_DBG_COM
    dbg_com_init(); // デバッグモニタ 初期化
#endif // USE_DBG_COM
}

void app_main(void)
{
    LL_GPIO_TogglePin(OB_LED_PIN_GPIO_Port, OB_LED_PIN_Pin);

#ifdef USE_DBG_COM
    dbg_com_main(); // (DEBUG)デバッグモニタ メイン
#endif // USE_DBG_COM

    LL_mDelay(300);
}
