// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_double at sqlite3.c:93656:19 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:93832:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:93686:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:93809:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int fuzz_sqlite3_column_functions(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int col_count = 0;
    double d_val;
    const void *v_val;
    int type_val;
    int fuzz_col;

    if (Size == 0) {
        return 0;
    }

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to ensure we have columns to query
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a safe query that works regardless of input content
    // We use a static query to avoid memory management issues with the input buffer
    const char *sql_query = "SELECT id, name, value FROM test_table;";
    
    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step through results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        col_count = sqlite3_column_count(stmt);
        if (col_count <= 0) {
            break;
        }

        // Fuzz column index using input data if available
        fuzz_col = Data[0] % col_count;

        // Call target API functions
        // 1. sqlite3_column_double
        d_val = sqlite3_column_double(stmt, fuzz_col);
        (void)d_val;

        // 2. sqlite3_column_decltype16
        v_val = sqlite3_column_decltype16(stmt, fuzz_col);
        (void)v_val;

        // 3. sqlite3_column_text16
        v_val = sqlite3_column_text16(stmt, fuzz_col);
        (void)v_val;

        // 4. sqlite3_column_name16
        v_val = sqlite3_column_name16(stmt, fuzz_col);
        (void)v_val;

        // 5. sqlite3_column_count
        col_count = sqlite3_column_count(stmt);
        (void)col_count;

        // 6. sqlite3_column_type
        type_val = sqlite3_column_type(stmt, fuzz_col);
        (void)type_val;

        // Also try invalid indices to test error handling
        int invalid_idx = col_count + 1;
        d_val = sqlite3_column_double(stmt, invalid_idx);
        v_val = sqlite3_column_decltype16(stmt, invalid_idx);
        v_val = sqlite3_column_text16(stmt, invalid_idx);
        v_val = sqlite3_column_name16(stmt, invalid_idx);
        type_val = sqlite3_column_type(stmt, invalid_idx);
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    return fuzz_sqlite3_column_functions(Data, Size);
}