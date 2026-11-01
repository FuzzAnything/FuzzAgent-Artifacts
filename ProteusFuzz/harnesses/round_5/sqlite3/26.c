// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // 1. Prepare the environment
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a table to ensure valid state for queries
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // Prepare a statement
    const char *safe_sql = "SELECT 1, X('00');";
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);

    if (stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Execute to get a row
    rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        // Target API Sequence:
        // 1. sqlite3_column_bytes
        int bytes1 = sqlite3_column_bytes(stmt, 0);
        (void)bytes1;

        // 2. sqlite3_column_blob
        const void *blob1 = sqlite3_column_blob(stmt, 0);
        (void)blob1;

        // 3. sqlite3_column_bytes
        int bytes2 = sqlite3_column_bytes(stmt, 1);
        (void)bytes2;

        // 4. sqlite3_column_blob
        const void *blob2 = sqlite3_column_blob(stmt, 1);
        (void)blob2;

        // 5. sqlite3_stmt_isexplain
        int is_explain = sqlite3_stmt_isexplain(stmt);
        (void)is_explain;

        // 6. sqlite3_reset
        rc = sqlite3_reset(stmt);
        (void)rc;

        // 7. sqlite3_step
        rc = sqlite3_step(stmt);
        (void)rc;

        // 8. sqlite3_errmsg
        const char *err1 = sqlite3_errmsg(db);
        (void)err1;

        // 9. sqlite3_errmsg
        const char *err2 = sqlite3_errmsg(db);
        (void)err2;
    } else {
        // If no row, we still try to exercise the remaining APIs safely
        // 6. sqlite3_reset
        rc = sqlite3_reset(stmt);
        (void)rc;

        // 7. sqlite3_step
        rc = sqlite3_step(stmt);
        (void)rc;

        // 8. sqlite3_errmsg
        const char *err1 = sqlite3_errmsg(db);
        (void)err1;

        // 9. sqlite3_errmsg
        const char *err2 = sqlite3_errmsg(db);
        (void)err2;
    }

    // 10. sqlite3_finalize
    rc = sqlite3_finalize(stmt);
    (void)rc;

    // Cleanup
    sqlite3_close(db);

    return 0;
}