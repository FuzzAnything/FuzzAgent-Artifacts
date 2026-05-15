// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:19373:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void test_vsnprintf_helper(int n, char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_vsnprintf(n, buf, fmt, args);
    va_end(args);
}

static void test_vmprintf_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    va_end(args);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_str_vappendf_helper(sqlite3_str *str, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(str, fmt, args);
    va_end(args);
}

static void test_vsnprintf(const uint8_t *data, size_t size) {
    char buffer[256];
    
    if (size < 4) return;
    
    int n = data[0] % 256;
    const char *str = (const char *)(data + 1);
    size_t str_len = size - 1;
    if (str_len > 255) str_len = 255;
    
    test_vsnprintf_helper(sizeof(buffer), buffer, "Test: %.*s %d", (int)str_len, str, n);
}

static void test_vmprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    const char *str = (const char *)data;
    size_t str_len = size;
    if (str_len > 100) str_len = 100;
    
    test_vmprintf_helper("Value: %.*s", (int)str_len, str);
}

static void test_snprintf(const uint8_t *data, size_t size) {
    char buffer[256];
    
    if (size < 1) return;
    
    int n = data[0] % 256;
    const char *format = "Data: %.*s";
    const char *str = (const char *)(data + 1);
    size_t str_len = size - 1;
    if (str_len > 255) str_len = 255;
    
    sqlite3_snprintf(sizeof(buffer), buffer, format, (int)str_len, str);
}

static void test_str_vappendf(const uint8_t *data, size_t size) {
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (!str_obj) return;
    
    const char *str = (const char *)data;
    size_t str_len = size;
    if (str_len > 100) str_len = 100;
    
    test_str_vappendf_helper(str_obj, "Append: %.*s", (int)str_len, str);
    
    char *result = sqlite3_str_finish(str_obj);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_str_appendf(const uint8_t *data, size_t size) {
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (!str_obj) return;
    
    const char *format = "Format: %.*s";
    const char *str = (const char *)data;
    size_t str_len = size;
    if (str_len > 100) str_len = 100;
    
    sqlite3_str_appendf(str_obj, format, (int)str_len, str);
    
    char *result = sqlite3_str_finish(str_obj);
    if (result) {
        sqlite3_free(result);
    }
}

static void test_mprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    const char *format = "MPrint: %.*s";
    const char *str = (const char *)data;
    size_t str_len = size;
    if (str_len > 100) str_len = 100;
    
    char *result = sqlite3_mprintf(format, (int)str_len, str);
    if (result) {
        sqlite3_free(result);
    }
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    unsigned char selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0:
            test_vsnprintf(fuzz_data, fuzz_size);
            break;
        case 1:
            test_vmprintf(fuzz_data, fuzz_size);
            break;
        case 2:
            test_snprintf(fuzz_data, fuzz_size);
            break;
        case 3:
            test_str_vappendf(fuzz_data, fuzz_size);
            break;
        case 4:
            test_str_appendf(fuzz_data, fuzz_size);
            break;
        case 5:
            test_mprintf(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}