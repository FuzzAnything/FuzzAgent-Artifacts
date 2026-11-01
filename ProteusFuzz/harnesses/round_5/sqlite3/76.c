// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static void fuzzer_initialize(void) {
    /* Initialize SQLite if needed, though sqlite3_str_new(NULL) works without a DB */
    /* No global initialization strictly required for sqlite3_str API with NULL db */
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzzer_initialize();

    /* Create a new sqlite3_str object with NULL database context */
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str == NULL) {
        return 0;
    }

    /* Use the fuzzer input to drive the string operations */
    size_t idx = 0;

    /* 1. Append characters using sqlite3_str_appendchar */
    if (idx < Size) {
        int count = Data[idx++];
        char ch = (char)(Data[idx % Size]);
        idx = (idx < Size) ? idx + 1 : idx;
        sqlite3_str_appendchar(str, count, ch);
    }

    /* 2. Append a string using sqlite3_str_append */
    if (idx < Size) {
        int len = Data[idx++];
        /* Prevent integer overflow and excessive allocation */
        if (len > (int)Size || len > 1024) {
            len = (int)Size > 1024 ? 1024 : (int)Size;
        }
        if (idx + len <= Size) {
            sqlite3_str_append(str, (const char*)&Data[idx], len);
            idx += len;
        }
    }

    /* 3. Truncate the string using sqlite3_str_truncate */
    if (idx < Size) {
        int truncate_len = (int)Data[idx++];
        /* Ensure non-negative and reasonable limit */
        if (truncate_len < 0) truncate_len = 0;
        if (truncate_len > 10000) truncate_len = 10000;
        sqlite3_str_truncate(str, truncate_len);
    }

    /* 4. Check error state using sqlite3_str_errcode */
    int err = sqlite3_str_errcode(str);
    (void)err; /* Suppress unused variable warning, logic handled by finish */

    /* 5. Get current length using sqlite3_str_length */
    int len = sqlite3_str_length(str);
    (void)len;

    /* 6. Retrieve the value using sqlite3_str_value */
    /* Note: This adds a null terminator. We should not modify the returned pointer 
       or use it after finish. We just read it to exercise the code path. */
    char *val = sqlite3_str_value(str);
    if (val != NULL) {
        /* Just a dummy read to ensure the pointer is valid */
        volatile char c = val[0];
        (void)c;
    }

    /* Cleanup: Finalize the string object to free memory */
    sqlite3_str_finish(str);

    return 0;
}