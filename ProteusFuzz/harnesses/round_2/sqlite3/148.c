// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Static helper to safely finalize a statement if it is not NULL.
 */
static void safe_finalize(sqlite3_stmt *pStmt) {
    if (pStmt) {
        sqlite3_finalize(pStmt);
    }
}

/*
 * Static helper to safely close a database connection if it is not NULL.
 */
static void safe_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    const char *pzTail = NULL;
    int rc;

    /*
     * Step 1: Prepare the environment.
     * Open an in-memory database to avoid file system dependencies.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Create a simple table to ensure the SQL statements we prepare are valid
     * and have parameters to bind.
     */
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value BLOB);";
    rc = sqlite3_exec(db, init_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        safe_close(db);
        return 0;
    }

    /*
     * Step 2: Process the fuzz input to generate SQL and binding data.
     * We interpret the input data as a stream of bytes to construct SQL strings
     * and binding values.
     */
    
    /* 
     * We need a valid SQL string with parameters to test the bind functions.
     * We will construct a simple INSERT or SELECT statement dynamically.
     * To ensure safety, we limit the length of the SQL string we extract from Data.
     */
    size_t sql_len = 0;
    if (Size > 0) {
        /* Use a portion of the input as the SQL string, ensuring null termination */
        if (Size > 256) sql_len = 256;
        else sql_len = Size;
    }

    /* 
     * Construct a safe SQL template with parameters to ensure we have something to bind to.
     * We will use the input data as the value for the parameter, but the SQL structure
     * itself will be controlled to ensure it's valid syntax.
     */
    char sql_buf[512];
    /* Default to a safe query if input is too short or invalid */
    const char *safe_sql = "INSERT INTO test_table (name, value) VALUES (:name, :value);";
    
    if (Size > 0 && sql_len > 0) {
        /* 
         * We will try to use the input data as the SQL text, but we must sanitize it
         * or use a template. For fuzzing, using a template with the input as data
         * is often safer and more effective for hitting bind logic.
         * However, to test sqlite3_prepare with fuzzed SQL, we can try to prepare
         * the raw input if it looks like SQL, but fallback to a known good statement.
         * Here, we will use a known good statement to ensure pStmt is valid for bind tests.
         */
        snprintf(sql_buf, sizeof(sql_buf), "INSERT INTO test_table (name, value) VALUES (%s, %s);", 
                 (Size > 10) ? "?, ?" : "?, ?"); 
        /* Actually, let's stick to named parameters to test sqlite3_bind_parameter_name/index */
        snprintf(sql_buf, sizeof(sql_buf), "INSERT INTO test_table (name, value) VALUES (:name, :value);");
    } else {
        snprintf(sql_buf, sizeof(sql_buf), "INSERT INTO test_table (name, value) VALUES (:name, :value);");
    }

    /*
     * Step 3: Invoke sqlite3_prepare_v3 to get a prepared statement.
     * We use v3 to test that API, but we could also use sqlite3_prepare.
     */
    rc = sqlite3_prepare_v3(db, sql_buf, -1, 0, &pStmt, &pzTail);
    if (rc != SQLITE_OK || pStmt == NULL) {
        /* 
         * If preparation fails, we might still want to test with a different SQL.
         * For robustness, we try a simpler query if the first one fails.
         */
        const char *fallback_sql = "SELECT * FROM test_table WHERE name = :name;";
        rc = sqlite3_prepare_v3(db, fallback_sql, -1, 0, &pStmt, &pzTail);
        if (rc != SQLITE_OK || pStmt == NULL) {
            safe_close(db);
            return 0;
        }
    }

    /*
     * Step 4: Explore program states by invoking the target API functions.
     */

    /* 4.1: Test sqlite3_bind_parameter_count */
    int param_count = sqlite3_bind_parameter_count(pStmt);
    (void)param_count; /* Suppress unused variable warning if not used in logic */

    /* 4.2: Test sqlite3_bind_parameter_name */
    /* Iterate through possible parameter indices to trigger different code paths */
    for (int i = 1; i <= param_count + 2; i++) {
        const char *name = sqlite3_bind_parameter_name(pStmt, i);
        /* name can be NULL if index is out of range or anonymous */
        (void)name;
    }

    /* 4.3: Test sqlite3_bind_parameter_index */
    /* Try to find indices for known and unknown names */
    int idx1 = sqlite3_bind_parameter_index(pStmt, ":name");
    int idx2 = sqlite3_bind_parameter_index(pStmt, ":value");
    int idx3 = sqlite3_bind_parameter_index(pStmt, ":unknown_param");
    int idx4 = sqlite3_bind_parameter_index(pStmt, NULL);
    
    (void)idx1; (void)idx2; (void)idx3; (void)idx4;

    /* 4.4: Test sqlite3_bind_text64 */
    /* Prepare data to bind from the fuzz input */
    if (Size > 0) {
        /* Use the input data as the text to bind */
        const char *bind_text = (const char *)Data;
        sqlite3_uint64 bind_len = (sqlite3_uint64)Size;
        
        /* Ensure we don't bind more than the buffer size if we were copying, 
           but here we point directly to Data which is valid for the fuzzer lifetime. */
        
        /* Bind to :name if it exists */
        if (idx1 > 0) {
            rc = sqlite3_bind_text64(pStmt, idx1, bind_text, bind_len, SQLITE_STATIC, SQLITE_UTF8);
            if (rc != SQLITE_OK) {
                /* Error handling: reset or continue */
            }
        }

        /* Bind to :value if it exists */
        if (idx2 > 0) {
            /* Try binding with different encodings or lengths to explore states */
            rc = sqlite3_bind_text64(pStmt, idx2, bind_text, bind_len, SQLITE_STATIC, SQLITE_UTF8);
            if (rc != SQLITE_OK) {
                /* Ignore errors for fuzzing exploration */
            }
        }
    }

    /*
     * Step 5: Cleanup.
     * Finalize the statement and close the database.
     */
    safe_finalize(pStmt);
    safe_close(db);

    return 0;
}