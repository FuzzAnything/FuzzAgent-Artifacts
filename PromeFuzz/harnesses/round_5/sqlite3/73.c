// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:19373:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void test_vsnprintf_wrapper(const char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    sqlite3_vsnprintf(sizeof(buffer), buffer, format, args);
    va_end(args);
}

static void test_str_vappendf_wrapper(const char *format, ...) {
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (str_obj) {
        va_list args;
        va_start(args, format);
        sqlite3_str_vappendf(str_obj, format, args);
        va_end(args);
        char *result = sqlite3_str_finish(str_obj);
        if (result) {
            sqlite3_free(result);
        }
    }
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

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Extract format string from input
    size_t format_len = Size > 100 ? 100 : Size;
    char *format_str = (char *)malloc(format_len + 1);
    if (!format_str) return 0;
    
    memcpy(format_str, Data, format_len);
    format_str[format_len] = '\0';
    
    // Extract some arguments from input
    int int_arg = 0;
    double double_arg = 0.0;
    char *str_arg = NULL;
    
    if (Size >= format_len + sizeof(int)) {
        memcpy(&int_arg, Data + format_len, sizeof(int));
    }
    
    if (Size >= format_len + sizeof(int) + sizeof(double)) {
        memcpy(&double_arg, Data + format_len + sizeof(int), sizeof(double));
    }
    
    if (Size >= format_len + sizeof(int) + sizeof(double) + 10) {
        size_t str_len = Size - (format_len + sizeof(int) + sizeof(double));
        str_len = str_len > 50 ? 50 : str_len;
        str_arg = (char *)malloc(str_len + 1);
        if (str_arg) {
            memcpy(str_arg, Data + format_len + sizeof(int) + sizeof(double), str_len);
            str_arg[str_len] = '\0';
        }
    }
    
    // Test sqlite3_vsnprintf using wrapper
    test_vsnprintf_wrapper(format_str, int_arg, double_arg, str_arg ? str_arg : "");
    
    // Test sqlite3_str_vappendf using wrapper
    test_str_vappendf_wrapper(format_str, int_arg, double_arg, str_arg ? str_arg : "");
    
    // Test sqlite3_snprintf
    {
        char buffer[256];
        if (str_arg) {
            sqlite3_snprintf(sizeof(buffer), buffer, format_str, int_arg, double_arg, str_arg);
        } else {
            sqlite3_snprintf(sizeof(buffer), buffer, format_str, int_arg, double_arg);
        }
    }
    
    // Test sqlite3_str_appendf
    {
        sqlite3_str *str_obj = sqlite3_str_new(NULL);
        if (str_obj) {
            if (str_arg) {
                sqlite3_str_appendf(str_obj, format_str, int_arg, double_arg, str_arg);
            } else {
                sqlite3_str_appendf(str_obj, format_str, int_arg, double_arg);
            }
            char *result = sqlite3_str_finish(str_obj);
            if (result) {
                sqlite3_free(result);
            }
        }
    }
    
    // Test sqlite3_mprintf
    {
        char *result = NULL;
        if (str_arg) {
            result = sqlite3_mprintf(format_str, int_arg, double_arg, str_arg);
        } else {
            result = sqlite3_mprintf(format_str, int_arg, double_arg);
        }
        if (result) {
            sqlite3_free(result);
        }
    }
    
    // Test sqlite3_vmprintf using wrapper
    test_vmprintf_wrapper(format_str, int_arg, double_arg, str_arg ? str_arg : "");
    
    // Cleanup
    free(format_str);
    if (str_arg) free(str_arg);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}

#ifdef __cplusplus
}
#endif