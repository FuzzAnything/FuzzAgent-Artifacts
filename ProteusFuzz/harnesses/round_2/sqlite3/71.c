// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
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

static int fuzz_sqlite3_column_functions(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to work with
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Use fuzzing input as SQL query if possible, or fallback to safe queries
    char *sql_query = NULL;
    if (Size > 0) {
        // Ensure null termination
        sql_query = (char *)malloc(Size + 1);
        if (!sql_query) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql_query, Data, Size);
        sql_query[Size] = '\0';
        
        // Basic sanitization: only allow SELECT statements for safety
        if (strncasecmp(sql_query, "SELECT", 6) == 0) {
            // Proceed with user input
        } else {
            // Fallback to safe query
            free(sql_query);
            sql_query = strdup("SELECT id, name, value, data FROM test_table LIMIT 1;");
        }
    } else {
        sql_query = strdup("SELECT id, name, value, data FROM test_table LIMIT 1;");
    }

    if (!sql_query) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    free(sql_query);

    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    
    // Get column count
    int col_count = sqlite3_column_count(stmt);
    if (col_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Fuzz column index based on input data
    int fuzz_col_idx = 0;
    if (Size > 0) {
        fuzz_col_idx = Data[0] % col_count;
    }

    // Call target API functions
    if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
        // sqlite3_column_type
        int col_type = sqlite3_column_type(stmt, fuzz_col_idx);
        (void)col_type; // Suppress unused variable warning

        // sqlite3_column_double
        double col_double = sqlite3_column_double(stmt, fuzz_col_idx);
        (void)col_double;

        // sqlite3_column_decltype16
        const void *col_decltype16 = sqlite3_column_decltype16(stmt, fuzz_col_idx);
        (void)col_decltype16;

        // sqlite3_column_name16
        const void *col_name16 = sqlite3_column_name16(stmt, fuzz_col_idx);
        (void)col_name16;

        // sqlite3_column_text16
        const void *col_text16 = sqlite3_column_text16(stmt, fuzz_col_idx);
        (void)col_text16;
    }

    // Finalize and cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file if needed (not used in this implementation but prepared)
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    return fuzz_sqlite3_column_functions(Data, Size);
}