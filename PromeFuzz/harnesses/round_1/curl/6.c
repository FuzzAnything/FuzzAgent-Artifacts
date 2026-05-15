// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "curl/mprintf.h"

static int helper_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return ret;
}

static int helper_mvsnprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    int ret = curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Use first byte to choose function variant
    uint8_t variant = Data[0] % 6;
    
    // Use next bytes for format string (null-terminated)
    size_t format_len = (Size - 1) > 100 ? 100 : (Size - 1);
    char format[101];
    memcpy(format, Data + 1, format_len);
    format[format_len] = '\0';
    
    // Prepare arguments based on remaining data
    int int_arg = 0;
    const char *str_arg = "test";
    char dummy_str[101];
    
    if (Size > 101) {
        size_t str_len = (Size - 101) > 100 ? 100 : (Size - 101);
        memcpy(dummy_str, Data + 101, str_len);
        dummy_str[str_len] = '\0';
        str_arg = dummy_str;
        int_arg = *(int*)(Data + 101 + str_len);
    }
    
    char buffer[1024];
    FILE *fd = NULL;
    
    switch (variant) {
        case 0:  // curl_mprintf
            curl_mprintf(format, str_arg, int_arg);
            break;
            
        case 1:  // curl_msnprintf
            curl_msnprintf(buffer, sizeof(buffer), format, str_arg, int_arg);
            break;
            
        case 2:  // curl_msprintf
            curl_msprintf(buffer, format, str_arg, int_arg);
            break;
            
        case 3:  // curl_mvsprintf via helper
            helper_mvsprintf(format, str_arg, int_arg);
            break;
            
        case 4:  // curl_mvsnprintf via helper
            helper_mvsnprintf(format, str_arg, int_arg);
            break;
            
        case 5:  // curl_mfprintf
            fd = fopen("./dummy_file", "w");
            if (fd) {
                curl_mfprintf(fd, format, str_arg, int_arg);
                fclose(fd);
                remove("./dummy_file");
            }
            break;
    }
    
    return 0;
}