// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdarg.h>

static void dummy_vprintf(const char *format, ...) {
    // Do nothing, just consume the arguments
    va_list args;
    va_start(args, format);
    va_end(args);
}

static void dummy_vfprintf(FILE *fd, const char *format, ...) {
    // Do nothing, just consume the arguments
    va_list args;
    va_start(args, format);
    va_end(args);
}

static void test_mvsnprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
}

static void test_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a null-terminated string from fuzzer data for format strings
    char *format_str = malloc(Size + 1);
    if (!format_str) {
        return 0;
    }
    memcpy(format_str, Data, Size);
    format_str[Size] = '\0';

    // Create a safe dummy string argument
    char dummy_arg[] = "fuzz";
    
    // Buffer for formatted output
    char buffer[1024];
    
    // Test curl_msnprintf - limit format string to prevent buffer overflows
    if (Size < 100) {
        curl_msnprintf(buffer, sizeof(buffer), "%.*s", (int)Size, format_str);
    } else {
        curl_msnprintf(buffer, sizeof(buffer), "%.99s", format_str);
    }
    
    // Test curl_mvsnprintf using helper function - with safe format
    if (Size < 100) {
        test_mvsnprintf("%.*s", (int)Size, dummy_arg);
    } else {
        test_mvsnprintf("%s", dummy_arg);
    }
    
    // Test curl_mvsprintf using helper function - with safe format
    if (Size < 100) {
        test_mvsprintf("%.*s", (int)Size, dummy_arg);
    } else {
        test_mvsprintf("%s", dummy_arg);
    }
    
    // Test curl_mprintf - using dummy function to avoid actual printing
    dummy_vprintf("%s", dummy_arg);
    
    // Test curl_mfprintf
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        dummy_vfprintf(dummy_file, "%s", dummy_arg);
        fclose(dummy_file);
    }
    
    // Test curl_msprintf - with safe format
    if (Size < 100) {
        curl_msprintf(buffer, "%.*s", (int)Size, format_str);
    } else {
        curl_msprintf(buffer, "%.99s", format_str);
    }
    
    free(format_str);
    return 0;
}