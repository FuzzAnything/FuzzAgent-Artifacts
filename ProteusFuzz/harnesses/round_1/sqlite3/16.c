// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

static char *safe_vmprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *formatted = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    // Initialize database connection using in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // 1. sqlite3_vmprintf
    // Prepare a format string from the input data, ensuring it's null-terminated
    size_t fmt_len = (Size > 100) ? 100 : Size;
    char *fmt_buf = (char *)sqlite3_malloc(fmt_len + 1);
    if (fmt_buf) {
        memcpy(fmt_buf, Data, fmt_len);
        fmt_buf[fmt_len] = '\0';
        
        formatted = safe_vmprintf(fmt_buf);
        
        sqlite3_free(fmt_buf);
    }

    // 2. sqlite3_prepare_v2
    // Use the input data as SQL, ensuring null termination
    size_t sql_len = (Size > 2000) ? 2000 : Size;
    char *sql_buf = (char *)sqlite3_malloc(sql_len + 1);
    if (sql_buf) {
        memcpy(sql_buf, Data, sql_len);
        sql_buf[sql_len] = '\0';
        
        // Try to prepare the statement
        rc = sqlite3_prepare_v2(db, sql_buf, sql_len, &stmt, &tail);
        
        sqlite3_free(sql_buf);
    }

    // 3. sqlite3_extended_errcode
    if (db) {
        int ext_err = sqlite3_extended_errcode(db);
        (void)ext_err;
    }

    // 4. sqlite3_errmsg
    if (db) {
        const char *msg = sqlite3_errmsg(db);
        (void)msg;
    }

    // 5. sqlite3_step
    if (stmt) {
        rc = sqlite3_step(stmt);
        (void)rc;
    }

    // 6. sqlite3_reset
    if (stmt) {
        rc = sqlite3_reset(stmt);
        (void)rc;
    }

    // 7. sqlite3_extended_errcode (again)
    if (db) {
        int ext_err = sqlite3_extended_errcode(db);
        (void)ext_err;
    }

    // 8. sqlite3_errmsg (again)
    if (db) {
        const char *msg = sqlite3_errmsg(db);
        (void)msg;
    }

    // 9. sqlite3_free
    if (formatted) {
        sqlite3_free(formatted);
        formatted = NULL;
    }

    // 10. sqlite3_finalize
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // Cleanup database connection
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }

    return 0;
}