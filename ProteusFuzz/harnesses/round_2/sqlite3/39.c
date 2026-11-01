// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_aggregate_context, sqlite3_value_bytes, and sqlite3_value_text.
 *
 * Strategy:
 * 1. Create an in-memory database.
 * 2. Define a custom aggregate function that uses the target APIs in xStep and xFinal.
 * 3. Execute a query using this aggregate function with fuzzed SQL input.
 * 4. Clean up resources.
 */

static void agg_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Call sqlite3_aggregate_context to get or allocate state */
    void *ctx = sqlite3_aggregate_context(context, 1024);
    (void)ctx; /* Use the return value to prevent unused warning */

    /* We need an sqlite3_value to call the other functions.
     * In a real aggregate, we would use one of the arguments (argv[i]).
     * Here, we simulate by checking if we have arguments.
     * If no arguments are provided, we cannot safely call value_bytes/text.
     */
    if (argc > 0 && argv[0] != NULL) {
        /* Call sqlite3_value_bytes */
        int bytes = sqlite3_value_bytes(argv[0]);
        (void)bytes; /* Use the return value to prevent unused warning */

        /* Call sqlite3_value_text */
        const unsigned char *text = sqlite3_value_text(argv[0]);
        (void)text; /* Use the return value to prevent unused warning */
    }
}

static void agg_final(sqlite3_context *context) {
    /* Call sqlite3_aggregate_context with 0 bytes in xFinal as per documentation */
    void *ctx = sqlite3_aggregate_context(context, 0);
    (void)ctx; /* Use the return value to prevent unused warning */
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Register the custom aggregate function */
    /* 
       Signature: sqlite3_create_function_v2(
         sqlite3 *db,
         const char *zFunctionName,
         int nArg,
         int eTextRep,
         void *pApp,
         void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
         void (*xStep)(sqlite3_context*,int,sqlite3_value**),
         void (*xFinal)(sqlite3_context*),
         void(*xDestroy)(void*)
       )
    */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_agg",
        -1, /* nArg = -1 for variable arguments */
        SQLITE_UTF8,
        NULL, /* pApp */
        NULL, /* xFunc (scalar function callback, not used for aggregate) */
        agg_step, /* xStep */
        agg_final, /* xFinal */
        NULL  /* xDestroy */
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 3. Explore program states: Execute a query with fuzzed input */
    /* We construct a simple SQL statement using the input data as a string literal.
     * We escape single quotes to prevent SQL syntax errors.
     */
    char *sql = NULL;
    size_t sql_len = 0;
    size_t i;

    /* Calculate required buffer size */
    for (i = 0; i < Size; i++) {
        if (Data[i] == '\'') sql_len += 2;
        else sql_len += 1;
    }
    sql_len += 32; /* Buffer for "SELECT fuzz_agg('" and "')" */

    sql = malloc(sql_len + 1);
    if (sql == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Build the SQL string */
    int offset = sprintf(sql, "SELECT fuzz_agg('");
    if (offset < 0) {
        free(sql);
        sqlite3_close(db);
        return 0;
    }

    for (i = 0; i < Size; i++) {
        if (Data[i] == '\'') {
            offset += sprintf(sql + offset, "''");
        } else {
            offset += sprintf(sql + offset, "%c", Data[i]);
        }
    }
    offset += sprintf(sql + offset, "')");
    sql[offset] = '\0';

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    free(sql);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step through the result set */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* We don't necessarily need to read the result, just executing triggers the callbacks */
        const unsigned char *res = sqlite3_column_text(stmt, 0);
        (void)res;
    }

    /* 4. Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}