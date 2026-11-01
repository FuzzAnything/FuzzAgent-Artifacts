// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize sqlite3_str object
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str == NULL) {
        return 0;
    }

    // Use input data to drive fuzzing logic
    const char *input = (const char *)Data;
    size_t inputLen = Size;

    // Fuzz sqlite3_str_append
    // Append varying lengths of input data
    for (size_t i = 0; i < inputLen; i++) {
        int len = (Data[i] % 100) + 1;
        if (i + len <= inputLen) {
            sqlite3_str_append(str, &input[i], len);
        } else {
            // Fallback to appending single char if remaining data is insufficient
            sqlite3_str_append(str, &input[i], 1);
        }
    }

    // Fuzz sqlite3_str_appendchar
    // Append random characters
    for (size_t i = 0; i < inputLen; i++) {
        int count = (Data[i] % 50) + 1;
        char ch = (char)(Data[i] % 256);
        sqlite3_str_appendchar(str, count, ch);
    }

    // Fuzz sqlite3_str_truncate
    // Truncate to various lengths
    for (size_t i = 0; i < inputLen; i++) {
        int n = (Data[i] % 1000);
        sqlite3_str_truncate(str, n);
    }

    // Fuzz sqlite3_str_length
    // Check length multiple times
    for (size_t i = 0; i < inputLen; i++) {
        (void)sqlite3_str_length(str);
    }

    // Fuzz sqlite3_str_value
    // Get the string value multiple times
    for (size_t i = 0; i < inputLen; i++) {
        char *val = sqlite3_str_value(str);
        if (val != NULL) {
            // Just read the value to ensure it's valid
            (void)strlen(val);
        }
    }

    // Fuzz sqlite3_str_errcode
    // Check error code multiple times
    for (size_t i = 0; i < inputLen; i++) {
        (void)sqlite3_str_errcode(str);
    }

    // Cleanup
    sqlite3_str_finish(str);

    return 0;
}