// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Limit SQL size to prevent excessive memory usage or stack issues
    size_t sql_len = Size;
    if (sql_len > 4096) {
        sql_len = 4096;
    }

    // Create a safe SQL string from the fuzz input
    char *safe_sql = (char *)malloc(sql_len + 1);
    if (!safe_sql) {
        return 0;
    }
    memcpy(safe_sql, Data, sql_len);
    safe_sql[sql_len] = '\0';

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(safe_sql);
        return 0;
    }

    // 1. sqlite3_prepare_v2
    rc = sqlite3_prepare_v2(db, safe_sql, (int)sql_len, &stmt, &tail);
    
    // 2. sqlite3_errmsg
    // Check for errors after prepare
    if (rc != SQLITE_OK) {
        (void)sqlite3_errmsg(db);
        sqlite3_close(db);
        free(safe_sql);
        return 0;
    }

    // 3. sqlite3_step (First call)
    rc = sqlite3_step(stmt);

    // 4. sqlite3_step (Second call)
    // Explore state transitions
    if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
        rc = sqlite3_step(stmt);
    }

    // 5. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);

    // 6. sqlite3_column_type
    if (col_count > 0) {
        int col_idx = 0;
        (void)sqlite3_column_type(stmt, col_idx);
    }

    // 7. sqlite3_column_name
    if (col_count > 0) {
        (void)sqlite3_column_name(stmt, 0);
    }

    // 8, 9, 10. sqlite3_column_text (Three calls)
    if (col_count > 0) {
        (void)sqlite3_column_text(stmt, 0);
        int idx2 = (col_count > 1) ? 1 : 0;
        (void)sqlite3_column_text(stmt, idx2);
        int idx3 = (col_count > 2) ? 2 : 0;
        (void)sqlite3_column_text(stmt, idx3);
    }

    // 11. sqlite3_column_bytes
    if (col_count > 0) {
        (void)sqlite3_column_bytes(stmt, 0);
    }

    // 12. sqlite3_finalize
    if (stmt) {
        sqlite3_finalize(stmt);
    }

    // Cleanup
    sqlite3_close(db);
    free(safe_sql);

    return 0;
}