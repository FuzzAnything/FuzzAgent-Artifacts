// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * The fuzzer entry point.
 * This function must be defined exactly as shown to be linked by libFuzzer.
 */
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    char *result = NULL;
    int ret;

    // Initialize database connection (optional for sqlite3_str_new, but good for context)
    ret = sqlite3_open(":memory:", &db);
    if (ret != SQLITE_OK) {
        // If open fails, we can still test with NULL db passed to sqlite3_str_new
        db = NULL;
    }

    // Create a new dynamic string object
    str = sqlite3_str_new(db);
    if (str == NULL) {
        // Should not happen per docs, but handle gracefully
        if (db) sqlite3_close(db);
        return 0;
    }

    // Use the fuzzing input to drive various append operations
    size_t offset = 0;
    
    // 1. Test sqlite3_str_appendall with a null-terminated string derived from input
    if (Size > 0) {
        // Find a null byte or use the whole buffer if none exists
        size_t len = strnlen((const char *)Data, Size);
        if (len < Size) {
            sqlite3_str_appendall(str, (const char *)Data);
            offset = len + 1; // Skip the null terminator
        } else {
            // No null terminator, treat as binary for append
            sqlite3_str_append(str, (const char *)Data, Size);
            offset = Size;
        }
    }

    // 2. Test sqlite3_str_append with remaining data
    if (offset < Size) {
        size_t remaining = Size - offset;
        // Append a chunk of remaining data
        int n = (remaining > 1000) ? 1000 : remaining;
        sqlite3_str_append(str, (const char *)(Data + offset), n);
        offset += n;
    }

    // 3. Test sqlite3_str_appendf with formatted strings
    if (offset < Size) {
        unsigned char c = Data[offset];
        // Safe format string: just append an integer derived from input
        sqlite3_str_appendf(str, "val:%d", c);
        offset++;
    }
    
    // Test another appendf with a string
    if (offset < Size) {
        // Construct a safe format string
        char fmt[64] = {0};
        int fmtLen = (Size - offset > 30) ? 30 : (Size - offset);
        // Copy safe characters to fmt, avoiding %
        int j = 0;
        for (int i = 0; i < fmtLen && j < 60; i++) {
            unsigned char b = Data[offset + i];
            if (b != '%' && b >= 32 && b < 127) {
                fmt[j++] = b;
            }
        }
        if (j > 0) {
            sqlite3_str_appendf(str, fmt);
            offset += fmtLen;
        }
    }

    // 4. Test sqlite3_str_value to get current state (does not consume)
    char *val = sqlite3_str_value(str);
    if (val != NULL) {
        // Just read the value, no need to free, it's internal
        (void)val;
    }

    // 5. Test sqlite3_str_finish to finalize and get the result
    result = sqlite3_str_finish(str);
    str = NULL; // str is now invalid

    // Cleanup result
    if (result != NULL) {
        sqlite3_free(result);
    }

    // Cleanup database
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}