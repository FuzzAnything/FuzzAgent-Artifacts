// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

// Helper function to call sqlite3_vmprintf with a va_list
// The function must have a variadic argument to use va_start correctly.
static char* safe_vmprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = sqlite3_vmprintf(fmt, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *formatted = NULL;
    int rc;

    // Initialize database connection using a dummy file
    if (sqlite3_open("./dummy_file", &db) != SQLITE_OK) {
        return 0;
    }

    // Ensure we have enough data for a format string and arguments
    if (Size < 1) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_vmprintf
    // Use the input data as a format string, ensuring it is null-terminated
    size_t fmt_len = Size > 1000 ? 1000 : Size;
    char *fmt_buf = (char *)sqlite3_malloc(fmt_len + 1);
    if (!fmt_buf) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(fmt_buf, Data, fmt_len);
    fmt_buf[fmt_len] = '\0';

    // Pass a dummy argument to satisfy the variadic requirement of safe_vmprintf
    // The format string might expect arguments, but we are fuzzing the string itself.
    // Passing a dummy string is safe for the va_list mechanism.
    formatted = safe_vmprintf(fmt_buf, "fuzz_test");
    sqlite3_free(fmt_buf);

    // 2. sqlite3_prepare_v2
    // Use the input data as SQL. Ensure null termination.
    char *sql_buf = (char *)sqlite3_malloc(Size + 1);
    if (!sql_buf) {
        if (formatted) sqlite3_free(formatted);
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    rc = sqlite3_prepare_v2(db, sql_buf, (int)Size, &stmt, &tail);
    sqlite3_free(sql_buf);

    if (rc != SQLITE_OK) {
        // If preparation fails, we still want to test error handling paths
        (void)sqlite3_extended_errcode(db);
        (void)sqlite3_errmsg(db);
        
        if (formatted) sqlite3_free(formatted);
        sqlite3_close(db);
        return 0;
    }

    // 3. sqlite3_extended_errcode (Check state before step, usually OK)
    (void)sqlite3_extended_errcode(db);

    // 4. sqlite3_errmsg (Check state before step)
    (void)sqlite3_errmsg(db);

    // 5. sqlite3_step
    // Execute the statement. Loop to explore multiple rows if any.
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Do nothing with the row, just iterate
    }

    // 6. sqlite3_reset
    // Reset the statement to try re-execution or just clean up state
    (void)sqlite3_reset(stmt);

    // 7. sqlite3_extended_errcode (Check state after reset)
    (void)sqlite3_extended_errcode(db);

    // 8. sqlite3_errmsg (Check state after reset)
    (void)sqlite3_errmsg(db);

    // 9. sqlite3_free
    // Free the memory allocated by sqlite3_vmprintf
    if (formatted) {
        sqlite3_free(formatted);
    }

    // 10. sqlite3_finalize
    // Destroy the prepared statement
    (void)sqlite3_finalize(stmt);

    // Cleanup database connection
    sqlite3_close(db);

    return 0;
}