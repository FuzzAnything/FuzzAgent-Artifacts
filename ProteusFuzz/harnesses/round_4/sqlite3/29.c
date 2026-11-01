// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_test_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return 0;
    }
    return 1;
}

static int prepare_statement(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        *stmt = NULL;
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_text = NULL;
    int col_int1 = 0;
    int col_int2 = 0;
    int rc;

    if (Size == 0) {
        return 0;
    }

    if (!create_test_db(&db)) {
        return 0;
    }

    // Create a simple table to ensure sqlite3_step can return a row
    const char *setup_sql = "CREATE TABLE test_table (id INTEGER, val INTEGER);";
    sqlite3_exec(db, setup_sql, NULL, NULL, NULL);

    // Prepare a statement using the fuzzed input if it looks like SQL, otherwise use a safe default
    // We ensure the input is null-terminated for safety
    char *safe_sql = (char *)malloc(Size + 1);
    if (!safe_sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(safe_sql, Data, Size);
    safe_sql[Size] = '\0';

    // Basic heuristic to check if input starts with valid SQL keywords to avoid obvious garbage
    // If not, we use a default SELECT to ensure we can test the column_int functions
    const char *target_sql = safe_sql;
    int is_valid_sql = 0;
    size_t len = strlen(safe_sql);
    if (len > 0) {
        char upper_first = safe_sql[0];
        if (upper_first >= 'a' && upper_first <= 'z') upper_first -= 32;
        if (upper_first == 'S' || upper_first == 'I' || upper_first == 'U' || upper_first == 'D') {
            is_valid_sql = 1;
        }
    }

    if (!is_valid_sql) {
        target_sql = "SELECT 1, 2;";
    }

    // 1. sqlite3_sql (on NULL stmt to test error handling or valid stmt later)
    // First, let's try to prepare the statement
    if (prepare_statement(db, target_sql, &stmt)) {
        // 2. sqlite3_sql (Call 1)
        sql_text = sqlite3_sql(stmt);
        
        // 3. sqlite3_step
        rc = sqlite3_step(stmt);
        
        // 4. sqlite3_sql (Call 2)
        sql_text = sqlite3_sql(stmt);
        
        // 5. sqlite3_column_int (Call 1)
        if (rc == SQLITE_ROW) {
            col_int1 = sqlite3_column_int(stmt, 0);
        }
        
        // 6. sqlite3_column_int (Call 2)
        if (rc == SQLITE_ROW) {
            col_int2 = sqlite3_column_int(stmt, 1);
        }
        
        // 7. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    } else {
        // If prepare failed, we still need to follow the sequence roughly or skip safely
        // The task says invoke in order. If stmt is NULL, sqlite3_sql returns NULL.
        // Let's create a dummy valid stmt to ensure the flow happens if the first failed
        if (!prepare_statement(db, "SELECT 0, 0;", &stmt)) {
            sqlite3_close(db);
            free(safe_sql);
            return 0;
        }
        
        // Retry sequence with valid stmt
        // 1. sqlite3_sql
        sql_text = sqlite3_sql(stmt);
        // 2. sqlite3_step
        rc = sqlite3_step(stmt);
        // 3. sqlite3_sql
        sql_text = sqlite3_sql(stmt);
        // 4. sqlite3_column_int
        if (rc == SQLITE_ROW) col_int1 = sqlite3_column_int(stmt, 0);
        // 5. sqlite3_column_int
        if (rc == SQLITE_ROW) col_int2 = sqlite3_column_int(stmt, 1);
        // 6. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // 8. sqlite3_db_release_memory
    sqlite3_db_release_memory(db);

    // Second sequence: sqlite3_sql -> sqlite3_step -> sqlite3_sql -> sqlite3_finalize
    // Prepare a new statement for the second part of the sequence
    if (prepare_statement(db, target_sql, &stmt)) {
        // 9. sqlite3_sql
        sql_text = sqlite3_sql(stmt);
        
        // 10. sqlite3_step
        rc = sqlite3_step(stmt);
        
        // 11. sqlite3_sql
        sql_text = sqlite3_sql(stmt);
        
        // 12. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    } else {
        // Fallback if prepare fails again
        if (prepare_statement(db, "SELECT 0;", &stmt)) {
            sqlite3_sql(stmt);
            sqlite3_step(stmt);
            sqlite3_sql(stmt);
            sqlite3_finalize(stmt);
        }
    }

    sqlite3_close(db);
    free(safe_sql);
    return 0;
}