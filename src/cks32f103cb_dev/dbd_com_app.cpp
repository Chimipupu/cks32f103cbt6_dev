/**
 * @file dbd_com_app.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグモニタのアプリ
 * @version 0.1
 * @date 2025-08-02
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "dbg_com.hpp"
#include "ansi_esc.hpp"

#define MCU_NAME               "CSK32F103CBT6"
#define PCB_NAME               "BluePill"
#define CPU_TYPE                "ARM Cortex-M3"
#define MCU_FLASH_SIZE         128
#define MCU_RAM_SIZE           20
#define FW_VERSION_MAJOR       0
#define FW_VERSION_MINOR       0
#define FW_VERSION_REVISION    1

static void dbg_com_init_msg(dbg_cmd_args_t *p_args);

void cmd_help(dbg_cmd_args_t *p_args);
static void cmd_cls(dbg_cmd_args_t *p_args);
static void cmd_system(dbg_cmd_args_t *p_args);
static void cmd_mem_dump(dbg_cmd_args_t *p_args);

// コマンドテーブル
const dbg_cmd_info_t g_cmd_tbl[] = {
//  | 文字列 | 種類 | コールバック関数 | 最小引数 | 最大引数 | 説明 |
    {"help",    CMD_HELP,       &cmd_help,        0,    0,    "Command All Show"},
    {"cls",     CMD_CLS,        &cmd_cls,         0,    0,    "Display Clear"},
    {"sys",     CMD_SYSTEM,     &cmd_system,      0,    0,    "Show System Information"},
    {"memd",    CMD_MEM_DUMP,   &cmd_mem_dump,    2,    2,    "Memory Dump Command. args -> (#address, #length)"},
};

// コマンドテーブルのコマンド数(const)
const size_t g_cmd_tbl_size = sizeof(g_cmd_tbl) / sizeof(g_cmd_tbl[0]);

static void dbg_com_init_msg(dbg_cmd_args_t *p_args)
{
    Serial.printf("\nDebug Command Monitor for %s Ver%d.%d.%d\n",MCU_NAME,
                                                        FW_VERSION_MAJOR,
                                                        FW_VERSION_MINOR,
                                                        FW_VERSION_REVISION);
    Serial.printf("Copyright (c) 2025 Chimipupu All Rights Reserved.\n");
    Serial.printf("Type 'help' for available commands\n");
#ifdef _WDT_ENABLE_
    Serial.printf("[INFO] Wanning! WDT Enabled: %dms\n", _WDT_OVF_TIME_MS_);
#endif // _WDT_ENABLE_
}

void cmd_help(dbg_cmd_args_t *p_args)
{
    dbg_com_init_msg(p_args);

    Serial.printf("\nAvailable %d commands:\n", g_cmd_tbl_size);
    for (uint8_t i = 0; i < g_cmd_tbl_size; i++)
    {
        Serial.printf("  %-10s - %s\n", g_cmd_tbl[i].p_cmd_str, g_cmd_tbl[i].p_description);
    }
}

static void cmd_cls(dbg_cmd_args_t *p_args)
{
    Serial.printf(ANSI_ESC_CLS);
}

static void cmd_system(dbg_cmd_args_t *p_args)
{
    // volatile uint16_t start_time = micros();

    Serial.printf("\n[System Information]\n");

    // 基板
    Serial.printf("\n[PCB Info]\nPCB Name : %s\n", PCB_NAME);

    // マイコン
    Serial.printf("MCU : %s\n", MCU_NAME);
    Serial.printf("CPU : %s\n", CPU_TYPE);

    // ROM/RAM
    Serial.printf("\n[Mem Info]\n");
    Serial.printf("Flash Size : %d KB\n", MCU_FLASH_SIZE);
    Serial.printf("RAM Size : %d KB\n", MCU_RAM_SIZE);

    // クロック関連
    Serial.printf("\n[Clock Info]\n");
    Serial.printf("System Clock : %d MHz\r\n", SystemCoreClock / 1000000);

    // volatile uint16_t end_time = micros();
    // Serial.printf("proc time : %u us\n", end_time - start_time);
}

static void cmd_mem_dump(dbg_cmd_args_t *p_args)
{
    uint32_t addr;
    uint32_t length;

    if (p_args->argc != 3) {
        Serial.printf("Error: Invalid number of arguments. Usage: mem_dump <address> <length>\n");
        return;
    }

    // アドレスを16進数文字列から数値に変換
    if (sscanf(p_args->p_argv[1], "#%x", &addr) != 1) {
        Serial.printf("Error: Invalid address format. Use hexadecimal with # prefix (e.g., #F0000000)\n");
        return;
    }

    // 長さを16進数文字列から数値に変換
    if (sscanf(p_args->p_argv[2], "#%x", &length) != 1) {
        Serial.printf("Error: Invalid length format. Use hexadecimal with # prefix (e.g., #10)\n");
        return;
    }

    show_mem_dump(addr, length);
}