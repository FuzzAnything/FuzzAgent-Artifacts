// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void dummy_log_callback(void *pArg, int iErrCode, const char *zMsg) {
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
}

static void test_vmprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = sqlite3_vmprintf(format, args);
    va_end(args);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_str_vappendf(sqlite3_str *str, const char *format, ...) {
    if (!str) return;
    va_list args;
    va_start(args, format);
    sqlite3_str_vappendf(str, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3_initialize();
    
    size_t data_len = Size > 1024 ? 1024 : Size;
    char *input_str = (char *)malloc(data_len + 1);
    if (!input_str) return 0;
    
    memcpy(input_str, Data, data_len);
    input_str[data_len] = '\0';
    
    char buffer1[256];
    sqlite3_snprintf(sizeof(buffer1), buffer1, "Format: %s, int: %d", input_str, (int)Size);
    
    char *result1 = sqlite3_mprintf("Allocated: %s, size: %lld", input_str, (long long)Size);
    if (result1) {
        sqlite3_free(result1);
    }
    
    sqlite3_config(SQLITE_CONFIG_LOG, dummy_log_callback, NULL);
    sqlite3_log(SQLITE_OK, "Log message: %s, data size: %zu", input_str, Size);
    sqlite3_log(SQLITE_ERROR, input_str);
    
    test_vmprintf("Varargs: %s, %d", input_str, (int)Size);
    test_vmprintf("Test: %s", input_str);
    
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (str_obj) {
        sqlite3_str_appendf(str_obj, "String builder: %s, length: %zu", input_str, Size);
        test_str_vappendf(str_obj, " Varargs append: %s, %d", input_str, (int)Size);
        char *str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    char buffer2[64];
    sqlite3_snprintf(sizeof(buffer2), buffer2, "Short: %s", input_str);
    
    char buffer3[512];
    sqlite3_snprintf(sizeof(buffer3), buffer3, "Long buffer test with string: %s and number: %u", 
                     input_str, (unsigned int)Size);
    
    char buffer4[1];
    sqlite3_snprintf(sizeof(buffer4), buffer4, "");
    
    char *result3 = sqlite3_mprintf("");
    if (result3) {
        sqlite3_free(result3);
    }
    
    for (int i = 0; i < 5; i++) {
        sqlite3_log(i, "Error code %d: %s", i, input_str);
    }
    
    test_vmprintf(NULL);
    
    free(input_str);
    sqlite3_shutdown();
    
    return 0;
}