// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_sqlite3_sequence(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int param_count = 0;
    int col_count = 0;
    const char *sql_text = NULL;
    sqlite3_str *str = NULL;
    char *final_str = NULL;

    // 1. Prepare the environment
    // Create an in-memory database to avoid file system dependencies
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Ensure we have some data to use as SQL if Size > 0
    // We will try to prepare a statement from the fuzz input if it looks like SQL
    // Otherwise, use a safe default to ensure the statement is valid for the API calls
    const char *sql_input = (Size > 0 && Size < 10000) ? (const char *)Data : "SELECT 1";
    
    // Try to prepare a statement. 
    // We use a subset of the input or a default to ensure we get a valid statement for testing.
    // If the input is garbage, sqlite3_prepare_v2 will fail, which is also a valid state to test.
    rc = sqlite3_prepare_v2(db, sql_input, Size > 0 ? (int)Size : -1, &stmt, NULL);
    
    // Even if prepare fails, we might have a NULL stmt. The APIs handle NULL gracefully.
    // However, to explore more states, if prepare fails, we try a simple valid statement.
    if (rc != SQLITE_OK && stmt == NULL) {
        rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, NULL);
    }

    // 2. Invoke the target functions in the specified order

    // Step 1: sqlite3_db_handle
    // Retrieves the database connection handle for the prepared statement.
    sqlite3 *handle = sqlite3_db_handle(stmt);
    (void)handle; // Use the handle to avoid unused variable warnings, logic is implicit

    // Step 2: sqlite3_column_count
    // Returns the number of columns in the result set.
    if (stmt != NULL) {
        col_count = sqlite3_column_count(stmt);
    }

    // Step 3: sqlite3_bind_parameter_count
    // Returns the highest parameter index.
    if (stmt != NULL) {
        param_count = sqlite3_bind_parameter_count(stmt);
    }

    // Step 4: sqlite3_sql
    // Retrieves the original SQL text.
    if (stmt != NULL) {
        sql_text = sqlite3_sql(stmt);
        // Just access the pointer to ensure it's valid if stmt is valid
        if (sql_text) {
            // Do nothing with it, just retrieving is the test
        }
    }

    // Step 5: sqlite3_str_new
    // Allocates a dynamic string object.
    // We pass the db handle retrieved earlier (or NULL if handle was NULL, though unlikely here)
    str = sqlite3_str_new(handle ? handle : db);
    
    if (str != NULL) {
        // Optional: Append some data to exercise the string object further
        // We use the fuzz data if available
        if (Size > 0) {
            // We can't blindly append binary data to a string, but we can try to append a safe subset
            // or just call finish to test the cleanup path.
            // Let's try to append a small portion if it looks printable, otherwise just finish.
            if (Size > 0) {
                // Simple append test
                sqlite3_str_append(str, (const char *)Data, Size > 100 ? 100 : Size);
            }
        }
        
        // Always finish the string to free resources
        final_str = sqlite3_str_finish(str);
        if (final_str != NULL) {
            sqlite3_free(final_str);
        }
    }

    // 3. Cleanup
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Wrap logic in the entry point
    fuzz_sqlite3_sequence(Data, Size);
    return 0;
}