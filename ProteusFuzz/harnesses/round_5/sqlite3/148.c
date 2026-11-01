// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * Helper to convert raw fuzz input to UTF-16.
 * Input is treated as UTF-8 or raw bytes. We convert to UTF-16LE for testing.
 * Returns allocated buffer, caller must free.
 */
static uint16_t *convert_to_utf16le(const uint8_t *data, size_t size, size_t *out_size) {
    if (size == 0) {
        *out_size = 0;
        return NULL;
    }

    /* Allocate buffer for worst case (1 byte -> 2 bytes) + null terminator */
    size_t max_utf16_size = (size + 1) * 2;
    uint16_t *utf16_buf = (uint16_t *)malloc(max_utf16_size);
    if (!utf16_buf) return NULL;

    size_t idx = 0;
    for (size_t i = 0; i < size; i++) {
        uint8_t byte = data[i];
        /* Simple conversion: treat each byte as a character for fuzzing purposes */
        utf16_buf[idx++] = (uint16_t)byte;
    }
    /* Null terminate */
    utf16_buf[idx] = 0;
    *out_size = (idx + 1) * sizeof(uint16_t);
    return utf16_buf;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    int rc;
    uint16_t *utf16_sql = NULL;
    size_t utf16_len = 0;

    /* Ensure we have some data to work with */
    if (Size < 2) {
        return 0;
    }

    /* Create a temporary file for the database */
    const char *db_file = "./dummy_file";
    int fd = open(db_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    close(fd);

    /* Open database in memory or file */
    rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        unlink(db_file);
        return 0;
    }

    /* Convert input to UTF-16 */
    utf16_sql = convert_to_utf16le(Data, Size, &utf16_len);
    if (!utf16_sql && Size > 0) {
        sqlite3_close(db);
        unlink(db_file);
        return 0;
    }

    if (utf16_sql) {
        /* Fuzz sqlite3_prepare16_v3 */
        rc = sqlite3_prepare16_v3(db, utf16_sql, (int)utf16_len, 0, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            /* Check column names */
            for (int i = 0; i < 10; i++) {
                sqlite3_column_name16(stmt, i);
            }
            /* Check busy/expired status */
            sqlite3_stmt_busy(stmt);
            sqlite3_expired(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }

        /* Fuzz sqlite3_prepare16_v2 */
        rc = sqlite3_prepare16_v2(db, utf16_sql, (int)utf16_len, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_column_name16(stmt, 0);
            sqlite3_stmt_busy(stmt);
            sqlite3_expired(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }

        /* Fuzz sqlite3_prepare16 */
        rc = sqlite3_prepare16(db, utf16_sql, (int)utf16_len, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_column_name16(stmt, 0);
            sqlite3_stmt_busy(stmt);
            sqlite3_expired(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /* Test with NULL statement for safety checks */
    sqlite3_expired(NULL);
    sqlite3_stmt_busy(NULL);

    /* Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    free(utf16_sql);
    unlink(db_file);

    return 0;
}