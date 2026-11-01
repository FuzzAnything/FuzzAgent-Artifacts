// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3 string formatting functions:
 * - sqlite3_mprintf
 * - sqlite3_snprintf
 * - sqlite3_str_vappendf
 * - sqlite3_vsnprintf
 * - sqlite3_vmprintf
 * - sqlite3_log
 */

static void safe_free(void *ptr) {
    if (ptr) {
        sqlite3_free(ptr);
    }
}

static int fuzzer_init(void) {
    /* Initialize SQLite if not already done */
    if (sqlite3_initialize() != SQLITE_OK) {
        return -1;
    }
    return 0;
}

/* Helper function to test va_list based functions with variadic args */
static void test_va_list_functions(const char *format, ...) {
    va_list args;
    va_list args_copy;
    char buffer[256];
    char vsnprintf_buffer[256];

    va_start(args, format);
    va_copy(args_copy, args);

    /* Test sqlite3_str_vappendf */
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str) {
        va_copy(args_copy, args);
        sqlite3_str_vappendf(str, format, args_copy);
        va_end(args_copy);
        
        char *str_result = sqlite3_str_finish(str);
        if (str_result) {
            safe_free(str_result);
        }
    }

    /* Test sqlite3_vsnprintf */
    va_copy(args_copy, args);
    char *vsnprintf_result = sqlite3_vsnprintf(sizeof(vsnprintf_buffer), vsnprintf_buffer, format, args_copy);
    va_end(args_copy);
    if (vsnprintf_result) {
        (void)vsnprintf_result;
    }

    /* Test sqlite3_vmprintf */
    va_copy(args_copy, args);
    char *vmprintf_result = sqlite3_vmprintf(format, args_copy);
    va_end(args_copy);
    if (vmprintf_result) {
        safe_free(vmprintf_result);
    }

    va_end(args);
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    if (fuzzer_init() != 0) {
        return 0;
    }

    /* Use a portion of the input as a format string */
    const char *format_start = (const char *)Data;
    size_t format_len = 0;
    
    /* Find a null terminator or use the whole buffer if none exists */
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0) {
            format_len = i;
            break;
        }
    }
    if (format_len == 0) {
        format_len = Size;
    }
    
    /* Ensure format string is valid (at least 1 byte) */
    if (format_len == 0) {
        return 0;
    }

    /* Create a safe format string with a null terminator */
    char *safe_format = (char *)sqlite3_malloc64(format_len + 1);
    if (!safe_format) {
        return 0;
    }
    memcpy(safe_format, Data, format_len);
    safe_format[format_len] = '\0';

    /* Test sqlite3_mprintf */
    char *mprintf_result = sqlite3_mprintf(safe_format, 123, 45.6, "test");
    if (mprintf_result) {
        safe_free(mprintf_result);
    }

    /* Test sqlite3_snprintf */
    char buffer[256];
    char *snprintf_result = sqlite3_snprintf(sizeof(buffer), buffer, safe_format, 123, 45.6, "test");
    if (snprintf_result) {
        (void)snprintf_result;
    }

    /* Test va_list based functions via variadic wrapper */
    test_va_list_functions(safe_format, 123, 45.6, "test");

    /* Test sqlite3_log */
    sqlite3_log(SQLITE_ERROR, safe_format, 123, 45.6, "test");

    /* Cleanup */
    safe_free(safe_format);

    return 0;
}