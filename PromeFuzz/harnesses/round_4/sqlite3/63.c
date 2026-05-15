// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_check at sqlite3.c:169743:16 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:169742:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:169735:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // 1. sqlite3_keyword_check
    // Use the fuzzer input as the keyword to check
    if (Size > 0) {
        // Create a null-terminated string from the input data
        char *keyword = (char *)malloc(Size + 1);
        if (keyword) {
            memcpy(keyword, Data, Size);
            keyword[Size] = '\0';
            (void)sqlite3_keyword_check(keyword, (int)Size);
            free(keyword);
        }
    }

    // 2. sqlite3_sourceid
    // No input needed, just call the function
    (void)sqlite3_sourceid();

    // 3. sqlite3_keyword_count
    // No input needed, just call the function
    int keyword_count = sqlite3_keyword_count();

    // 4. sqlite3_compileoption_used
    // Use fuzzer input as option name
    if (Size > 0) {
        char *opt_name = (char *)malloc(Size + 1);
        if (opt_name) {
            memcpy(opt_name, Data, Size);
            opt_name[Size] = '\0';
            (void)sqlite3_compileoption_used(opt_name);
            free(opt_name);
        }
    }

    // 5. sqlite3_compileoption_get
    // Use first byte of input as index (modulo to keep it reasonable)
    if (Size > 0) {
        int idx = Data[0] % 100;  // Limit to reasonable range
        (void)sqlite3_compileoption_get(idx);
    }

    // 6. sqlite3_keyword_name
    // Use first byte of input as index, bounded by keyword_count
    if (Size > 0 && keyword_count > 0) {
        int idx = Data[0] % keyword_count;
        const char *keyword_name = NULL;
        int keyword_len = 0;
        (void)sqlite3_keyword_name(idx, &keyword_name, &keyword_len);
    }

    return 0;
}