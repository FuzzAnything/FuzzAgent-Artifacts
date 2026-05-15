// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include "curl/curl.h"
#include <stdarg.h>
#include <time.h>

static void test_va_functions(const char *format, ...) {
    va_list args;
    char buffer[1024];
    char small_buffer[16];
    
    // Test curl_mvsprintf
    va_start(args, format);
    int ret1 = curl_mvsprintf(buffer, format, args);
    va_end(args);
    (void)ret1;
    
    // Test curl_mvsnprintf with different buffer sizes
    va_start(args, format);
    int ret2 = curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    (void)ret2;
    
    // Test with small buffer to trigger truncation
    va_start(args, format);
    int ret3 = curl_mvsnprintf(small_buffer, sizeof(small_buffer), format, args);
    va_end(args);
    (void)ret3;
    
    // Test curl_mvaprintf
    va_start(args, format);
    char *aprintf_result = curl_mvaprintf(format, args);
    va_end(args);
    if (aprintf_result) {
        curl_free(aprintf_result);
    }
}

static void test_mvprintf_wrapper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvprintf(format, args);
    va_end(args);
    (void)ret;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_getdate
    time_t result = curl_getdate(input_str, NULL);
    (void)result;

    // Test curl_maprintf with various format strings
    char *formatted = NULL;
    
    // Test with simple string format
    formatted = curl_maprintf("%s", input_str);
    if (formatted) {
        curl_free(formatted);
    }
    
    // Test with integer format (use first byte as integer)
    if (Size > 0) {
        formatted = curl_maprintf("Number: %d", (int)Data[0]);
        if (formatted) {
            curl_free(formatted);
        }
    }
    
    // Test with multiple format specifiers
    if (Size > 1) {
        formatted = curl_maprintf("String: %s, Int: %d, Char: %c", 
                                  input_str, (int)Data[0], (char)Data[1 % Size]);
        if (formatted) {
            curl_free(formatted);
        }
    }

    // Test va_list functions with different format strings
    test_va_functions("%s", input_str);
    
    if (Size > 0) {
        test_va_functions("%d", (int)Data[0]);
    }
    
    if (Size > 1) {
        test_va_functions("String: %s, Int: %d", input_str, (int)Data[0]);
    }

    // Test curl_mvprintf (outputs to stdout, but we can't avoid it)
    // We'll only call it occasionally to avoid flooding output
    if (Size > 0 && Data[0] % 100 == 0) {
        test_mvprintf_wrapper("Fuzz: %s\n", input_str);
    }

    // Test with file-based operations if needed
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        
        // Additional tests could use the file here
        
        // Clean up
        remove("./dummy_file");
    }

    // Test with empty format string
    formatted = curl_maprintf("");
    if (formatted) {
        curl_free(formatted);
    }

    free(input_str);
    return 0;
}