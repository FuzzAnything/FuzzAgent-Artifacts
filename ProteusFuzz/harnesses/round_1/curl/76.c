// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void safe_free(void *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

// Helper function to test va_list functions with a variadic interface
static void test_va_functions_wrapper(const char *format, ...) {
    va_list args;
    char buffer[256];
    int ret;

    if (!format) return;

    va_start(args, format);
    
    // Test curl_mvsnprintf
    ret = curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    (void)ret;
    
    // Reset args for next call
    va_end(args);
    va_start(args, format);

    // Test curl_mvsprintf
    // Ensure buffer is large enough to avoid overflow issues in mvsprintf
    ret = curl_mvsprintf(buffer, format, args);
    (void)ret;
    
    va_end(args);
    va_start(args, format);

    // Test curl_mvprintf
    ret = curl_mvprintf(format, args);
    (void)ret;
    
    va_end(args);
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_msnprintf
    char buffer1[256];
    int ret1 = curl_msnprintf(buffer1, sizeof(buffer1), "%s", input_str);
    (void)ret1;

    // Test curl_msnprintf with length specifier
    int ret2 = curl_msnprintf(buffer1, sizeof(buffer1), "%.*s", (int)Size, Data);
    (void)ret2;

    // Test curl_maprintf
    char *allocated1 = curl_maprintf("%s", input_str);
    safe_free(allocated1);

    // Test curl_maprintf with length specifier
    char *allocated2 = curl_maprintf("%.*s", (int)Size, Data);
    safe_free(allocated2);

    // Test va_list functions using the wrapper
    // Only pass valid, non-null format strings
    test_va_functions_wrapper("%s", input_str);
    test_va_functions_wrapper("%.*s", (int)Size, Data);

    // Test curl_unescape with length -1
    char *unescaped1 = curl_unescape(input_str, -1);
    safe_free(unescaped1);

    // Test curl_unescape with specific length
    char *unescaped2 = curl_unescape(input_str, (int)Size);
    safe_free(unescaped2);

    free(input_str);
    return 0;
}