// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_keyword_check at sqlite3.c:184487:16 in sqlite3.h
// sqlite3_complete16 at sqlite3.c:185515:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_keyword_count at sqlite3.c:184486:16 in sqlite3.h
// sqlite3_keyword_name at sqlite3.c:184479:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/*
 * Helper function to convert UTF-8 to UTF-16.
 * Returns 0 on success, -1 on failure.
 */
static int utf8_to_utf16(const uint8_t *src, size_t size, wchar_t **dest) {
    if (src == NULL || size == 0) {
        *dest = NULL;
        return 0;
    }

    /* Allocate space for worst-case scenario (1 byte UTF-8 -> 2 bytes UTF-16) + null terminator */
    size_t max_len = size + 1;
    wchar_t *buffer = (wchar_t *)malloc(max_len * sizeof(wchar_t));
    if (buffer == NULL) {
        return -1;
    }

    size_t i = 0;
    size_t j = 0;
    while (i < size && j < max_len - 1) {
        unsigned char c = src[i];
        if (c < 0x80) {
            buffer[j++] = (wchar_t)c;
            i++;
        } else if (c < 0xE0) {
            if (i + 1 >= size) break;
            buffer[j++] = ((c & 0x1F) << 6) | (src[i+1] & 0x3F);
            i += 2;
        } else if (c < 0xF0) {
            if (i + 2 >= size) break;
            buffer[j++] = ((c & 0x0F) << 12) | ((src[i+1] & 0x3F) << 6) | (src[i+2] & 0x3F);
            i += 3;
        } else {
            /* Skip invalid sequences or 4-byte chars for simplicity in this fuzzer */
            i++;
        }
    }
    buffer[j] = L'\0';
    *dest = buffer;
    return 0;
}

/*
 * Helper to create a valid database connection for testing prepare functions.
 * Uses an in-memory database to avoid file system dependencies.
 */
static sqlite3 *get_db_connection(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input is treated as a string by appending a null terminator if needed */
    char *utf8_buf = NULL;
    wchar_t *utf16_buf = NULL;
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;

    /* Allocate buffer with space for null terminator */
    utf8_buf = (char *)malloc(Size + 1);
    if (!utf8_buf) return 0;
    memcpy(utf8_buf, Data, Size);
    utf8_buf[Size] = '\0';

    /* 1. Test sqlite3_complete (UTF-8) */
    int rc_complete = sqlite3_complete(utf8_buf);
    (void)rc_complete; /* Suppress unused warning, result is just for coverage */

    /* 2. Test sqlite3_keyword_count */
    int keyword_count = sqlite3_keyword_count();
    if (keyword_count > 0) {
        /* 3. Test sqlite3_keyword_name */
        /* Fuzz index based on input data to explore bounds and valid indices */
        int idx = (Size > 0) ? (Data[0] % (keyword_count + 2)) : 0;
        const char *keyword_name = NULL;
        int keyword_len = 0;
        int rc_name = sqlite3_keyword_name(idx, &keyword_name, &keyword_len);
        (void)rc_name;
        (void)keyword_name;
        (void)keyword_len;

        /* 4. Test sqlite3_keyword_check */
        /* Use the input string as a potential identifier */
        int rc_check = sqlite3_keyword_check(utf8_buf, Size);
        (void)rc_check;
    }

    /* 5. Test sqlite3_prepare16_v3 and sqlite3_complete16 */
    /* Convert input to UTF-16 for these APIs */
    if (utf8_to_utf16(Data, Size, &utf16_buf) == 0 && utf16_buf != NULL) {
        /* Test sqlite3_complete16 */
        int rc_complete16 = sqlite3_complete16(utf16_buf);
        (void)rc_complete16;

        /* Setup database for prepare16 */
        db = get_db_connection();
        if (db) {
            /* Calculate byte length for UTF-16 string */
            size_t len_utf16 = wcslen(utf16_buf);
            int nByte = (int)(len_utf16 * sizeof(wchar_t));
            
            /* Fuzz flags using input data */
            unsigned int prepFlags = 0;
            if (Size > 1) {
                prepFlags = Data[1] & 0x0F; /* Limit to reasonable flag combinations */
            }

            /* Test sqlite3_prepare16_v3 */
            int rc_prepare = sqlite3_prepare16_v3(
                db,
                utf16_buf,
                nByte,
                prepFlags,
                &stmt,
                &tail
            );
            
            if (rc_prepare == SQLITE_OK && stmt != NULL) {
                /* Finalize the statement to clean up */
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            
            /* Close database */
            sqlite3_close(db);
            db = NULL;
        }
        
        free(utf16_buf);
        utf16_buf = NULL;
    }

    free(utf8_buf);
    return 0;
}