// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void dummy_log_callback(void *pArg, int iErrCode, const char *zMsg) {
    // Do nothing - just a dummy callback for sqlite3_log
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
}

static void test_vmprintf_wrapper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = sqlite3_vmprintf(format, args);
    if (result) {
        sqlite3_free(result);
    }
    va_end(args);
}

static void test_str_vappendf_wrapper(sqlite3_str *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sqlite3_str_vappendf(str, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create a dummy file for any potential file operations
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    // Use the fuzzer data to create format strings and arguments
    // We'll use the first part of data for format string, rest for arguments
    
    // Extract format string (up to 100 bytes or until null byte)
    char format[101] = {0};
    size_t format_len = Size > 100 ? 100 : Size;
    if (format_len > 0) {
        memcpy(format, Data, format_len);
        format[format_len] = '\0';
    } else {
        format[0] = '\0';
    }
    
    // Extract argument string (if any remaining data)
    const char *arg_str = NULL;
    char arg_buffer[101] = {0};
    if (Size > format_len) {
        size_t arg_len = Size - format_len;
        if (arg_len > 100) arg_len = 100;
        memcpy(arg_buffer, Data + format_len, arg_len);
        arg_buffer[arg_len] = '\0';
        arg_str = arg_buffer;
    }
    
    // Test sqlite3_vmprintf using wrapper
    if (format_len > 0) {
        test_vmprintf_wrapper(format, arg_str);
        test_vmprintf_wrapper(format, 123, 456.789, arg_str);
    }
    
    // Test sqlite3_vmprintf with NULL format
    {
        char *result = sqlite3_vmprintf(NULL, NULL);
        if (result) {
            sqlite3_free(result);
        }
    }
    
    // Test sqlite3_snprintf
    char buffer[256];
    if (format_len > 0 && arg_str) {
        sqlite3_snprintf(sizeof(buffer), buffer, format, arg_str);
    }
    
    // Test sqlite3_snprintf with various buffer sizes
    if (format_len > 0) {
        sqlite3_snprintf(1, buffer, format);  // Very small buffer
        sqlite3_snprintf(sizeof(buffer), buffer, format, 123, 456.789, arg_str);
    }
    
    // Test sqlite3_str_vappendf using wrapper
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (str_obj) {
        if (format_len > 0) {
            test_str_vappendf_wrapper(str_obj, format, arg_str);
            test_str_vappendf_wrapper(str_obj, format, 123, 456.789);
        }
        
        // Also test with NULL format
        sqlite3_str_vappendf(str_obj, NULL, NULL);
        
        char *str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    // Test sqlite3_log
    // First set up a dummy log callback
    sqlite3_config(SQLITE_CONFIG_LOG, dummy_log_callback, NULL);
    
    if (format_len > 0) {
        sqlite3_log(SQLITE_OK, format, arg_str);
        sqlite3_log(SQLITE_ERROR, format, 123, 456.789);
    }
    
    // Test sqlite3_str_appendf
    sqlite3_str *str_obj2 = sqlite3_str_new(NULL);
    if (str_obj2) {
        if (format_len > 0 && arg_str) {
            sqlite3_str_appendf(str_obj2, format, arg_str);
        }
        
        if (format_len > 0) {
            sqlite3_str_appendf(str_obj2, format, 123, 456.789);
        }
        
        char *str_result2 = sqlite3_str_finish(str_obj2);
        if (str_result2) {
            sqlite3_free(str_result2);
        }
    }
    
    // Test sqlite3_mprintf
    if (format_len > 0 && arg_str) {
        char *result3 = sqlite3_mprintf(format, arg_str);
        if (result3) {
            sqlite3_free(result3);
        }
    }
    
    if (format_len > 0) {
        char *result4 = sqlite3_mprintf(format, 123, 456.789, arg_str);
        if (result4) {
            sqlite3_free(result4);
        }
    }
    
    // Test sqlite3_mprintf with NULL format (should handle gracefully)
    char *result5 = sqlite3_mprintf(NULL);
    if (result5) {
        sqlite3_free(result5);
    }
    
    // Cleanup
    sqlite3_shutdown();
    
    return 0;
}