// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a null-terminated format string from the input
    char *format = (char *)malloc(Size + 1);
    if (!format) return 0;
    memcpy(format, Data, Size);
    format[Size] = '\0';
    
    // Create buffer for output
    char buffer[1024];
    
    // Test curl_mprintf
    curl_mprintf("%s", format);
    
    // Test curl_msprintf
    curl_msprintf(buffer, "%s", format);
    
    // Test curl_mvsprintf using helper
    dummy_vsprintf(buffer, "%s", format);
    
    // Test curl_mvprintf using helper
    dummy_vprintf("%s", format);
    
    // Test curl_mvsnprintf using helper
    dummy_vsnprintf(buffer, sizeof(buffer), "%s", format);
    
    // Test curl_mvfprintf using helper
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        dummy_vfprintf(fp, "%s", format);
        fclose(fp);
    }
    
    free(format);
    return 0;
}