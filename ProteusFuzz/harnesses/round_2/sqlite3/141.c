// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3_value_* and sqlite3_result_* functions.
 * These functions are designed to be called within the context of a
 * user-defined SQL function. To fuzz them, we must:
 * 1. Open a database connection.
 * 2. Register a custom SQL function.
 * 3. Execute a SQL statement that invokes this function with various inputs.
 * 4. Inside the function callback, invoke the target APIs using the fuzzed data.
 */

static sqlite3 *g_db = NULL;

/*
 * Custom SQL function implementation.
 * This function receives sqlite3_context and sqlite3_value* arguments.
 * We will use the fuzzed input to populate arguments or simulate behavior.
 */
static void fuzz_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (!context || !argv) return;

    /*
     * We iterate through the arguments passed to this function.
     * In a real scenario, these arguments come from the SQL query.
     * Since we control the SQL query in the fuzzer, we can pass different types.
     * However, to maximize coverage with the raw fuzz input, we might also
     * construct values manually if needed, but here we rely on the SQL engine
     * to pass the values based on the query we execute.
     */
    for (int i = 0; i < argc; i++) {
        sqlite3_value *val = argv[i];
        if (!val) continue;

        /*
         * Target API: sqlite3_value_bytes
         * Returns the byte length of the value as UTF-8.
         */
        int bytes = sqlite3_value_bytes(val);
        (void)bytes; /* Use return value to prevent unused warnings, logic handled by engine */

        /*
         * Target API: sqlite3_value_blob
         * Retrieves BLOB data. Returns pointer or NULL.
         */
        const void *blob = sqlite3_value_blob(val);
        if (blob) {
            /* Do not dereference blindly, just check validity */
        }

        /*
         * Target API: sqlite3_value_text
         * Retrieves UTF-8 text.
         */
        const unsigned char *text = sqlite3_value_text(val);
        if (text) {
            /* Text is valid until callback returns */
        }

        /*
         * Target API: sqlite3_value_int64
         * Extracts 64-bit integer.
         */
        sqlite3_int64 int_val = sqlite3_value_int64(val);
        (void)int_val;

        /*
         * Target API: sqlite3_value_encoding
         * Reveals internal text encoding.
         */
        int encoding = sqlite3_value_encoding(val);
        (void)encoding;
    }

    /*
     * Target API: sqlite3_result_int64
     * Sets the result of the function.
     * We use a deterministic value based on argc to ensure the function returns something.
     * This tests the result setting path.
     */
    sqlite3_result_int64(context, (sqlite3_int64)argc);
}

/*
 * Helper to safely open a database in memory for fuzzing.
 */
static int init_db(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }

    if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
        return -1;
    }

    /* Register the fuzz function */
    if (sqlite3_create_function(g_db, "fuzz_func", -1, SQLITE_UTF8, NULL, fuzz_function, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return -1;
    }

    return 0;
}

/*
 * Helper to cleanup database.
 */
static void cleanup_db(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}

/*
 * Entry point for the fuzzer.
 * Data: Pointer to fuzzed input buffer.
 * Size: Size of the fuzzed input buffer.
 */
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    /* Initialize database connection if not already done */
    if (!g_db) {
        if (init_db() != 0) {
            return 0;
        }
    }

    /*
     * We need to construct a SQL query that calls our 'fuzz_func'.
     * The fuzzed data (Data, Size) can be used to generate arguments.
     * Since we cannot directly pass raw binary as SQL arguments without escaping,
     * we will use the data to influence the SQL string construction or
     * simply execute a query that passes the data as a blob/text literal.
     *
     * Strategy:
     * 1. Create a table to hold fuzzed data if needed, or pass directly.
     * 2. Construct a query like: SELECT fuzz_func(X, Y, Z)...
     * 3. Use the fuzzed bytes to create the arguments.
     */

    sqlite3_stmt *stmt = NULL;
    int rc;

    /*
     * We will try to create a query that passes the fuzzed data as arguments.
     * To avoid SQL injection issues or syntax errors from arbitrary bytes,
     * we will bind the data to the statement rather than embedding it in the SQL string.
     *
     * Query: SELECT fuzz_func(?, ?, ?)
     * We will bind up to 3 arguments from the fuzzed input.
     */

    const char *sql = "SELECT fuzz_func(?, ?, ?)";

    rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        /* Prepare failed, try to recover or skip */
        sqlite3_finalize(stmt);
        return 0;
    }

    /*
     * Bind data from the fuzz input to the statement.
     * We try to bind different types to exercise different code paths in the callback.
     */

    /* Argument 1: Bind as BLOB if possible */
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_null(stmt, 1);
    }

    /* Argument 2: Bind as TEXT (if valid UTF-8, otherwise treat as blob or null) */
    /* For simplicity in fuzzing, we bind a subset as text if it looks safe, or just null */
    /* Let's bind a small chunk as text if Size is large enough */
    if (Size > 10) {
        /* Check for null bytes to ensure valid C-string for TEXT binding */
        int has_null = 0;
        for (size_t k = 0; k < 10; k++) {
            if (Data[k] == 0) {
                has_null = 1;
                break;
            }
        }
        if (!has_null) {
            sqlite3_bind_text(stmt, 2, (const char *)Data, 10, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
    } else {
        sqlite3_bind_null(stmt, 2);
    }

    /* Argument 3: Bind as INTEGER */
    sqlite3_int64 int_val = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&int_val, Data + (Size > sizeof(sqlite3_int64) ? Size - sizeof(sqlite3_int64) : 0), sizeof(sqlite3_int64));
        sqlite3_bind_int64(stmt, 3, int_val);
    } else {
        sqlite3_bind_int(stmt, 3, Size);
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /*
     * rc can be SQLITE_ROW (success), SQLITE_DONE (no rows), or error.
     * If SQLITE_ROW, the function was called.
     * We don't necessarily need to fetch the result, the side effect is the callback execution.
     */

    /* Cleanup statement */
    sqlite3_finalize(stmt);

    /*
     * Additional exploration:
     * Try to create a table and insert fuzzed data to see if it triggers
     * different internal states when queried.
     */
    if (Size > 0) {
        /* Create a temporary table */
        const char *create_sql = "CREATE TEMP TABLE fuzz_test (id INTEGER PRIMARY KEY, data BLOB)";
        sqlite3_exec(g_db, create_sql, NULL, NULL, NULL);

        /* Insert data */
        const char *insert_sql = "INSERT INTO fuzz_test (data) VALUES (?)";
        sqlite3_stmt *insert_stmt = NULL;
        if (sqlite3_prepare_v2(g_db, insert_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_blob(insert_stmt, 1, Data, Size, SQLITE_TRANSIENT);
            sqlite3_step(insert_stmt);
            sqlite3_finalize(insert_stmt);

            /* Select and call function on the stored data */
            const char *select_sql = "SELECT fuzz_func(data) FROM fuzz_test LIMIT 1";
            sqlite3_stmt *select_stmt = NULL;
            if (sqlite3_prepare_v2(g_db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
                sqlite3_step(select_stmt);
                sqlite3_finalize(select_stmt);
            }
        }

        /* Drop table */
        sqlite3_exec(g_db, "DROP TABLE IF EXISTS fuzz_test", NULL, NULL, NULL);
    }

    return 0;
}