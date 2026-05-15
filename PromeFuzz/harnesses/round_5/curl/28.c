// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
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

static void dummy_vprintf(const char *format, ...) {
    // Helper function to call curl_mvprintf with va_list
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void dummy_vfprintf(FILE *fd, const char *format, ...) {
    // Helper function to call curl_mvfprintf with va_list
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

static void dummy_vsprintf(char *buffer, const char *format, ...) {
    // Helper function to call curl_mvsprintf with va_list
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a null-terminated format string from fuzzer input
    char *format = (char*)malloc(Size + 1);
    if (!format) return 0;
    memcpy(format, Data, Size);
    format[Size] = '\0';
    
    // Prepare buffer for sprintf functions
    char buffer[1024];
    
    // Prepare file for fprintf functions
    FILE *fd = fopen("./dummy_file", "w");
    if (!fd) {
        free(format);
        return 0;
    }
    
    // Test curl_mprintf
    curl_mprintf("%s", format);
    
    // Test curl_mvprintf
    dummy_vprintf("%s", format);
    
    // Test curl_msprintf
    curl_msprintf(buffer, "%s", format);
    
    // Test curl_mvsprintf
    dummy_vsprintf(buffer, "%s", format);
    
    // Test curl_mfprintf
    curl_mfprintf(fd, "%s", format);
    
    // Test curl_mvfprintf
    dummy_vfprintf(fd, "%s", format);
    
    // Cleanup
    fclose(fd);
    free(format);
    
    return 0;
}