// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_sqlite3_snprintf(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a buffer for snprintf
    int buf_size = (data[0] % 128) + 1;  // Buffer size between 1-128
    char *buffer = (char *)malloc(buf_size);
    if (!buffer) return;
    
    // Use part of the input as format string (null-terminated)
    size_t fmt_len = size > 100 ? 100 : size - 1;
    char *fmt = (char *)malloc(fmt_len + 1);
    if (!fmt) {
        free(buffer);
        return;
    }
    memcpy(fmt, data + 1, fmt_len);
    fmt[fmt_len] = '\0';
    
    // Call sqlite3_snprintf with various arguments
    char *result = sqlite3_snprintf(buf_size, buffer, fmt, 
                                   (int)size, (long)size, (double)size);
    (void)result;  // Result is same as buffer
    
    free(buffer);
    free(fmt);
}

// Variadic helper function for sqlite3_vmprintf
static void call_sqlite3_vmprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    va_end(args);
    
    if (result) {
        sqlite3_free(result);
    }
}

static void test_sqlite3_vmprintf(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create format string from input
    size_t fmt_len = size > 100 ? 100 : size - 1;
    char *fmt = (char *)malloc(fmt_len + 1);
    if (!fmt) return;
    memcpy(fmt, data + 1, fmt_len);
    fmt[fmt_len] = '\0';
    
    // Call through variadic helper
    call_sqlite3_vmprintf(fmt, (int)size, (long)size, (double)size);
    
    free(fmt);
}

static void test_sqlite3_mprintf(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create format string from input
    size_t fmt_len = size > 100 ? 100 : size - 1;
    char *fmt = (char *)malloc(fmt_len + 1);
    if (!fmt) return;
    memcpy(fmt, data + 1, fmt_len);
    fmt[fmt_len] = '\0';
    
    // Call sqlite3_mprintf with various arguments
    char *result = sqlite3_mprintf(fmt, 
                                  (int)size, 
                                  (long long)size, 
                                  (double)size,
                                  data,
                                  (const char *)data);
    
    if (result) {
        sqlite3_free(result);
    }
    
    free(fmt);
}

// Variadic helper function for sqlite3_str_vappendf
static void call_sqlite3_str_vappendf(sqlite3_str *str, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(str, fmt, args);
    va_end(args);
}

static void test_sqlite3_str_functions(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create a sqlite3_str object
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return;
    
    // Create format string from input
    size_t fmt_len = size > 100 ? 100 : size - 1;
    char *fmt = (char *)malloc(fmt_len + 1);
    if (!fmt) {
        sqlite3_str_finish(str);
        return;
    }
    memcpy(fmt, data + 1, fmt_len);
    fmt[fmt_len] = '\0';
    
    // Test sqlite3_str_appendf
    sqlite3_str_appendf(str, fmt, (int)size, (double)size);
    
    // Test sqlite3_str_vappendf via helper
    call_sqlite3_str_vappendf(str, fmt, (int)size, (double)size);
    
    // Get the result and free it
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }
    
    free(fmt);
}

static void test_sqlite3_log(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create format string from input
    size_t fmt_len = size > 100 ? 100 : size - 1;
    char *fmt = (char *)malloc(fmt_len + 1);
    if (!fmt) return;
    memcpy(fmt, data + 1, fmt_len);
    fmt[fmt_len] = '\0';
    
    // Call sqlite3_log with various error codes
    int err_code = data[0] % 256;
    sqlite3_log(err_code, fmt, (int)size, (const char *)data);
    
    free(fmt);
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Initialize SQLite if needed
    static int initialized = 0;
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    // Seed random number generator based on input
    unsigned int seed = 0;
    for (size_t i = 0; i < Size && i < sizeof(seed); i++) {
        seed = (seed << 8) | Data[i];
    }
    srand(seed);
    
    // Choose which functions to test based on input
    int choice = Data[0] % 5;
    
    switch (choice) {
        case 0:
            test_sqlite3_snprintf(Data, Size);
            break;
        case 1:
            test_sqlite3_vmprintf(Data, Size);
            break;
        case 2:
            test_sqlite3_mprintf(Data, Size);
            break;
        case 3:
            test_sqlite3_str_functions(Data, Size);
            break;
        case 4:
            test_sqlite3_log(Data, Size);
            break;
    }
    
    return 0;
}