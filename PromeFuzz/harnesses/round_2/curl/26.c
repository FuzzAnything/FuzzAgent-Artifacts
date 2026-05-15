// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int helper_mvsnprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return result;
}

static int helper_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_msnprintf
    char buffer1[1024];
    curl_msnprintf(buffer1, sizeof(buffer1), "%s", input_str);
    
    // Test curl_mvsnprintf via helper
    helper_mvsnprintf("%s", input_str);
    
    // Test curl_mvsprintf via helper
    helper_mvsprintf("%s", input_str);
    
    // Test curl_mprintf
    curl_mprintf("%s", input_str);
    
    // Test curl_mfprintf
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        curl_mfprintf(dummy_file, "%s", input_str);
        fclose(dummy_file);
    }
    
    // Test curl_msprintf
    char buffer2[1024];
    curl_msprintf(buffer2, "%s", input_str);

    free(input_str);
    return 0;
}