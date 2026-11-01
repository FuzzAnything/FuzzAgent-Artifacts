// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
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

/* 
 * Custom SQL function to trigger the target API sequence.
 * This function receives arguments, converts them to text/blob,
 * checks their size, and returns an integer result.
 */
static void fuzz_target_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc < 1) {
        /* If no arguments, return a default value */
        sqlite3_result_int64(context, 0);
        return;
    }

    sqlite3_value *val = argv[0];

    /* 1. Invoke sqlite3_value_text */
    /* Note: This may convert the value to text internally */
    const unsigned char *text_val = sqlite3_value_text(val);
    (void)text_val; /* Use the pointer to ensure the call is made, ignore result for fuzzing */

    /* 2. Invoke sqlite3_value_blob */
    /* Note: This may convert the value to blob internally */
    const void *blob_val = sqlite3_value_blob(val);
    (void)blob_val; /* Use the pointer to ensure the call is made */

    /* 3. Invoke sqlite3_value_bytes */
    /* Note: This returns the byte length of the current representation */
    int bytes_len = sqlite3_value_bytes(val);
    (void)bytes_len; /* Use the result */

    /* 4. Invoke sqlite3_result_int64 */
    /* Set the result based on the length or a fixed value to explore states */
    sqlite3_int64 result_val = (sqlite3_int64)bytes_len;
    sqlite3_result_int64(context, result_val);
}

/*
 * Fuzz driver entry point.
 * Creates a database, registers the custom function, and executes SQL
 * using the provided fuzzing data.
 */
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function that calls the target APIs */
    rc = sqlite3_create_function(db, "fuzz_target", -1, SQLITE_UTF8, NULL, fuzz_target_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to store fuzz data if needed, or just use the function directly */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Explore program states by invoking the target function diversely.
     * We will construct SQL statements using the fuzz input.
     * We sanitize the input slightly to avoid trivial SQL syntax errors 
     * that prevent the function from being called, but keep the data content raw.
     */
    
    if (Size > 0) {
        /* Strategy 1: Pass data as a BLOB literal to the function */
        /* We construct a query like: SELECT fuzz_target(X'...') */
        /* To avoid hex encoding overhead, we can use the input as a string or blob directly via binding */
        
        const char *sql = "SELECT fuzz_target(?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            /* Bind the fuzz data as a BLOB */
            sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_STATIC);
            
            /* Step the statement to execute the function */
            rc = sqlite3_step(stmt);
            (void)rc; /* We don't care about the result, just the execution path */
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }

        /* Strategy 2: Insert data into a table and select it (different code path for value retrieval) */
        /* This might trigger different internal states for the value object */
        const char *insert_sql = "INSERT INTO test_table (data) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }

        /* Strategy 3: Select the inserted data and pass it to the function */
        const char *select_sql = "SELECT fuzz_target(data) FROM test_table LIMIT 1;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(stmt);
            (void)rc;
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /* 2. Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    /* Drop the table and close the database */
    sqlite3_exec(db, "DROP TABLE IF EXISTS test_table;", NULL, NULL, NULL);
    sqlite3_close(db);

    return 0;
}