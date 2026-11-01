// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = (const char *)Data;
    const char *tail = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Ensure the input is treated as a null-terminated string for sqlite3_prepare_v2
    // We create a temporary buffer to ensure null termination if the fuzzer input lacks it
    char *safe_sql = (char *)malloc(Size + 1);
    if (!safe_sql) {
        return 0;
    }
    memcpy(safe_sql, Data, Size);
    safe_sql[Size] = '\0';

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(safe_sql);
        return 0;
    }

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, safe_sql, (int)Size, &stmt, &tail);
    
    // Invoke sqlite3_errmsg as requested in the sequence
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    if (rc != SQLITE_OK) {
        // If preparation fails, finalize (safe on NULL) and cleanup
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free(safe_sql);
        return 0;
    }

    // Execute the statement step-by-step (first call)
    rc = sqlite3_step(stmt);
    
    // Execute again (second call) to explore state transitions
    rc = sqlite3_step(stmt);

    // Get column count
    int col_count = sqlite3_column_count(stmt);
    
    // Iterate over columns to exercise column access functions
    // Only proceed if we have columns
    if (col_count > 0) {
        for (int i = 0; i < col_count; i++) {
            // Check column type
            int col_type = sqlite3_column_type(stmt, i);
            (void)col_type;

            // Get column name
            const char *col_name = sqlite3_column_name(stmt, i);
            (void)col_name;

            // Get column text (called 3 times as per request)
            const unsigned char *text1 = sqlite3_column_text(stmt, i);
            const unsigned char *text2 = sqlite3_column_text(stmt, i);
            const unsigned char *text3 = sqlite3_column_text(stmt, i);
            (void)text1;
            (void)text2;
            (void)text3;

            // Get column bytes
            int col_bytes = sqlite3_column_bytes(stmt, i);
            (void)col_bytes;
        }
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    free(safe_sql);

    return 0;
}