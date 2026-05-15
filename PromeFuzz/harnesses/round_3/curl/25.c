// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
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

static void test_curl_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void test_curl_mvfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

static void test_curl_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare a format string from the input data
    char *format = malloc(Size + 1);
    if (!format) {
        return 0;
    }
    memcpy(format, Data, Size);
    format[Size] = '\0';

    // Prepare buffer for sprintf functions
    char buffer[1024];
    
    // Prepare FILE* for fprintf functions
    FILE *fd = fopen("./dummy_file", "w");
    if (!fd) {
        free(format);
        return 0;
    }

    // Test curl_mprintf
    curl_mprintf("%s", format);

    // Test curl_mfprintf
    curl_mfprintf(fd, "%s", format);
    fflush(fd);

    // Test curl_msprintf
    curl_msprintf(buffer, "%s", format);

    // Test curl_mvprintf using helper function
    test_curl_mvprintf("%s", format);

    // Test curl_mvfprintf using helper function
    test_curl_mvfprintf(fd, "%s", format);
    fflush(fd);

    // Test curl_mvsprintf using helper function
    test_curl_mvsprintf(buffer, "%s", format);

    // Cleanup
    fclose(fd);
    free(format);
    
    return 0;
}

#ifdef __cplusplus
}
#endif