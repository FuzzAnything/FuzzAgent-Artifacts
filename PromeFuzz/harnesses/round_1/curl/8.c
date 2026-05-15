// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

static void test_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_mvsnprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data for format strings
    char *format_str = (char *)malloc(Size + 1);
    if (!format_str) {
        return 0;
    }
    memcpy(format_str, Data, Size);
    format_str[Size] = '\0';

    // Create another string for buffer content
    char *buffer_str = (char *)malloc(Size + 1);
    if (!buffer_str) {
        free(format_str);
        return 0;
    }
    memcpy(buffer_str, Data, Size);
    buffer_str[Size] = '\0';

    // Create a buffer for formatted output
    char buffer[1024];
    size_t buffer_size = sizeof(buffer);

    // Test curl_mprintf
    curl_mprintf("%s", format_str);

    // Test curl_msnprintf
    curl_msnprintf(buffer, buffer_size, "%s", format_str);

    // Test curl_msprintf
    curl_msprintf(buffer, "%s", format_str);

    // Test curl_mvsprintf using helper function
    test_mvsprintf("%s", format_str);

    // Test curl_mvsnprintf using helper function
    test_mvsnprintf("%s", format_str);

    // Test curl_mfprintf
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        curl_mfprintf(fp, "%s", format_str);
        fclose(fp);
    }

    // Test with different format specifiers to explore more states
    if (Size > 8) {
        // Use first 4 bytes as integer if possible
        int int_val = 0;
        memcpy(&int_val, Data, sizeof(int_val) < Size ? sizeof(int_val) : Size);
        
        curl_mprintf("Number: %d, String: %s", int_val, format_str);
        curl_msnprintf(buffer, buffer_size, "Number: %d, String: %s", int_val, format_str);
        
        // Test with file again
        fp = fopen("./dummy_file", "w");
        if (fp) {
            curl_mfprintf(fp, "Number: %d, String: %s", int_val, format_str);
            fclose(fp);
        }
        
        // Test variable argument functions with multiple arguments
        test_mvsprintf("Number: %d, String: %s", int_val, format_str);
        test_mvsnprintf("Number: %d, String: %s", int_val, format_str);
    }

    // Clean up
    free(format_str);
    free(buffer_str);
    
    // Remove dummy file if it exists
    remove("./dummy_file");

    return 0;
}

#ifdef __cplusplus
}
#endif