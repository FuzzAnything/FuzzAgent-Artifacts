// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdarg.h>

static void test_mvsprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_mvsnprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create format string from input data
    char format[256];
    size_t format_len = Size < sizeof(format) ? Size : sizeof(format) - 1;
    memcpy(format, Data, format_len);
    format[format_len] = '\0';

    // Create buffer for output
    char buffer[1024];
    
    // Test curl_mvsprintf via helper function
    test_mvsprintf(format, 42, "test", 3.14);

    // Test curl_mfprintf
    {
        FILE *fp = fopen("./dummy_file", "w");
        if (fp) {
            curl_mfprintf(fp, format, 42, "test", 3.14);
            fclose(fp);
        }
    }

    // Test curl_mvsnprintf via helper function
    test_mvsnprintf(format, 42, "test", 3.14);

    // Test curl_msnprintf
    {
        curl_msnprintf(buffer, sizeof(buffer), format, 42, "test", 3.14);
    }

    // Test curl_mprintf
    {
        curl_mprintf(format, 42, "test", 3.14);
    }

    // Test curl_msprintf
    {
        curl_msprintf(buffer, format, 42, "test", 3.14);
    }

    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}