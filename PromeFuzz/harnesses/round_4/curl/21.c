// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/mprintf.h"
#include <stdarg.h>

#define MAX_BUFFER_SIZE 4096
#define MAX_FORMAT_SIZE 256

static void test_mvsprintf_wrapper(const char *format, ...) {
    char buffer[MAX_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_mvprintf_wrapper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void test_mvfprintf_wrapper(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Limit input size to prevent excessive memory usage
    size_t usable_size = Size > MAX_BUFFER_SIZE ? MAX_BUFFER_SIZE : Size;
    
    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(usable_size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, usable_size);
    input_str[usable_size] = '\0';
    
    // Create a format string - use a simple format that won't cause crashes
    // but still exercises the formatting code
    char format[MAX_FORMAT_SIZE];
    snprintf(format, sizeof(format), "Test: %%.%zus", usable_size % 100);
    
    // Test curl_msprintf
    {
        char buffer[MAX_BUFFER_SIZE];
        curl_msprintf(buffer, "%s", input_str);
        
        // Test with format string
        if (usable_size > 10) {
            curl_msprintf(buffer, format, input_str);
        }
    }
    
    // Test curl_mprintf
    {
        curl_mprintf("%s", input_str);
        
        if (usable_size > 10) {
            curl_mprintf(format, input_str);
        }
    }
    
    // Test curl_mfprintf
    {
        FILE *dummy_file = fopen("./dummy_file", "w");
        if (dummy_file) {
            curl_mfprintf(dummy_file, "%s", input_str);
            
            if (usable_size > 10) {
                curl_mfprintf(dummy_file, format, input_str);
            }
            fclose(dummy_file);
        }
    }
    
    // Test curl_mvsprintf via wrapper
    {
        test_mvsprintf_wrapper("%s", input_str);
        
        if (usable_size > 10) {
            test_mvsprintf_wrapper(format, input_str);
        }
    }
    
    // Test curl_mvprintf via wrapper
    {
        test_mvprintf_wrapper("%s", input_str);
        
        if (usable_size > 10) {
            test_mvprintf_wrapper(format, input_str);
        }
    }
    
    // Test curl_mvfprintf via wrapper
    {
        FILE *dummy_file = fopen("./dummy_file", "w");
        if (dummy_file) {
            test_mvfprintf_wrapper(dummy_file, "%s", input_str);
            
            if (usable_size > 10) {
                test_mvfprintf_wrapper(dummy_file, format, input_str);
            }
            fclose(dummy_file);
        }
    }
    
    free(input_str);
    return 0;
}