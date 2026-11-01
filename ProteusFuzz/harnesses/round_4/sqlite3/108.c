// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:93828:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *zTail = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    // Create an in-memory database to avoid file I/O issues during fuzzing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Ensure the input data is treated as a C-string by ensuring it's null-terminated
    // We use a local buffer to safely handle the input
    char *sql_buffer = (char *)malloc(Size + 1);
    if (!sql_buffer) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_buffer, Data, Size);
    sql_buffer[Size] = '\0';

    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql_buffer, (int)Size, &stmt, &zTail);

    if (rc == SQLITE_OK || rc == SQLITE_DONE || rc == SQLITE_ROW) {
        // If preparation succeeded (or returned a valid state), try to explore column info
        if (stmt) {
            int col_count = sqlite3_column_count(stmt);
            
            // Explore column names and types
            for (int i = 0; i < col_count; i++) {
                const char *col_name = sqlite3_column_name(stmt, i);
                (void)col_name; // Suppress unused variable warning
                
                const char *col_decltype = sqlite3_column_decltype(stmt, i);
                (void)col_decltype; // Suppress unused variable warning
            }

            // Execute the statement to potentially get rows
            rc = sqlite3_step(stmt);
            
            // If we have rows, try to retrieve integer values
            if (rc == SQLITE_ROW) {
                int col_count_row = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count_row; i++) {
                    int val = sqlite3_column_int(stmt, i);
                    (void)val; // Suppress unused variable warning
                }
            }
            
            // Retrieve the original SQL to exercise that path
            const char *orig_sql = sqlite3_sql(stmt);
            (void)orig_sql; // Suppress unused variable warning
        }
    }

    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    free(sql_buffer);

    return 0;
}