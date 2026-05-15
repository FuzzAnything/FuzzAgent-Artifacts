// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:19373:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void test_vsnprintf_helper(int n, char *buf, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sqlite3_vsnprintf(n, buf, format, args);
    va_end(args);
}

static char *test_vmprintf_helper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

static void test_str_vappendf_helper(sqlite3_str *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    sqlite3_str_vappendf(str, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Test sqlite3_snprintf
    {
        char buffer[256];
        const char *format = "Test %.*s %d";
        int n = (Data[0] % 128) + 1;
        if (n > sizeof(buffer)) n = sizeof(buffer);
        
        int arg1 = (int)(Size > 0 ? Data[0] : 0);
        const char *arg2 = (const char*)Data;
        size_t arg2_len = Size > 100 ? 100 : Size;
        
        sqlite3_snprintf(n, buffer, format, arg1, arg2_len, arg2, 42);
    }
    
    // Test sqlite3_vsnprintf using helper
    {
        char buffer[256];
        const char *format = "Test %.*s %d";
        int n = (Data[0] % 128) + 1;
        if (n > sizeof(buffer)) n = sizeof(buffer);
        
        size_t arg_len = Size > 50 ? 50 : Size;
        int num = (int)(Size > 1 ? Data[1] : 0);
        
        test_vsnprintf_helper(n, buffer, format, arg_len, Data, num);
    }
    
    // Test sqlite3_vmprintf using helper
    {
        const char *format = "Test %.*s %llu";
        size_t arg_len = Size > 30 ? 30 : Size;
        unsigned long long num = (unsigned long long)Size;
        
        char *result = test_vmprintf_helper(format, arg_len, Data, num);
        if (result) {
            sqlite3_free(result);
        }
    }
    
    // Test sqlite3_mprintf
    {
        const char *format = "Test %.*s %llu";
        size_t arg_len = Size > 20 ? 20 : Size;
        unsigned long long num = (unsigned long long)Size;
        
        char *result = sqlite3_mprintf(format, arg_len, Data, num);
        if (result) {
            sqlite3_free(result);
        }
    }
    
    // Test sqlite3_str_vappendf and sqlite3_str_appendf
    {
        sqlite3_str *str = sqlite3_str_new(NULL);
        if (str) {
            const char *format1 = "Prefix %.*s";
            size_t arg_len1 = Size > 10 ? 10 : Size;
            
            // Test sqlite3_str_appendf
            sqlite3_str_appendf(str, format1, arg_len1, Data);
            
            const char *format2 = " Suffix %d";
            int num = (int)(Size > 0 ? Data[0] : 0);
            
            // Test sqlite3_str_vappendf using helper
            test_str_vappendf_helper(str, format2, num);
            
            char *result = sqlite3_str_finish(str);
            if (result) {
                sqlite3_free(result);
            }
        }
    }
    
    // Clean up
    remove("./dummy_file");
    sqlite3_shutdown();
    
    return 0;
}