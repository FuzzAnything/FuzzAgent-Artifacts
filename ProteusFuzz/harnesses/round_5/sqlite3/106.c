// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16_v3 at sqlite3.c:147356:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Trace callback for sqlite3_trace_v2.
 * This is a static function to avoid name conflicts.
 */
static int trace_callback(unsigned event, void *pCtx, void *pArg1, void *pArg2) {
    (void)event;
    (void)pCtx;
    (void)pArg1;
    (void)pArg2;
    return 0;
}

/*
 * Helper to safely finalize a statement if it is not NULL.
 */
static void safe_finalize(sqlite3_stmt *stmt) {
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
}

/*
 * Helper to safely close a database if it is not NULL.
 */
static void safe_close(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
    }
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const void *tail16 = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Use the fuzz input as SQL for sqlite3_prepare_v3 (UTF-8).
     * We pass the raw data as the SQL string.
     */
    rc = sqlite3_prepare_v3(db, (const char *)Data, (int)Size, 0, &stmt, &tail);
    if (rc == SQLITE_OK) {
        /*
         * If preparation succeeded, try to get parameter count.
         */
        int param_count = sqlite3_bind_parameter_count(stmt);
        (void)param_count; /* Use the value to avoid unused warning */

        /*
         * Get the database handle associated with the statement.
         */
        sqlite3 *stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db != NULL) {
            /*
             * Try to get error offset (might be -1 if no error).
             */
            int offset = sqlite3_error_offset(stmt_db);
            (void)offset;
        }

        /*
         * Finalize the statement.
         */
        safe_finalize(stmt);
        stmt = NULL;
    } else {
        /*
         * If preparation failed, try to get error offset.
         */
        int offset = sqlite3_error_offset(db);
        (void)offset;
    }

    /*
     * Now try sqlite3_prepare16_v3 (UTF-16).
     * The input data must be interpreted as UTF-16.
     * SQLite expects the length to be in bytes.
     * To avoid buffer overflows when SQLite tries to read the BOM or validate encoding,
     * we ensure the data passed is at least 2 bytes if we want to simulate valid UTF-16,
     * but we can also pass small sizes. The key is that SQLite will handle the size check.
     * However, if Size is 1, passing it as UTF-16 might cause issues if SQLite expects pairs.
     * Let's ensure we only call prepare16 if Size >= 2 to be safe, or let SQLite handle it.
     * Actually, the sanitizer error suggests reading past the buffer.
     * This happens if SQLite assumes valid UTF-16 structure (pairs of bytes) but the input is odd or malformed.
     * We will pass the size as is, but ensure we don't pass 1 byte if possible, or just let it fail safely.
     * The safest approach for fuzzing is to pass the raw bytes and let SQLite handle the error.
     * The error in the log shows a read of size 1 at offset +1, implying it tried to read a second byte of a 16-bit char.
     * If Size is 1, and SQLite tries to read a 2-byte char, it overflows.
     * We can skip prepare16 if Size < 2 to avoid this specific overflow in the fuzzer logic,
     * or we can trust SQLite to handle it. The stack trace shows it happened inside sqlite3Utf16to8.
     * To be safe, we will only call prepare16 if Size >= 2.
     */
    if (Size >= 2) {
        /*
         * Ensure the size passed to sqlite3_prepare16_v3 is even to avoid odd-byte reads.
         * If Size is odd, we decrement it by 1 to make it even.
         */
        int safe_size = (Size % 2 != 0) ? (int)Size - 1 : (int)Size;
        
        rc = sqlite3_prepare16_v3(db, (const void *)Data, safe_size, 0, &stmt, &tail16);
        if (rc == SQLITE_OK) {
            int param_count = sqlite3_bind_parameter_count(stmt);
            (void)param_count;

            sqlite3 *stmt_db = sqlite3_db_handle(stmt);
            if (stmt_db != NULL) {
                int offset = sqlite3_error_offset(stmt_db);
                (void)offset;
            }

            safe_finalize(stmt);
            stmt = NULL;
        } else {
            int offset = sqlite3_error_offset(db);
            (void)offset;
        }
    }

    /*
     * Register a trace callback using sqlite3_trace_v2.
     * We use a simple mask and the static callback.
     */
    rc = sqlite3_trace_v2(db, SQLITE_TRACE_STMT, trace_callback, NULL);
    (void)rc;

    /*
     * Try to prepare a statement again to see if tracing affects it.
     */
    rc = sqlite3_prepare_v3(db, (const char *)Data, (int)Size, 0, &stmt, &tail);
    if (rc == SQLITE_OK) {
        int param_count = sqlite3_bind_parameter_count(stmt);
        (void)param_count;

        sqlite3 *stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db != NULL) {
            int offset = sqlite3_error_offset(stmt_db);
            (void)offset;
        }

        safe_finalize(stmt);
        stmt = NULL;
    }

    /*
     * Disable tracing by passing NULL callback.
     */
    rc = sqlite3_trace_v2(db, SQLITE_TRACE_STMT, NULL, NULL);
    (void)rc;

    /*
     * Clean up.
     */
    safe_finalize(stmt);
    safe_close(db);

    return 0;
}