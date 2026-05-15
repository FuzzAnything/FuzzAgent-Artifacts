// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:19130:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_free at sqlite3.c:19244:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:19130:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void dummy_vappendf_helper(sqlite3_str *pStr, const char *zFormat, ...) {
    va_list args;
    va_start(args, zFormat);
    sqlite3_str_vappendf(pStr, zFormat, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size < 1024 ? Size : 1024, fp);
        }
        fclose(fp);
    }

    // Create sqlite3_str object using sqlite3_str_new (required for the other functions)
    sqlite3_str *pStr = sqlite3_str_new(NULL);
    if (!pStr) {
        return 0;
    }

    // Use the input data to create null-terminated strings for testing
    char *test_str = NULL;
    if (Size > 0) {
        test_str = (char *)malloc(Size + 1);
        if (test_str) {
            memcpy(test_str, Data, Size);
            test_str[Size] = '\0';
        }
    } else {
        test_str = (char *)malloc(1);
        if (test_str) {
            test_str[0] = '\0';
        }
    }

    if (!test_str) {
        sqlite3_str_free(pStr);
        return 0;
    }

    // Test sqlite3_str_appendall
    if (Size > 0) {
        sqlite3_str_appendall(pStr, test_str);
    }

    // Test sqlite3_str_append
    int append_len = (int)(Size % 256); // Limit to 255 to avoid negative values
    if (append_len > 0 && Size >= (size_t)append_len) {
        sqlite3_str_append(pStr, test_str, append_len);
    }

    // Test sqlite3_str_appendf with various format strings
    if (Size > 0) {
        // Use first byte to determine format type
        uint8_t format_type = Data[0] % 4;
        switch (format_type) {
            case 0:
                sqlite3_str_appendf(pStr, "Test %s %d", test_str, (int)Size);
                break;
            case 1:
                sqlite3_str_appendf(pStr, "Size: %zu", Size);
                break;
            case 2:
                sqlite3_str_appendf(pStr, "%s", test_str);
                break;
            case 3:
                sqlite3_str_appendf(pStr, "%.*s", (int)(Size % 100), test_str);
                break;
        }
    }

    // Test sqlite3_str_vappendf
    if (Size > 1) {
        uint8_t vformat_type = Data[1] % 3;
        switch (vformat_type) {
            case 0:
                dummy_vappendf_helper(pStr, "VAppend %s", test_str);
                break;
            case 1:
                dummy_vappendf_helper(pStr, "Num: %d", (int)Size);
                break;
            case 2:
                dummy_vappendf_helper(pStr, "Hex: 0x%x", (unsigned int)Size);
                break;
        }
    }

    // Test sqlite3_str_value at different points
    char *current_value = sqlite3_str_value(pStr);
    (void)current_value; // Use value to avoid unused variable warning

    // Test sqlite3_str_finish
    char *final_str = sqlite3_str_finish(pStr);
    if (final_str) {
        sqlite3_free(final_str);
    }

    // Clean up
    free(test_str);

    // Create another sqlite3_str object for additional testing
    sqlite3_str *pStr2 = sqlite3_str_new(NULL);
    if (pStr2) {
        // Test with empty operations
        sqlite3_str_appendall(pStr2, "");
        sqlite3_str_append(pStr2, "", 0);
        
        char *empty_value = sqlite3_str_value(pStr2);
        (void)empty_value;
        
        char *final_empty = sqlite3_str_finish(pStr2);
        if (final_empty) {
            sqlite3_free(final_empty);
        }
    }

    return 0;
}