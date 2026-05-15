// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "curl/curl.h"
#include "curl/mprintf.h"

static void dummy_va_function(const char *format, ...) {
    // This function exists only to create a va_list for testing
    return;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test curl_getdate
    {
        char *date_str = (char *)malloc(Size + 1);
        if (date_str) {
            memcpy(date_str, Data, Size);
            date_str[Size] = '\0';
            time_t result = curl_getdate(date_str, NULL);
            (void)result; // Use result to avoid unused variable warning
            free(date_str);
        }
    }

    // Test curl_maprintf
    {
        char *format_str = (char *)malloc(Size + 1);
        if (format_str) {
            memcpy(format_str, Data, Size);
            format_str[Size] = '\0';
            
            // Create a simple format string test
            char *result = curl_maprintf("%s test", format_str);
            if (result) {
                curl_free(result);
            }
            free(format_str);
        }
    }

    // Test curl_getenv
    {
        char *env_var = (char *)malloc(Size + 1);
        if (env_var) {
            memcpy(env_var, Data, Size);
            env_var[Size] = '\0';
            
            char *result = curl_getenv(env_var);
            if (result) {
                curl_free(result);
            }
            free(env_var);
        }
    }

    // Test curl_mvsprintf
    {
        char buffer[1024];
        char *format_str = (char *)malloc(Size + 1);
        if (format_str && Size < 512) { // Limit size to avoid buffer overflow
            memcpy(format_str, Data, Size);
            format_str[Size] = '\0';
            
            va_list args;
            dummy_va_function("%s", format_str);
            
            // Note: We can't safely pass arbitrary format strings with va_list
            // So we use a simple test case
            int result = curl_mvsprintf(buffer, "Test: %s", args);
            (void)result; // Use result to avoid unused variable warning
            
            free(format_str);
        }
    }

    // Test curl_mvaprintf
    {
        char *format_str = (char *)malloc(Size + 1);
        if (format_str && Size < 512) {
            memcpy(format_str, Data, Size);
            format_str[Size] = '\0';
            
            va_list args;
            dummy_va_function("%s", format_str);
            
            char *result = curl_mvaprintf("Format: %s", args);
            if (result) {
                curl_free(result);
            }
            free(format_str);
        }
    }

    // Test curl_free with NULL (should be safe)
    curl_free(NULL);

    return 0;
}