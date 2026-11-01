// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Helper function to convert UTF-8 input to UTF-16LE.
 * SQLite's prepare16 functions expect UTF-16 encoded strings.
 * Fuzzers typically provide raw bytes (often interpreted as UTF-8 or binary).
 * We convert the input to UTF-16LE to ensure valid encoding for the target API.
 */
static void convert_utf8_to_utf16le(const uint8_t *in, size_t in_size, uint16_t *out, size_t out_size) {
    size_t i = 0;
    size_t j = 0;
    while (i < in_size && j < out_size) {
        uint8_t c = in[i];
        if (c < 0x80) {
            if (j + 1 < out_size) {
                out[j++] = c;
                out[j++] = 0;
            } else {
                break;
            }
            i++;
        } else if (c < 0xE0) {
            if (i + 1 < in_size && j + 1 < out_size) {
                uint16_t val = ((c & 0x1F) << 6) | (in[i + 1] & 0x3F);
                out[j++] = val & 0xFF;
                out[j++] = (val >> 8) & 0xFF;
                i += 2;
            } else {
                i++;
            }
        } else if (c < 0xF0) {
            if (i + 2 < in_size && j + 1 < out_size) {
                uint16_t val = ((c & 0x0F) << 12) | ((in[i + 1] & 0x3F) << 6) | (in[i + 2] & 0x3F);
                out[j++] = val & 0xFF;
                out[j++] = (val >> 8) & 0xFF;
                i += 3;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
    /* Ensure null termination if space permits */
    if (j + 1 < out_size) {
        out[j++] = 0;
        out[j++] = 0;
    }
}

/*
 * Helper to safely finalize a statement if it is not NULL.
 */
static void safe_finalize(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Ensure we have enough data to create a valid UTF-16 string (at least 2 bytes for null terminator) */
    if (Size < 2) {
        sqlite3_close(db);
        return 0;
    }

    /* Allocate buffer for UTF-16 conversion. 
     * Worst case: 1 UTF-8 char -> 2 bytes UTF-16. 
     * We allocate Size * 2 + 4 to be safe.
     */
    size_t utf16_size = Size * 2 + 4;
    uint16_t *utf16_buf = (uint16_t *)malloc(utf16_size * sizeof(uint16_t));
    if (!utf16_buf) {
        sqlite3_close(db);
        return 0;
    }

    /* Convert input to UTF-16LE */
    convert_utf8_to_utf16le(Data, Size, utf16_buf, utf16_size);
    
    /* Calculate length in bytes for the UTF-16 buffer */
    int nByte = (int)(utf16_size * sizeof(uint16_t));

    /* 2. Fuzzing Logic: Explore diverse states */
    
    /* Strategy 1: sqlite3_prepare16 */
    tail = NULL;
    rc = sqlite3_prepare16(db, utf16_buf, nByte, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        /* If successful, try to inspect column names */
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            (void)sqlite3_column_name16(stmt, i);
        }
        /* Check busy status */
        (void)sqlite3_stmt_busy(stmt);
        /* Check expired status */
        (void)sqlite3_expired(stmt);
        
        /* Execute step to change state for busy check */
        (void)sqlite3_step(stmt);
        (void)sqlite3_stmt_busy(stmt);
        
        safe_finalize(stmt);
        stmt = NULL;
    }

    /* Strategy 2: sqlite3_prepare16_v2 */
    tail = NULL;
    rc = sqlite3_prepare16_v2(db, utf16_buf, nByte, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            (void)sqlite3_column_name16(stmt, i);
        }
        (void)sqlite3_stmt_busy(stmt);
        (void)sqlite3_expired(stmt);
        (void)sqlite3_step(stmt);
        (void)sqlite3_stmt_busy(stmt);
        safe_finalize(stmt);
        stmt = NULL;
    }

    /* Strategy 3: sqlite3_prepare16_v3 with different flags */
    /* Try with SQLITE_PREPARE_PERSISTENT */
    tail = NULL;
    rc = sqlite3_prepare16_v3(db, utf16_buf, nByte, SQLITE_PREPARE_PERSISTENT, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            (void)sqlite3_column_name16(stmt, i);
        }
        (void)sqlite3_stmt_busy(stmt);
        (void)sqlite3_expired(stmt);
        (void)sqlite3_step(stmt);
        safe_finalize(stmt);
        stmt = NULL;
    }

    /* Try with SQLITE_PREPARE_NORMALIZE */
    tail = NULL;
    rc = sqlite3_prepare16_v3(db, utf16_buf, nByte, SQLITE_PREPARE_NORMALIZE, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count; i++) {
            (void)sqlite3_column_name16(stmt, i);
        }
        (void)sqlite3_stmt_busy(stmt);
        (void)sqlite3_expired(stmt);
        (void)sqlite3_step(stmt);
        safe_finalize(stmt);
        stmt = NULL;
    }

    /* Strategy 4: Test with NULL statement pointer for safety checks */
    (void)sqlite3_stmt_busy(NULL);
    (void)sqlite3_expired(NULL);

    /* Strategy 5: Test with invalid column indices */
    /* We need a valid statement for this. Let's create a simple one if possible. */
    const uint16_t valid_sql[] = { 'S', 0, 'E', 0, 'L', 0, 'E', 0, 'C', 0, 'T', 0, ' ', 0, '1', 0, 0, 0 };
    tail = NULL;
    rc = sqlite3_prepare16_v2(db, valid_sql, sizeof(valid_sql), &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        /* Test valid index */
        (void)sqlite3_column_name16(stmt, 0);
        /* Test invalid index */
        (void)sqlite3_column_name16(stmt, -1);
        (void)sqlite3_column_name16(stmt, 100);
        safe_finalize(stmt);
        stmt = NULL;
    }

    /* 3. Cleanup */
    safe_finalize(stmt);
    free(utf16_buf);
    sqlite3_close(db);

    return 0;
}