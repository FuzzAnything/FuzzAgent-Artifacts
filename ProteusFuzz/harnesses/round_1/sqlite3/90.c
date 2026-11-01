// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:92481:25 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
#include <stdio.h>

/*
 * Fuzz driver for sqlite3_value_* functions.
 * 
 * Strategy:
 * 1. Open an in-memory database.
 * 2. Prepare a simple SQL statement that returns various types of values.
 * 3. Extract values using sqlite3_column_value().
 * 4. Apply the target API functions on these values.
 * 5. Handle memory management (dup/free) carefully.
 * 6. Use fuzzer input to control SQL statements and value selections.
 */

static int sqlite3_create_function_subtype(
    sqlite3 *db,
    const char *zName,
    int nArg,
    int eTextRep,
    void *pApp,
    void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
    void (*xStep)(sqlite3_context*,int,sqlite3_value**),
    void (*xFinal)(sqlite3_context*)
) {
    /* Wrapper to register function with SQLITE_SUBTYPE flag */
    return sqlite3_create_function_v2(
        db, zName, nArg, eTextRep | SQLITE_SUBTYPE, pApp, xFunc, xStep, xFinal, 0
    );
}

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Dummy function to test sqlite3_value_subtype */
    if (argc > 0) {
        unsigned int subtype = sqlite3_value_subtype(argv[0]);
        (void)subtype; /* Use the value to avoid warning */
    }
    sqlite3_result_null(ctx);
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Dummy step function */
    (void)ctx; (void)argc; (void)argv;
}

static void dummy_final(sqlite3_context *ctx) {
    /* Dummy final function */
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int i;
    int nCols;
    sqlite3_value *val = NULL;
    sqlite3_value *dup_val = NULL;
    const void *text16 = NULL;
    const void *text16le = NULL;
    const void *text16be = NULL;
    unsigned int subtype = 0;
    
    /* Guard against empty input */
    if (Size == 0) return 0;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register a function with SQLITE_SUBTYPE to test sqlite3_value_subtype */
    sqlite3_create_function_subtype(db, "test_subtype", 1, SQLITE_UTF8, NULL, dummy_func, dummy_step, dummy_final);

    /* Use fuzzer input to generate SQL */
    /* We limit the SQL length to avoid buffer issues */
    size_t sql_len = Size;
    if (sql_len > 1000) sql_len = 1000;
    
    /* Ensure null termination */
    char *sql = (char *)malloc(sql_len + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, sql_len);
    sql[sql_len] = '\0';

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    free(sql);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute the statement and process results */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        nCols = sqlite3_column_count(stmt);
        for (i = 0; i < nCols; i++) {
            /* Get the value from the column */
            val = sqlite3_column_value(stmt, i);
            if (!val) continue;

            /* Test sqlite3_value_dup */
            dup_val = sqlite3_value_dup(val);
            if (dup_val) {
                /* Test text retrieval functions on the duplicated value */
                text16 = sqlite3_value_text16(dup_val);
                (void)text16;
                
                text16le = sqlite3_value_text16le(dup_val);
                (void)text16le;
                
                text16be = sqlite3_value_text16be(dup_val);
                (void)text16be;

                /* Test subtype on duplicated value */
                subtype = sqlite3_value_subtype(dup_val);
                (void)subtype;

                /* Free the duplicated value */
                sqlite3_value_free(dup_val);
            }

            /* Test text retrieval functions on original value (if protected) */
            /* Note: sqlite3_column_value returns unprotected values in some contexts,
             * but these functions often work or return NULL safely. */
            text16 = sqlite3_value_text16(val);
            (void)text16;
            
            text16le = sqlite3_value_text16le(val);
            (void)text16le;
            
            text16be = sqlite3_value_text16be(val);
            (void)text16be;

            /* Test subtype on original value */
            subtype = sqlite3_value_subtype(val);
            (void)subtype;
        }
    }

    /* Finalize statement */
    sqlite3_finalize(stmt);

    /* Close database */
    sqlite3_close(db);

    return 0;
}