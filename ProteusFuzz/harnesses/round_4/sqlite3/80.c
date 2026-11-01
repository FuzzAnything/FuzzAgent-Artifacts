// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a temporary database file.
 */
static int create_temp_db(const char *zFile, sqlite3 **ppDb) {
    FILE *f = fopen(zFile, "wb");
    if (!f) return SQLITE_CANTOPEN;
    fprintf(f, "SQLite format 3\000");
    for (int i = 0; i < 100; i++) fputc(0, f);
    fclose(f);

    int rc = sqlite3_open(zFile, ppDb);
    if (rc != SQLITE_OK) {
        sqlite3_close(*ppDb);
        *ppDb = 0;
        return rc;
    }
    return SQLITE_OK;
}

/*
 * Destructor for auxdata and result memory.
 */
static void my_destructor(void *p) {
    if (p) free(p);
}

/*
 * Aggregate step function implementation.
 */
static void agg_step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (!ctx) return;

    /* Test sqlite3_aggregate_context with varying sizes */
    int nBytes = 0;
    void *pCtx = sqlite3_aggregate_context(ctx, nBytes);
    if (pCtx) {
        /* Write some data to the context */
        memset(pCtx, 0xAA, 1);
    }

    /* Test sqlite3_set_auxdata */
    static int dummy_data = 42;
    sqlite3_set_auxdata(ctx, 0, &dummy_data, NULL);

    /* Test sqlite3_result_int */
    sqlite3_result_int(ctx, 12345);
}

/*
 * Aggregate final function implementation.
 */
static void agg_final_func(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Call aggregate_context with 0 to avoid reallocation */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    (void)pCtx;

    /* Test sqlite3_result_text */
    const char *text = "aggregate_result";
    sqlite3_result_text(ctx, text, -1, SQLITE_STATIC);
}

/*
 * Scalar function implementation.
 */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (!ctx) return;

    /* Test sqlite3_result_blob */
    const unsigned char blob[] = {0x01, 0x02, 0x03, 0x04};
    sqlite3_result_blob(ctx, blob, 4, SQLITE_STATIC);

    /* Test sqlite3_result_text16le */
    const unsigned char utf16le[] = {0x41, 0x00, 0x42, 0x00}; /* "AB" in UTF-16LE */
    sqlite3_result_text16le(ctx, utf16le, 4, SQLITE_STATIC);
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *zFile = "./dummy_file";
    sqlite3 *db = 0;
    int rc = create_temp_db(zFile, &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a table to allow aggregate queries */
    const char *zCreate = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, val TEXT);";
    sqlite3_exec(db, zCreate, 0, 0, 0);

    /* Insert some data based on fuzz input to vary execution paths */
    char zInsert[256];
    int val = (Size > 0) ? Data[0] : 0;
    snprintf(zInsert, sizeof(zInsert), "INSERT INTO test_table (val) VALUES ('%d');", val);
    sqlite3_exec(db, zInsert, 0, 0, 0);

    /* Register custom aggregate function */
    /* Signature: int sqlite3_create_function(sqlite3*, const char*, int, int, void*, void(*)(...), void(*)(...), void(*)(...)) */
    rc = sqlite3_create_function(
        db, "fuzz_agg", 0, SQLITE_UTF8, 0,
        0, agg_step_func, agg_final_func
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom scalar function */
    rc = sqlite3_create_function(
        db, "fuzz_scalar", 0, SQLITE_UTF8, 0,
        scalar_func, 0, 0
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare and execute queries to trigger the custom functions */
    sqlite3_stmt *stmt = 0;

    /* Query 1: Aggregate function */
    const char *zSql1 = "SELECT fuzz_agg(val) FROM test_table;";
    rc = sqlite3_prepare_v2(db, zSql1, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Consume result to ensure full execution */
            (void)sqlite3_column_text(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    /* Query 2: Scalar function */
    const char *zSql2 = "SELECT fuzz_scalar();";
    rc = sqlite3_prepare_v2(db, zSql2, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (void)sqlite3_column_text(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    /* Query 3: Mixed usage with potential NULLs or empty sets */
    const char *zSql3 = "SELECT fuzz_agg(val) FROM test_table WHERE val = 'nonexistent';";
    rc = sqlite3_prepare_v2(db, zSql3, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (void)sqlite3_column_text(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_close(db);
    remove(zFile);

    return 0;
}