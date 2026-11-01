// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
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
#include <stdio.h>

/*
 * Helper function to create a UTF-16 string from the fuzzer input.
 * This ensures we have a valid UTF-16 buffer for sqlite3_prepare16_v2.
 */
static int create_utf16_from_input(const uint8_t *Data, size_t Size,
                                   wchar_t **out_utf16, int *out_len) {
    if (Size == 0) {
        *out_utf16 = NULL;
        *out_len = 0;
        return 0;
    }

    /* Allocate space for UTF-16 string (worst case: 1 byte -> 1 wchar_t + null) */
    size_t max_wchars = Size + 1;
    wchar_t *utf16_buf = (wchar_t *)malloc(max_wchars * sizeof(wchar_t));
    if (!utf16_buf) {
        return -1;
    }

    /* Simple conversion: treat input bytes as characters, ensuring validity */
    size_t i;
    for (i = 0; i < Size; i++) {
        /* Map bytes to valid ASCII range to avoid invalid UTF-16 sequences if possible,
         * though sqlite3_prepare16_v2 handles raw bytes. We just ensure we don't overflow. */
        utf16_buf[i] = (wchar_t)Data[i];
    }
    utf16_buf[i] = L'\0';

    *out_utf16 = utf16_buf;
    *out_len = (int)(i * sizeof(wchar_t)); /* Length in bytes for the API */
    return 0;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const void *pzTail = NULL;
    int rc;
    wchar_t *utf16_sql = NULL;
    int utf16_len = 0;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy table to allow SQL execution */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Convert fuzzer input to UTF-16 for sqlite3_prepare16_v2 */
    if (create_utf16_from_input(Data, Size, &utf16_sql, &utf16_len) != 0) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Invoke target functions diversely */

    /* A. Test sqlite3_prepare16_v2 */
    if (utf16_sql) {
        rc = sqlite3_prepare16_v2(db, utf16_sql, utf16_len, &stmt1, &pzTail);
        if (rc == SQLITE_OK) {
            /* B. Test sqlite3_data_count (requires a row) */
            /* Step the statement to see if we get a row */
            rc = sqlite3_step(stmt1);
            if (rc == SQLITE_ROW) {
                int count = sqlite3_data_count(stmt1);
                (void)count; /* Use the return value to silence warnings */
            } else if (rc == SQLITE_DONE) {
                /* sqlite3_data_count should return 0 here */
                int count = sqlite3_data_count(stmt1);
                (void)count;
            }
        }

        /* Prepare a second statement for transfer_bindings test */
        /* We try to prepare the same SQL again to ensure parameter counts match */
        sqlite3_stmt *stmt_temp = NULL;
        rc = sqlite3_prepare16_v2(db, utf16_sql, utf16_len, &stmt_temp, &pzTail);
        if (rc == SQLITE_OK) {
            stmt2 = stmt_temp;
        }
    }

    /* C. Test sqlite3_transfer_bindings */
    /* Only if both statements are valid */
    if (stmt1 && stmt2) {
        rc = sqlite3_transfer_bindings(stmt1, stmt2);
        (void)rc;
    }

    /* D. Test sqlite3_stmt_readonly */
    /* Test with valid statements and NULL */
    if (stmt1) {
        int readonly1 = sqlite3_stmt_readonly(stmt1);
        (void)readonly1;
    }
    if (stmt2) {
        int readonly2 = sqlite3_stmt_readonly(stmt2);
        (void)readonly2;
    }
    int readonly_null = sqlite3_stmt_readonly(NULL);
    (void)readonly_null;

    /* E. Test sqlite3_expired */
    /* Test with valid statements and NULL */
    if (stmt1) {
        int expired1 = sqlite3_expired(stmt1);
        (void)expired1;
    }
    if (stmt2) {
        int expired2 = sqlite3_expired(stmt2);
        (void)expired2;
    }
    int expired_null = sqlite3_expired(NULL);
    (void)expired_null;

    /* F. Test sqlite3_reset */
    /* Reset statements if they exist */
    if (stmt1) {
        rc = sqlite3_reset(stmt1);
        (void)rc;
    }
    if (stmt2) {
        rc = sqlite3_reset(stmt2);
        (void)rc;
    }

    /* 3. Cleanup */
    if (stmt1) {
        sqlite3_finalize(stmt1);
    }
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    if (utf16_sql) {
        free(utf16_sql);
    }
    sqlite3_close(db);

    return 0;
}