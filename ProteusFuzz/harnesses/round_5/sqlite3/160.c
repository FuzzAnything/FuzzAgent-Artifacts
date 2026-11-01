// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
 * Fuzz driver for sqlite3_value_* functions.
 * Since sqlite3_value functions operate on protected sqlite3_value objects
 * typically passed to user-defined SQL functions, we create a custom SQL
 * function that receives these values and exercises the target APIs.
 */

/*
 * Custom SQL function implementation that exercises the target APIs.
 * It receives arguments and calls the fuzzing logic on them.
 */
static void fuzzing_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    
    /* Iterate over all arguments passed to the SQL function */
    for (i = 0; i < argc; i++) {
        sqlite3_value *val = argv[i];
        if (val == NULL) continue;

        /* 1. sqlite3_value_text16be */
        const void *text16be = sqlite3_value_text16be(val);
        (void)text16be; /* Suppress unused variable warning, usage is side-effect free check */

        /* 2. sqlite3_value_subtype */
        unsigned int subtype = sqlite3_value_subtype(val);
        (void)subtype;

        /* 3. sqlite3_value_text16 */
        const void *text16 = sqlite3_value_text16(val);
        (void)text16;

        /* 4. sqlite3_value_bytes */
        int bytes = sqlite3_value_bytes(val);
        (void)bytes;

        /* 5. sqlite3_value_blob */
        const void *blob = sqlite3_value_blob(val);
        (void)blob;

        /* 6. sqlite3_value_text16le */
        const void *text16le = sqlite3_value_text16le(val);
        (void)text16le;
    }
    
    /* Return a dummy result to keep the function valid */
    sqlite3_result_int(ctx, 0);
}

/*
 * Helper to safely open a database and register the function.
 */
static int setup_db_and_func(sqlite3 **db_out) {
    sqlite3 *db = NULL;
    int rc;

    /* Create an in-memory database for fuzzing */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    /* Register the custom function with SQLITE_SUBTYPE flag to enable subtype testing */
    rc = sqlite3_create_function(
        db,
        "fuzz_func",
        -1, /* Variable number of arguments */
        SQLITE_UTF8 | SQLITE_SUBTYPE, /* Enable subtype support */
        NULL, /* No application data */
        fuzzing_func,
        NULL, /* xStep */
        NULL  /* xFinal */
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    *db_out = db;
    return 0;
}

/*
 * Helper to generate SQL statements from fuzzing input.
 * This attempts to create various data types (text, blob, integers)
 * to exercise the value extraction functions.
 */
static void fuzz_with_input(sqlite3 *db, const uint8_t *data, size_t size) {
    if (db == NULL || data == NULL || size == 0) return;

    sqlite3_stmt *stmt;
    int rc;
    char sql[256];
    int num_args = 0;
    const char *args[10]; /* Max 10 arguments for safety */
    sqlite3_int64 int_vals[10];
    const char *blob_ptrs[10];
    int blob_sizes[10];

    /* 
     * Strategy:
     * 1. Use the input data to construct a SQL statement with multiple arguments.
     * 2. Bind different types of data (text, blob, integers) to these arguments.
     * 3. Execute the statement to trigger the custom function.
     */

    /* Limit the number of arguments to avoid stack overflow or resource exhaustion */
    int max_args = (size > 10) ? 10 : (size > 0) ? 1 : 0;
    
    /* Construct SQL: SELECT fuzz_func(arg1, arg2, ...) */
    snprintf(sql, sizeof(sql), "SELECT fuzz_func(");
    for (int i = 0; i < max_args; i++) {
        if (i > 0) strcat(sql, ", ");
        strcat(sql, "?");
    }
    strcat(sql, ")");

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return;

    /* Bind values based on input data */
    for (int i = 0; i < max_args; i++) {
        int idx = i + 1;
        
        /* Alternate between types based on input byte */
        if (i < size) {
            uint8_t b = data[i];
            
            if (b % 3 == 0) {
                /* Bind as INTEGER */
                sqlite3_int64 val = (sqlite3_int64)(b << 24 | b << 16 | b << 8 | b);
                sqlite3_bind_int64(stmt, idx, val);
            } else if (b % 3 == 1) {
                /* Bind as BLOB */
                int blob_len = (b % 100) + 1;
                if (i + blob_len > size) blob_len = size - i;
                if (blob_len > 0) {
                    sqlite3_bind_blob(stmt, idx, data + i, blob_len, SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, idx);
                }
            } else {
                /* Bind as TEXT */
                int text_len = (b % 100) + 1;
                if (i + text_len > size) text_len = size - i;
                if (text_len > 0) {
                    /* Ensure null termination for safety, though bind_text handles length */
                    sqlite3_bind_text(stmt, idx, (const char*)(data + i), text_len, SQLITE_TRANSIENT);
                } else {
                    sqlite3_bind_null(stmt, idx);
                }
            }
        } else {
            sqlite3_bind_null(stmt, idx);
        }
    }

    /* Step through the statement to execute the function */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* The function is called during step, we just consume the result */
    }

    sqlite3_finalize(stmt);
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;

    /* Setup database and register function */
    if (setup_db_and_func(&db) != 0) {
        return 0;
    }

    /* Fuzz with the provided input */
    fuzz_with_input(db, Data, Size);

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}