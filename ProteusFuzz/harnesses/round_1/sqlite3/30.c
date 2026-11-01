// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_dummy_db(sqlite3 **ppDb) {
    const char *db_path = "./dummy_file";
    int rc = sqlite3_open(db_path, ppDb);
    if (rc != SQLITE_OK) {
        return -1;
    }
    // Initialize with a simple table to ensure valid schema for queries
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, value INTEGER);";
    sqlite3_exec(*ppDb, init_sql, NULL, NULL, NULL);
    return 0;
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    if (create_dummy_db(&db) != 0) {
        return 0;
    }

    // Ensure the input data is treated as a null-terminated string for SQL
    // We create a buffer that is guaranteed to be null-terminated
    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) {
        cleanup_db(db);
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    // Filter out non-printable characters or ensure it's valid UTF-8 if needed,
    // but for fuzzing raw input is often desired to trigger edge cases.
    // However, sqlite3_prepare_v2 expects valid SQL. We will let it fail gracefully.

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql_buf, Size, &stmt, NULL);

    if (rc == SQLITE_OK && stmt != NULL) {
        // 1. sqlite3_sql
        const char *sql_text = sqlite3_sql(stmt);
        (void)sql_text; // Use the return value to avoid unused warnings, logic is just to call it

        // 2. sqlite3_step
        rc = sqlite3_step(stmt);

        // 3. sqlite3_sql (again)
        sql_text = sqlite3_sql(stmt);
        (void)sql_text;

        // 4. sqlite3_column_int
        int col1 = sqlite3_column_int(stmt, 0);
        (void)col1;

        // 5. sqlite3_column_int (again)
        int col2 = sqlite3_column_int(stmt, 1);
        (void)col2;

        // 6. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // 7. sqlite3_db_release_memory
    if (db) {
        sqlite3_db_release_memory(db);
    }

    // Prepare a second statement for the next sequence
    // We reuse the same SQL buffer, or try a different one if Size allows, 
    // but for simplicity and coverage, we reuse the input.
    rc = sqlite3_prepare_v2(db, sql_buf, Size, &stmt, NULL);

    if (rc == SQLITE_OK && stmt != NULL) {
        // 8. sqlite3_sql
        const char *sql_text2 = sqlite3_sql(stmt);
        (void)sql_text2;

        // 9. sqlite3_step
        rc = sqlite3_step(stmt);

        // 10. sqlite3_sql (again)
        sql_text2 = sqlite3_sql(stmt);
        (void)sql_text2;

        // 11. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    free(sql_buf);
    cleanup_db(db);

    return 0;
}