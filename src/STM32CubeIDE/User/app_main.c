/**
 * @file app_main.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#include "app_main.h"
#include "dbg_com.h"

/**
 * @brief メモリダンプ(16進HEX & Ascii)
 * 
 * @param dump_addr ダンプするメモリの32bitアドレス
 * @param dump_size ダンプするサイズ(Byte)
 */
void show_mem_dump(uint32_t dump_addr, uint32_t dump_size)
{
    printf("\n[Memory Dump '(addr:0x%04lX)]\n", dump_addr);

    // ヘッダー行を表示
    printf("Address  ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02X ", i);
    }
    printf("| ASCII\n");
    printf("-------- ");
    for (int i = 0; i < 16; i++)
    {
        printf("---");
    }
    printf("| ------\n");

    // 16バイトずつダンプ
    for (uint32_t offset = 0; offset < dump_size; offset += 16)
    {
        printf("%08lX: ", dump_addr + offset);

        // 16バイト分のデータを表示
        for (int i = 0; i < 16; i++)
        {
            if (offset + i < dump_size) {
                uint8_t data = *((volatile uint8_t*)(dump_addr + offset + i));
                printf("%02X ", data);
            } else {
                printf("   ");
            }
        }

        // ASCII表示
        printf("| ");
        for (int i = 0; i < 16; i++)
        {
            if (offset + i < dump_size) {
                uint8_t data = *((volatile uint8_t*)(dump_addr + offset + i));
                // 表示可能なASCII文字のみ表示
                printf("%c", (data >= 32 && data <= 126) ? data : '.');
            } else {
                printf(" ");  // データがない場合は空白を表示
            }
        }
        printf("\n");
    }
}

/**
 * @brief 関数の実行時間を計測する
 * 
 * @param func 計測対象の関数ポインタ
 * @param func_name 関数名（表示用）
 * @param ... 関数に渡す引数（可変長）
 */
void proc_exec_time(void (*func)(void), const char* func_name, ...)
{
#if 0
    volatile uint32_t start_time = micros();
    func();
    volatile uint32_t end_time = micros();
    printf("proc time %s: %u us\n", func_name, end_time - start_time);
#endif
}

/**
 * @brief アプリメイン初期化
 * 
 */
void app_main_init(void)
{
#ifdef USE_DBG_COM
    // デバッグモニタ 初期化
    dbg_com_init();
#endif // USE_DBG_COM
}

/**
 * @brief アプリメイン
 * 
 */
void app_main(void)
{
#ifdef USE_DBG_COM
    // (DEBUG)デバッグモニタ メイン
    dbg_com_main();
#endif // USE_DBG_COM
}
