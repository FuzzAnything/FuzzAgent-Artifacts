// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:184486:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:185515:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int convert_utf8_to_utf16(const uint8_t *src, size_t len, uint16_t **out, size_t *out_len) {
    if (len == 0) {
        *out = NULL;
        *out_len = 0;
        return 0;
    }

    // Allocate buffer: worst case 2 bytes per char + 2 for null terminator
    size_t max_utf16_len = len + 1;
    uint16_t *buffer = (uint16_t *)malloc(max_utf16_len * sizeof(uint16_t));
    if (!buffer) return -1;

    size_t j = 0;
    for (size_t i = 0; i < len && j < max_utf16_len - 1; i++) {
        buffer[j++] = (uint16_t)src[i];
    }
    buffer[j] = 0; // Null terminator

    *out = buffer;
    *out_len = j * sizeof(uint16_t);
    return 0;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure we have at least some data to work with
    uint8_t *input = (uint8_t *)malloc(Size);
    if (!input) return 0;
    memcpy(input, Data, Size);
    input[Size - 1] = 0; // Ensure null termination for string functions

    // Initialize SQLite if needed (some functions do this automatically, but good practice)
    if (sqlite3_initialize() != SQLITE_OK) {
        free(input);
        return 0;
    }

    // 1. Test sqlite3_complete (UTF-8)
    int ret_complete = sqlite3_complete((const char *)input);
    (void)ret_complete; // Suppress unused warning

    // 2. Test sqlite3_keyword_count
    int keyword_count = sqlite3_keyword_count();
    (void)keyword_count;

    // 3. Test sqlite3_keyword_name and sqlite3_keyword_check
    // Enumerate keywords safely
    for (int i = 0; i < keyword_count; i++) {
        const char *keyword_name = NULL;
        int keyword_len = 0;
        if (sqlite3_keyword_name(i, &keyword_name, &keyword_len) == SQLITE_OK) {
            // Check the keyword itself
            int is_keyword = sqlite3_keyword_check(keyword_name, keyword_len);
            (void)is_keyword;
        }
    }

    // Check the input string against keywords (if it looks like an identifier)
    if (Size > 0 && Size < 100) {
        int is_kw = sqlite3_keyword_check((const char *)input, Size - 1);
        (void)is_kw;
    }

    // 4. Prepare UTF-16 for sqlite3_complete16 and sqlite3_prepare16_v3
    uint16_t *utf16_buf = NULL;
    size_t utf16_len = 0;
    if (convert_utf8_to_utf16(input, Size, &utf16_buf, &utf16_len) == 0 && utf16_buf) {
        // Test sqlite3_complete16
        int ret_complete16 = sqlite3_complete16((const void *)utf16_buf);
        (void)ret_complete16;

        // Test sqlite3_prepare16_v3
        sqlite3 *db = NULL;
        // Open an in-memory database
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            sqlite3_stmt *stmt = NULL;
            const void *tail = NULL;
            
            // Use the full length of the UTF-16 buffer
            int ret_prepare = sqlite3_prepare16_v3(
                db, 
                (const void *)utf16_buf, 
                (int)utf16_len, 
                0, 
                &stmt, 
                &tail
            );
            (void)ret_prepare;
            (void)tail;

            if (stmt != NULL) {
                sqlite3_finalize(stmt);
            }
            
            sqlite3_close(db);
        }

        free(utf16_buf);
    }

    free(input);
    return 0;
}