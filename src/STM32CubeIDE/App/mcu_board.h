/**
 * @file mcu_board.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief 基板ヘッダー
 * @version 0.1
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#ifndef MCU_BOARD_h
#define MCU_BOARD_h

#define __BLUE_PILL__          // BluePill
// #define __BLACK_PILL__         // BlackPill

// #define BTN_USE

#if defined(__BLUE_PILL__)
    #define OB_LED_PIN        PC13 // LEDのGPIOピン(PC13) アクティブLow
#elif defined(__BLACK_PILL__)
    #define OB_LED_PIN        PB12 // LEDのGPIOピン(PB12) アクティブLow
#else
    #define OB_LED_PIN        PA1  // LEDのGPIOピン(PA1) アクティブLow
#endif /* __BLUE_PILL__ */

#ifdef BTN_USE
#define BUTTON_PIN            PA8  // ボタンピン(PA8) アクティブLow
#endif // BTN_USE

#endif /* MCU_BOARD_h */