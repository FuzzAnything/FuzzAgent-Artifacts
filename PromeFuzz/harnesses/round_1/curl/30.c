// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static void dummy_vprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void dummy_vsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void dummy_vsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
}

static void dummy_vfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';
    
    // Test curl_mprintf with various format strings
    curl_mprintf("%s", input_str);
    curl_mprintf("%.*s", (int)(Size % 256), input_str);
    
    // Test curl_msprintf with different buffer sizes
    size_t buffer_size = Size + 100;
    char *buffer = (char *)malloc(buffer_size);
    if (buffer) {
        curl_msprintf(buffer, "%s", input_str);
        curl_msprintf(buffer, "Prefix: %s", input_str);
        curl_msprintf(buffer, "%.*s", (int)(Size % 128), input_str);
        free(buffer);
    }
    
    // Test curl_mvsprintf using helper function
    buffer = (char *)malloc(buffer_size);
    if (buffer) {
        dummy_vsprintf(buffer, "%s", input_str);
        dummy_vsprintf(buffer, "Test: %s End", input_str);
        free(buffer);
    }
    
    // Test curl_mvprintf using helper function
    dummy_vprintf("%s\n", input_str);
    dummy_vprintf("Input: %s\n", input_str);
    
    // Test curl_mvsnprintf with different maxlength values
    buffer = (char *)malloc(buffer_size);
    if (buffer) {
        dummy_vsnprintf(buffer, Size % 256, "%s", input_str);
        dummy_vsnprintf(buffer, buffer_size, "Formatted: %s", input_str);
        dummy_vsnprintf(buffer, 10, "%.*s", 5, input_str);
        free(buffer);
    }
    
    // Test curl_mvfprintf with a file
    FILE *file = fopen("./dummy_file", "w");
    if (file) {
        dummy_vfprintf(file, "%s", input_str);
        dummy_vfprintf(file, "File content: %s\n", input_str);
        fclose(file);
    }
    
    // Clean up
    free(input_str);
    
    return 0;
}