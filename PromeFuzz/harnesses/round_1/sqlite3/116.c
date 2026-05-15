// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:19373:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "sqlite3.h"

static void fuzz_format_string(const uint8_t *data, size_t size, char *format_buf, size_t format_len) {
    if (size == 0) {
        strcpy(format_buf, "%s");
        return;
    }
    
    size_t copy_len = size < format_len - 1 ? size : format_len - 1;
    memcpy(format_buf, data, copy_len);
    format_buf[copy_len] = '\0';
    
    for (size_t i = 0; i < copy_len; i++) {
        if (format_buf[i] == '\0') {
            format_buf[i] = 'A';
        }
    }
}

static void fuzz_string_arg(const uint8_t *data, size_t size, char *str_buf, size_t str_len) {
    if (size == 0) {
        strcpy(str_buf, "test");
        return;
    }
    
    size_t copy_len = size < str_len - 1 ? size : str_len - 1;
    memcpy(str_buf, data, copy_len);
    str_buf[copy_len] = '\0';
}

static int fuzz_int_arg(const uint8_t *data, size_t size) {
    if (size == 0) return 42;
    
    int result = 0;
    size_t bytes = size < sizeof(int) ? size : sizeof(int);
    memcpy(&result, data, bytes);
    return result;
}

static void test_vsnprintf_wrapper(const char *format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
    sqlite3_vsnprintf(sizeof(buffer), buffer, format, args);
    va_end(args);
}

static void test_vmprintf_wrapper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = sqlite3_vmprintf(format, args);
    va_end(args);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_str_vappendf_wrapper(sqlite3_str *str_obj, const char *format, ...) {
    if (!str_obj) return;
    
    va_list args;
    va_start(args, format);
    sqlite3_str_vappendf(str_obj, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    char format_buf[256];
    char str_buf[256];
    char int_str_buf[64];
    char buffer[512];
    
    fuzz_format_string(Data, Size, format_buf, sizeof(format_buf));
    fuzz_string_arg(Data, Size, str_buf, sizeof(str_buf));
    int int_arg = fuzz_int_arg(Data, Size);
    
    snprintf(int_str_buf, sizeof(int_str_buf), "%d", int_arg);
    
    // Test sqlite3_vsnprintf through wrapper
    test_vsnprintf_wrapper(format_buf, str_buf, int_str_buf);
    
    // Test sqlite3_snprintf
    sqlite3_snprintf(sizeof(buffer), buffer, format_buf, str_buf, int_str_buf);
    
    // Test sqlite3_mprintf
    char *mprintf_result = sqlite3_mprintf(format_buf, str_buf, int_str_buf);
    if (mprintf_result) {
        sqlite3_free(mprintf_result);
    }
    
    // Test sqlite3_vmprintf through wrapper
    test_vmprintf_wrapper(format_buf, str_buf, int_str_buf);
    
    // Test sqlite3_str_appendf and sqlite3_str_vappendf
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (str_obj) {
        sqlite3_str_appendf(str_obj, format_buf, str_buf, int_str_buf);
        
        // Test sqlite3_str_vappendf through wrapper
        test_str_vappendf_wrapper(str_obj, format_buf, str_buf, int_str_buf);
        
        char *str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    return 0;
}