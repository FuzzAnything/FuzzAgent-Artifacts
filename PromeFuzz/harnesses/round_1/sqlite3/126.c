// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:19373:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void helper_vsnprintf(const char *fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    sqlite3_vsnprintf(sizeof(buffer), buffer, fmt, args);
    va_end(args);
}

static void helper_str_vappendf(sqlite3_str *s, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(s, fmt, args);
    va_end(args);
}

static void helper_vmprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    if (result) {
        sqlite3_free(result);
    }
    va_end(args);
}

static void test_sqlite3_vsnprintf(const uint8_t *data, size_t size) {
    char buffer[256];
    const char *format = "Test format: %.*s";
    
    int str_len = size > 255 ? 255 : (int)size;
    helper_vsnprintf(format, str_len, data);
}

static void test_sqlite3_str_appendf(const uint8_t *data, size_t size) {
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return;
    
    const char *format = "Test: %.*s";
    
    int str_len = size > 100 ? 100 : (int)size;
    char *temp_str = (char *)malloc(str_len + 1);
    if (temp_str) {
        memcpy(temp_str, data, str_len);
        temp_str[str_len] = '\0';
        
        sqlite3_str_appendf(str, format, str_len, temp_str);
        free(temp_str);
    }
    
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_sqlite3_str_vappendf(const uint8_t *data, size_t size) {
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return;
    
    const char *format = "Test: %.*s";
    
    int str_len = size > 100 ? 100 : (int)size;
    char *temp_str = (char *)malloc(str_len + 1);
    if (temp_str) {
        memcpy(temp_str, data, str_len);
        temp_str[str_len] = '\0';
        
        helper_str_vappendf(str, format, str_len, temp_str);
        free(temp_str);
    }
    
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_sqlite3_snprintf(const uint8_t *data, size_t size) {
    char buffer[256];
    const char *format = "Test: %.*s";
    
    int str_len = size > 100 ? 100 : (int)size;
    char *temp_str = (char *)malloc(str_len + 1);
    if (temp_str) {
        memcpy(temp_str, data, str_len);
        temp_str[str_len] = '\0';
        
        sqlite3_snprintf(sizeof(buffer), buffer, format, str_len, temp_str);
        free(temp_str);
    }
}

static void test_sqlite3_mprintf(const uint8_t *data, size_t size) {
    const char *format = "Test: %.*s";
    
    int str_len = size > 100 ? 100 : (int)size;
    char *temp_str = (char *)malloc(str_len + 1);
    if (temp_str) {
        memcpy(temp_str, data, str_len);
        temp_str[str_len] = '\0';
        
        char *result = sqlite3_mprintf(format, str_len, temp_str);
        if (result) {
            sqlite3_free(result);
        }
        free(temp_str);
    }
}

static void test_sqlite3_vmprintf(const uint8_t *data, size_t size) {
    const char *format = "Test: %.*s";
    
    int str_len = size > 100 ? 100 : (int)size;
    char *temp_str = (char *)malloc(str_len + 1);
    if (temp_str) {
        memcpy(temp_str, data, str_len);
        temp_str[str_len] = '\0';
        
        helper_vmprintf(format, str_len, temp_str);
        free(temp_str);
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *actual_data = Data + 1;
    size_t actual_size = Size > 1 ? Size - 1 : 0;
    
    if (selector & 0x01) {
        test_sqlite3_vsnprintf(actual_data, actual_size);
    }
    
    if (selector & 0x02) {
        test_sqlite3_str_appendf(actual_data, actual_size);
    }
    
    if (selector & 0x04) {
        test_sqlite3_str_vappendf(actual_data, actual_size);
    }
    
    if (selector & 0x08) {
        test_sqlite3_snprintf(actual_data, actual_size);
    }
    
    if (selector & 0x10) {
        test_sqlite3_mprintf(actual_data, actual_size);
    }
    
    if (selector & 0x20) {
        test_sqlite3_vmprintf(actual_data, actual_size);
    }
    
    return 0;
}