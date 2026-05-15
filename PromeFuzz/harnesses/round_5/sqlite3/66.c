// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

static void dummy_log_callback(void *pArg, int iErrCode, const char *zMsg) {
    // Dummy callback to consume log messages
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
}

static void test_vappendf(sqlite3_str *str, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sqlite3_str_vappendf(str, fmt, args);
    va_end(args);
}

static char *test_vmprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Ensure we have some data to work with
    if (Size < 1) {
        return 0;
    }

    // Initialize SQLite logging if needed
    sqlite3_config(SQLITE_CONFIG_LOG, dummy_log_callback, NULL);
    
    // Create a dummy buffer for various operations
    char buffer[256];
    char *dynamic_str = NULL;
    
    // Use first byte to determine which functions to exercise
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Extract format string from input (ensure null-terminated)
    size_t fmt_len = Size - data_offset;
    if (fmt_len > 100) fmt_len = 100; // Limit format string length
    char fmt_buf[101];
    if (fmt_len > 0) {
        memcpy(fmt_buf, Data + data_offset, fmt_len);
        fmt_buf[fmt_len] = '\0';
    } else {
        strcpy(fmt_buf, "%s");
    }
    
    // Extract argument string from remaining input
    size_t arg_offset = data_offset + fmt_len;
    size_t arg_len = Size - arg_offset;
    if (arg_len > 100) arg_len = 100; // Limit argument length
    char arg_buf[101];
    if (arg_len > 0) {
        memcpy(arg_buf, Data + arg_offset, arg_len);
        arg_buf[arg_len] = '\0';
    } else {
        strcpy(arg_buf, "fuzz");
    }
    
    // Test sqlite3_log with various error codes
    if (selector & 0x01) {
        int err_code = (int)(selector * 37) % 256;
        sqlite3_log(err_code, fmt_buf, arg_buf);
    }
    
    // Test sqlite3_snprintf
    if (selector & 0x02) {
        int buf_size = (int)(selector * 13) % sizeof(buffer);
        if (buf_size < 1) buf_size = 1;
        sqlite3_snprintf(buf_size, buffer, fmt_buf, arg_buf);
    }
    
    // Test sqlite3_mprintf
    if (selector & 0x04) {
        dynamic_str = sqlite3_mprintf(fmt_buf, arg_buf);
        if (dynamic_str) {
            sqlite3_free(dynamic_str);
            dynamic_str = NULL;
        }
    }
    
    // Test sqlite3_str functions
    if (selector & 0x08) {
        sqlite3_str *str_obj = sqlite3_str_new(NULL);
        if (str_obj) {
            // Test sqlite3_str_appendf
            sqlite3_str_appendf(str_obj, fmt_buf, arg_buf);
            
            // Test sqlite3_str_vappendf via helper function
            test_vappendf(str_obj, fmt_buf, arg_buf);
            
            // Clean up
            char *result = sqlite3_str_finish(str_obj);
            if (result) {
                sqlite3_free(result);
            }
        }
    }
    
    // Test sqlite3_vmprintf via helper function
    if (selector & 0x10) {
        dynamic_str = test_vmprintf(fmt_buf, arg_buf);
        
        if (dynamic_str) {
            sqlite3_free(dynamic_str);
            dynamic_str = NULL;
        }
    }
    
    // Additional test with different format specifiers
    if (selector & 0x20) {
        int int_val = (int)selector * 1000;
        double dbl_val = (double)selector / 255.0 * 100.0;
        
        sqlite3_log(SQLITE_OK, "Int: %d, Double: %f, Str: %s", 
                   int_val, dbl_val, arg_buf);
        
        sqlite3_snprintf(sizeof(buffer), buffer, 
                        "Int: %d, Double: %f, Str: %s",
                        int_val, dbl_val, arg_buf);
    }
    
    return 0;
}