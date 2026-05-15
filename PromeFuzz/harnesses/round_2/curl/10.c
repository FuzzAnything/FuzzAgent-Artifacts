// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdarg.h>
#include <unistd.h>

static void helper_mvfprintf(const char *format, ...) {
    FILE *fd = fopen("./dummy_file", "w");
    if (!fd) return;
    
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
    
    fclose(fd);
    unlink("./dummy_file");
}

static void helper_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void helper_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void test_curl_mvfprintf(const uint8_t *data, size_t size) {
    helper_mvfprintf("%.*s", (int)size, data);
}

static void test_curl_mvsprintf(const uint8_t *data, size_t size) {
    helper_mvsprintf("%.*s", (int)size, data);
}

static void test_curl_mprintf(const uint8_t *data, size_t size) {
    curl_mprintf("%.*s", (int)size, data);
}

static void test_curl_mfprintf(const uint8_t *data, size_t size) {
    FILE *fd = fopen("./dummy_file", "w");
    if (!fd) return;
    
    curl_mfprintf(fd, "%.*s", (int)size, data);
    
    fclose(fd);
    unlink("./dummy_file");
}

static void test_curl_mvprintf(const uint8_t *data, size_t size) {
    helper_mvprintf("%.*s", (int)size, data);
}

static void test_curl_msprintf(const uint8_t *data, size_t size) {
    char buffer[1024];
    curl_msprintf(buffer, "%.*s", (int)size, data);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for file-based operations
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        fwrite(Data, 1, Size, dummy);
        fclose(dummy);
    }
    
    // Test all target functions
    test_curl_mvfprintf(Data, Size);
    test_curl_mvsprintf(Data, Size);
    test_curl_mprintf(Data, Size);
    test_curl_mfprintf(Data, Size);
    test_curl_mvprintf(Data, Size);
    test_curl_msprintf(Data, Size);
    
    return 0;
}