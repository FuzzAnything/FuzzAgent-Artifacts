// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdarg.h>

static void dummy_vprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void dummy_printf(const char *format, ...) {
    curl_mprintf(format, 42, 3.14, "test");
}

static void dummy_fprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

static void dummy_sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_mvfprintf(const char *format, ...) {
    FILE *fd = fopen("./dummy_file", "w");
    if (fd) {
        va_list args;
        va_start(args, format);
        curl_mvfprintf(fd, format, args);
        va_end(args);
        fclose(fd);
    }
}

static void test_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated format string from fuzzer input
    char *format = malloc(Size + 1);
    if (!format) {
        return 0;
    }
    memcpy(format, Data, Size);
    format[Size] = '\0';

    // Buffer for sprintf functions
    char buffer[1024];

    // Test curl_mvsprintf via helper
    test_mvsprintf(format, 42, 3.14, "test");

    // Test curl_mvfprintf via helper
    test_mvfprintf(format, 42, 3.14, "test");

    // Test curl_mfprintf
    {
        FILE *fd = fopen("./dummy_file", "w");
        if (fd) {
            curl_mfprintf(fd, format, 42, 3.14, "test");
            fclose(fd);
        }
    }

    // Test curl_mprintf
    curl_mprintf(format, 42, 3.14, "test");

    // Test curl_mvprintf via helper
    test_mvprintf(format, 42, 3.14, "test");

    // Test curl_msprintf
    curl_msprintf(buffer, format, 42, 3.14, "test");

    // Test helper functions with different argument combinations
    dummy_vprintf(format);
    dummy_printf(format);
    
    FILE *fd = fopen("./dummy_file", "w");
    if (fd) {
        dummy_fprintf(fd, format);
        fclose(fd);
    }
    
    dummy_sprintf(buffer, format);

    free(format);
    return 0;
}