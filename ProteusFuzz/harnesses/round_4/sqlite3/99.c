// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:184486:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:185515:16 in sqlite3.h
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <wchar.h>

/*
 * Helper function to convert UTF-8 input to UTF-16 for fuzzing sqlite3_complete16
 * and sqlite3_prepare16_v3.
 */
static wchar_t *utf8_to_utf16(const uint8_t *data, size_t size) {
    if (size == 0) {
        wchar_t *empty = (wchar_t *)malloc(sizeof(wchar_t));
        if (empty) empty[0] = L'\0';
        return empty;
    }

    /* Allocate buffer for worst case (1 byte UTF-8 -> 2 bytes UTF-16) + null terminator */
    size_t max_len = size + 1;
    wchar_t *utf16 = (wchar_t *)malloc(max_len * sizeof(wchar_t));
    if (!utf16) return NULL;

    size_t i = 0;
    size_t j = 0;

    while (i < size) {
        uint32_t codepoint = 0;
        uint8_t c = data[i];

        if ((c & 0x80) == 0) {
            codepoint = c;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            if (i + 1 >= size) break;
            codepoint = ((c & 0x1F) << 6) | (data[i + 1] & 0x3F);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            if (i + 2 >= size) break;
            codepoint = ((c & 0x0F) << 12) | ((data[i + 1] & 0x3F) << 6) | (data[i + 2] & 0x3F);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            if (i + 3 >= size) break;
            codepoint = ((c & 0x07) << 18) | ((data[i + 1] & 0x3F) << 12) |
                        ((data[i + 2] & 0x3F) << 6) | (data[i + 3] & 0x3F);
            i += 4;
        } else {
            /* Invalid UTF-8 sequence, skip byte */
            i++;
            continue;
        }

        if (codepoint < 0xFFFF) {
            utf16[j++] = (wchar_t)codepoint;
        } else if (codepoint <= 0x10FFFF) {
            /* Surrogate pair */
            codepoint -= 0x10000;
            utf16[j++] = (wchar_t)(0xD800 + (codepoint >> 10));
            utf16[j++] = (wchar_t)(0xDC00 + (codepoint & 0x3FF));
        }
    }
    utf16[j] = L'\0';
    return utf16;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Create a temporary buffer for UTF-8 strings to ensure null termination */
    /* Allocate Size + 1 to safely add a null terminator */
    char *utf8_buf = (char *)malloc(Size + 1);
    if (!utf8_buf) return 0;
    
    memcpy(utf8_buf, Data, Size);
    utf8_buf[Size] = '\0';

    /* Convert to UTF-16 for 16-bit functions */
    wchar_t *utf16_buf = utf8_to_utf16(Data, Size);
    if (!utf16_buf) {
        free(utf8_buf);
        return 0;
    }

    /* Initialize SQLite if not already done (though sqlite3_complete16 does this internally) */
    sqlite3_initialize();

    /* 1. Fuzz sqlite3_complete (UTF-8) */
    int rc_complete = sqlite3_complete(utf8_buf);
    (void)rc_complete; /* Use return value to suppress warnings */

    /* 2. Fuzz sqlite3_complete16 (UTF-16) */
    int rc_complete16 = sqlite3_complete16(utf16_buf);
    (void)rc_complete16;

    /* 3. Fuzz sqlite3_keyword_check */
    /* Try checking the whole string and substrings */
    int rc_keyword_check = sqlite3_keyword_check(utf8_buf, (int)Size);
    (void)rc_keyword_check;

    /* Try with a substring if size > 1 */
    if (Size > 1) {
        int rc_keyword_check_sub = sqlite3_keyword_check(utf8_buf + 1, (int)(Size - 1));
        (void)rc_keyword_check_sub;
    }

    /* 4. Fuzz sqlite3_keyword_count and sqlite3_keyword_name */
    int keyword_count = sqlite3_keyword_count();
    if (keyword_count > 0) {
        const char *keyword_name = NULL;
        int keyword_len = 0;
        /* Use modulo to pick an index within bounds, or test out of bounds */
        int idx = (int)(Data[0] % (keyword_count + 2));
        int rc_keyword_name = sqlite3_keyword_name(idx, &keyword_name, &keyword_len);
        (void)rc_keyword_name;
        (void)keyword_name;
        (void)keyword_len;
    }

    /* 5. Fuzz sqlite3_prepare16_v3 */
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;

    /* Open an in-memory database */
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        int nByte = (int)(Size * sizeof(wchar_t));
        /* Ensure nByte doesn't exceed buffer size logic, though prepare handles it */
        if (nByte < 0) nByte = 0;

        /* Try with different flags */
        unsigned int flags = 0;
        if (Size > 0) {
            flags = Data[0] & 0x0F; /* Use first byte for flags */
        }

        int rc_prepare = sqlite3_prepare16_v3(db, utf16_buf, nByte, flags, &stmt, &tail);
        (void)rc_prepare;
        (void)tail;

        /* Cleanup statement if prepared successfully */
        if (stmt) {
            sqlite3_finalize(stmt);
        }

        /* Close database */
        sqlite3_close(db);
    }

    /* Cleanup */
    free(utf8_buf);
    free(utf16_buf);

    return 0;
}