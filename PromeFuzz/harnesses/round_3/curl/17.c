// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mprintf.h"
#include <stdarg.h>

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

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Prepare buffers and strings from fuzzer input
    char buffer[1024];
    char format[256];
    char arg_str[256];
    
    // Split input for format string and argument
    size_t format_len = Size > 256 ? 256 : Size;
    size_t arg_len = Size > 256 ? (Size - 256 > 256 ? 256 : Size - 256) : 0;
    
    memcpy(format, Data, format_len);
    format[format_len] = '\0';
    
    if (arg_len > 0) {
        memcpy(arg_str, Data + format_len, arg_len);
        arg_str[arg_len] = '\0';
    } else {
        strcpy(arg_str, "test");
    }
    
    // Test curl_mprintf
    curl_mprintf("%s", arg_str);
    
    // Test curl_msnprintf
    int maxlen = (int)(Size % 1024) + 1;
    curl_msnprintf(buffer, maxlen, "%s", arg_str);
    
    // Test curl_mfprintf
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        curl_mfprintf(fp, "%s", arg_str);
        fclose(fp);
    }
    
    // Test curl_msprintf
    curl_msprintf(buffer, "%s", arg_str);
    
    // Test curl_mvsnprintf using helper
    test_mvsnprintf("%s", arg_str);
    
    // Test curl_mvsprintf using helper
    test_mvsprintf("%s", arg_str);
    
    return 0;
}