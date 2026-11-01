// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
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
 * Helper function to convert UTF-8 input data to UTF-16.
 * This allows the fuzzer to provide arbitrary bytes which are then
 * interpreted as potential SQL statements.
 */
static int convert_utf8_to_utf16(const uint8_t *data, size_t size, wchar_t **out_utf16, size_t *out_len) {
    if (size == 0) {
        *out_utf16 = NULL;
        *out_len = 0;
        return 0;
    }

    /* Allocate enough space for worst case (1 byte -> 2 bytes) + null terminator */
    size_t max_len = size + 1;
    wchar_t *utf16_buf = (wchar_t *)malloc(max_len * sizeof(wchar_t));
    if (!utf16_buf) {
        return -1;
    }

    size_t i = 0;
    size_t j = 0;
    while (i < size) {
        uint8_t c = data[i];
        if (c < 0x80) {
            utf16_buf[j++] = (wchar_t)c;
            i++;
        } else if (c < 0xE0) {
            if (i + 1 < size) {
                utf16_buf[j++] = (wchar_t)((c & 0x1F) << 6 | (data[i+1] & 0x3F));
                i += 2;
            } else {
                break;
            }
        } else if (c < 0xF0) {
            if (i + 2 < size) {
                utf16_buf[j++] = (wchar_t)((c & 0x0F) << 12 | (data[i+1] & 0x3F) << 6 | (data[i+2] & 0x3F));
                i += 3;
            } else {
                break;
            }
        } else {
            /* Invalid or unsupported, skip byte */
            i++;
        }
    }
    utf16_buf[j] = L'\0';

    *out_utf16 = utf16_buf;
    *out_len = (j + 1) * sizeof(wchar_t);
    return 0;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const void *tail = NULL;
    wchar_t *utf16_sql = NULL;
    size_t utf16_len = 0;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to allow valid SQL execution if the input is valid */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Convert input data to UTF-16 for sqlite3_prepare16_v2 */
    if (convert_utf8_to_utf16(Data, Size, &utf16_sql, &utf16_len) == 0 && utf16_sql != NULL) {
        
        /* 2. Invoke sqlite3_prepare16_v2 */
        rc = sqlite3_prepare16_v2(db, utf16_sql, (int)utf16_len, &stmt1, &tail);
        
        if (rc == SQLITE_OK && stmt1 != NULL) {
            /* 
             * 3. Explore more program states 
             * We have a valid statement, try to exercise the other target APIs.
             */

            /* Try sqlite3_stmt_readonly */
            int is_readonly = sqlite3_stmt_readonly(stmt1);
            (void)is_readonly; /* Suppress unused warning */

            /* Try sqlite3_clear_bindings */
            rc = sqlite3_clear_bindings(stmt1);
            (void)rc;

            /* Try sqlite3_bind_zeroblob with fuzzed parameters */
            if (Size >= 4) {
                int param_idx = (Data[0] % 10) + 1; /* 1-10 */
                int blob_size = (Data[1] << 8) | Data[2]; /* 0-65535 */
                if (blob_size > 100000) blob_size = 100000; /* Cap size to prevent OOM */
                
                rc = sqlite3_bind_zeroblob(stmt1, param_idx, blob_size);
                (void)rc;
            }

            /* Prepare a second statement to test sqlite3_transfer_bindings */
            /* We try to prepare the same SQL again to ensure parameter count matches */
            sqlite3_stmt *stmt_temp = NULL;
            rc = sqlite3_prepare16_v2(db, utf16_sql, (int)utf16_len, &stmt_temp, &tail);
            if (rc == SQLITE_OK && stmt_temp != NULL) {
                /* Bind something to stmt_temp first to have something to transfer */
                if (Size >= 2) {
                    int param_idx = (Data[3] % 10) + 1;
                    int blob_size = (Data[4] % 100) + 1;
                    sqlite3_bind_zeroblob(stmt_temp, param_idx, blob_size);
                }

                /* Try sqlite3_transfer_bindings from stmt_temp to stmt1 */
                rc = sqlite3_transfer_bindings(stmt_temp, stmt1);
                (void)rc;

                /* Cleanup temp statement */
                sqlite3_finalize(stmt_temp);
            }

            /* Try sqlite3_reset */
            rc = sqlite3_reset(stmt1);
            (void)rc;

            /* Cleanup stmt1 */
            sqlite3_finalize(stmt1);
        } else {
            /* If preparation failed, we might still want to test error paths if possible,
             * but most APIs require a valid stmt. We proceed to cleanup. */
        }
    }

    /* 4. Cleanup */
    if (utf16_sql) {
        free(utf16_sql);
    }
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}