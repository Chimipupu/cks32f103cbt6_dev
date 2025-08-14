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
#include "CMSIS_DSP.h"

#define MCU_NAME               "CSK32F103CBT6"
#define PCB_NAME               "BluePill"
#define CPU_TYPE               "ARM Cortex-M3"
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
static void cmd_arm_math(dbg_cmd_args_t *p_args);

// コマンドテーブル
const dbg_cmd_info_t g_cmd_tbl[] = {
//  | 文字列 | 種類 | コールバック関数 | 最小引数 | 最大引数 | 説明 |
    {"help",    CMD_HELP,       &cmd_help,        0,    0,    "Command All Show"},
    {"cls",     CMD_CLS,        &cmd_cls,         0,    0,    "Display Clear"},
    {"sys",     CMD_SYSTEM,     &cmd_system,      0,    0,    "Show System Information"},
    {"memd",    CMD_MEM_DUMP,   &cmd_mem_dump,    2,    2,    "Memory Dump Command. args -> (#address, #length)"},
    {"amt",     CMD_MT_TEST,    &cmd_arm_math,    0,    0,    "ARM CMSIS DSP Lib Math Test"},
};

#define MAX_BLOCKSIZE   32
#define DELTA           (0.0001f)
const float32_t testInput_f32[MAX_BLOCKSIZE] =
{
    -1.244916875853235400,  -4.793533929171324800,   0.360705030233248850,   0.827929644170887320,  -3.299532218312426900,   3.427441903227623800,   3.422401784294607700,  -0.108308165334010680,
    0.941943896490312180,    0.502609575000365850,  -0.537345278736373500,   2.088817392965764500,  -1.693168684143455700,   6.283185307179590700,  -0.392545884746175080,   0.327893095115825040,
    3.070147440456292300,    0.170611405884662230,  -0.275275082396073010,  -2.395492805446796300,   0.847311163536506600,  -3.845517018083148800,   2.055818378415868300,   4.672594161978930800,
    -1.990923030266425800,   2.469305197656249500,   3.609002606064021000,  -4.586736582331667500,  -4.147080139136136300,   1.643756718868359500,  -1.150866392366494800,   1.985805026477433800
};
const float32_t testRefOutput_f32 = 1.000000000;
uint32_t blockSize = 32;
float32_t  testOutput;
float32_t  cosOutput;
float32_t  sinOutput;
float32_t  cosSquareOutput;
float32_t  sinSquareOutput;
arm_status status;

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
    Serial.printf(ANSI_ESC_PG_BRIGHT_YELLOW
                "[INFO] Wanning! WDT Enabled @ %d sec OVF!\n"
                ANSI_ESC_PG_RESET,
                _WDT_OVF_TIME_MS_ / 1000000);
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

// 三角関数の基本恒等式 cos²θ + sin²θ = 1 が成り立つかをテスト
static void cmd_arm_math(dbg_cmd_args_t *p_args)
{
    float32_t diff;
    uint32_t i;

    Serial.printf("ARM CMSIS DSP Lib Math Test");

    for (i = 0; i < blockSize; i++)
    {
        // cos(x), sin(x)を計算
        cosOutput = arm_cos_f32(testInput_f32[i]);
        sinOutput = arm_sin_f32(testInput_f32[i]);

        // cos²(x), sin²(x)を計算
        arm_mult_f32(&cosOutput, &cosOutput, &cosSquareOutput, 1);
        arm_mult_f32(&sinOutput, &sinOutput, &sinSquareOutput, 1);

        // cos²(x) + sin²(x)を計算
        arm_add_f32(&cosSquareOutput, &sinSquareOutput, &testOutput, 1);

        // 結果が1.0に近いかチェック（誤差DELTA = 0.0001f以内）
        diff = fabsf(testRefOutput_f32 - testOutput);
        status = (diff > DELTA) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;

        if (status == ARM_MATH_TEST_FAILURE) {
            break;
        }
    }

    if (status != ARM_MATH_SUCCESS) {
        Serial.printf("Test FAILURE\r\n");
    } else {
        Serial.printf("Test SUCCESS\r\n");
    }
}