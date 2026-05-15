// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:19373:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void call_vsnprintf_wrapper(int n, char *buf, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sqlite3_vsnprintf(n, buf, format, args);
    va_end(args);
}

static char* call_vmprintf_wrapper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

static void call_str_vappendf_wrapper(sqlite3_str *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sqlite3_str_vappendf(str, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Use first byte to determine format string type
    uint8_t format_type = Data[0] % 6;
    const char *format_strings[] = {
        "%s",
        "%.*s",
        "%d %s",
        "%q",
        "%Q",
        "%z"
    };
    
    const char *format = format_strings[format_type];
    
    // Use remaining data as string argument
    size_t str_len = Size - 1;
    const char *str_arg = (const char *)(Data + 1);
    
    // Create a null-terminated copy for functions that need it
    char *null_term_str = malloc(str_len + 1);
    if (!null_term_str) return 0;
    memcpy(null_term_str, str_arg, str_len);
    null_term_str[str_len] = '\0';
    
    // Test sqlite3_snprintf
    char buffer[256];
    int int_arg = (int)str_len;
    
    switch (format_type) {
        case 0: // %s
            sqlite3_snprintf(sizeof(buffer), buffer, format, null_term_str);
            break;
        case 1: // %.*s
            sqlite3_snprintf(sizeof(buffer), buffer, format, int_arg, str_arg);
            break;
        case 2: // %d %s
            sqlite3_snprintf(sizeof(buffer), buffer, format, int_arg, null_term_str);
            break;
        default:
            sqlite3_snprintf(sizeof(buffer), buffer, format, null_term_str);
            break;
    }
    
    // Test sqlite3_mprintf
    char *mprintf_result = NULL;
    switch (format_type) {
        case 0: // %s
            mprintf_result = sqlite3_mprintf(format, null_term_str);
            break;
        case 1: // %.*s
            mprintf_result = sqlite3_mprintf(format, int_arg, str_arg);
            break;
        case 2: // %d %s
            mprintf_result = sqlite3_mprintf(format, int_arg, null_term_str);
            break;
        default:
            mprintf_result = sqlite3_mprintf(format, null_term_str);
            break;
    }
    if (mprintf_result) {
        sqlite3_free(mprintf_result);
    }
    
    // Test sqlite3_vmprintf using wrapper
    char *vmprintf_result = NULL;
    switch (format_type) {
        case 0: // %s
            vmprintf_result = call_vmprintf_wrapper(format, null_term_str);
            break;
        case 1: // %.*s
            vmprintf_result = call_vmprintf_wrapper(format, int_arg, str_arg);
            break;
        case 2: // %d %s
            vmprintf_result = call_vmprintf_wrapper(format, int_arg, null_term_str);
            break;
        default:
            vmprintf_result = call_vmprintf_wrapper(format, null_term_str);
            break;
    }
    if (vmprintf_result) {
        sqlite3_free(vmprintf_result);
    }
    
    // Test sqlite3_str_appendf
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (str_obj) {
        switch (format_type) {
            case 0: // %s
                sqlite3_str_appendf(str_obj, format, null_term_str);
                break;
            case 1: // %.*s
                sqlite3_str_appendf(str_obj, format, int_arg, str_arg);
                break;
            case 2: // %d %s
                sqlite3_str_appendf(str_obj, format, int_arg, null_term_str);
                break;
            default:
                sqlite3_str_appendf(str_obj, format, null_term_str);
                break;
        }
        
        // Test sqlite3_str_vappendf using wrapper
        switch (format_type) {
            case 0: // %s
                call_str_vappendf_wrapper(str_obj, format, null_term_str);
                break;
            case 1: // %.*s
                call_str_vappendf_wrapper(str_obj, format, int_arg, str_arg);
                break;
            case 2: // %d %s
                call_str_vappendf_wrapper(str_obj, format, int_arg, null_term_str);
                break;
            default:
                call_str_vappendf_wrapper(str_obj, format, null_term_str);
                break;
        }
        
        char *str_finish_result = sqlite3_str_finish(str_obj);
        if (str_finish_result) {
            sqlite3_free(str_finish_result);
        }
    }
    
    // Test sqlite3_vsnprintf using wrapper
    char vsnprintf_buffer[256];
    switch (format_type) {
        case 0: // %s
            call_vsnprintf_wrapper(sizeof(vsnprintf_buffer), vsnprintf_buffer, format, null_term_str);
            break;
        case 1: // %.*s
            call_vsnprintf_wrapper(sizeof(vsnprintf_buffer), vsnprintf_buffer, format, int_arg, str_arg);
            break;
        case 2: // %d %s
            call_vsnprintf_wrapper(sizeof(vsnprintf_buffer), vsnprintf_buffer, format, int_arg, null_term_str);
            break;
        default:
            call_vsnprintf_wrapper(sizeof(vsnprintf_buffer), vsnprintf_buffer, format, null_term_str);
            break;
    }
    
    free(null_term_str);
    return 0;
}