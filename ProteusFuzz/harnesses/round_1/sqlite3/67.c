// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:184486:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:185515:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 API functions:
 * - sqlite3_complete
 * - sqlite3_keyword_name
 * - sqlite3_complete16
 * - sqlite3_prepare16_v3
 * - sqlite3_keyword_check
 * - sqlite3_keyword_count
 */

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize SQLite if not already initialized */
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Ensure cleanup happens on exit */
    int initialized = 1;

    /* Create a temporary in-memory database */
    sqlite3 *db = NULL;
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_shutdown();
        initialized = 0;
        return 0;
    }

    /* 
     * Fuzz sqlite3_complete with UTF-8 input
     * Ensure the input is null-terminated for sqlite3_complete
     */
    {
        char *utf8_buf = (char *)malloc(Size + 1);
        if (utf8_buf) {
            memcpy(utf8_buf, Data, Size);
            utf8_buf[Size] = '\0';
            sqlite3_complete(utf8_buf);
            free(utf8_buf);
        }
    }

    /*
     * Fuzz sqlite3_keyword_count and sqlite3_keyword_name
     * Iterate through keywords based on the count
     */
    int keyword_count = sqlite3_keyword_count();
    for (int i = 0; i < keyword_count; i++) {
        const char *name = NULL;
        int len = 0;
        sqlite3_keyword_name(i, &name, &len);
        if (name != NULL && len > 0) {
            /* Optionally fuzz keyword_check with the keyword name */
            sqlite3_keyword_check(name, len);
        }
    }

    /*
     * Fuzz sqlite3_keyword_check with input data
     * Try different lengths and offsets
     */
    for (size_t len = 1; len <= Size && len <= 256; len++) {
        for (size_t offset = 0; offset <= Size - len; offset++) {
            sqlite3_keyword_check((const char *)(Data + offset), (int)len);
        }
    }

    /*
     * Fuzz sqlite3_complete16 and sqlite3_prepare16_v3 with UTF-16 input
     * Convert input to UTF-16 (simple conversion assuming ASCII or basic UTF-8)
     */
    {
        size_t utf16_len = Size * 2 + 2; /* Worst case + null terminator */
        void *utf16_buf = malloc(utf16_len * sizeof(uint16_t));
        if (utf16_buf) {
            uint16_t *wbuf = (uint16_t *)utf16_buf;
            size_t j = 0;
            for (size_t i = 0; i < Size && j < utf16_len - 1; i++) {
                wbuf[j++] = (uint16_t)Data[i];
            }
            wbuf[j] = 0;

            /* Fuzz sqlite3_complete16 */
            sqlite3_complete16(wbuf);

            /* Fuzz sqlite3_prepare16_v3 */
            sqlite3_stmt *stmt = NULL;
            const void *tail = NULL;
            int nByte = (int)(j * sizeof(uint16_t));
            sqlite3_prepare16_v3(db, wbuf, nByte, 0, &stmt, &tail);
            if (stmt) {
                sqlite3_finalize(stmt);
            }

            free(utf16_buf);
        }
    }

    /* Cleanup */
    sqlite3_close(db);
    if (initialized) {
        sqlite3_shutdown();
    }

    return 0;
}