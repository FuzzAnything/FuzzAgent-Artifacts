// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
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
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_aggregate_count
 * - sqlite3_get_auxdata
 * - sqlite3_result_text16be
 * - sqlite3_result_error
 * - sqlite3_result_int
 * - sqlite3_result_blob
 *
 * Strategy:
 * 1. Create an in-memory database.
 * 2. Register custom scalar and aggregate functions that invoke the target APIs.
 * 3. Execute SQL statements that trigger these functions with varying inputs derived from the fuzz buffer.
 * 4. Ensure proper cleanup of database, statements, and resources.
 */

static void custom_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Test sqlite3_get_auxdata */
    void *aux = sqlite3_get_auxdata(ctx, 0);
    if (!aux) {
        /* Store some dummy auxiliary data */
        int *data = (int *)sqlite3_malloc(4);
        if (data) {
            *data = 0x12345678;
            sqlite3_set_auxdata(ctx, 0, data, sqlite3_free);
        }
    }

    /* Test sqlite3_result_int */
    sqlite3_result_int(ctx, 42);
}

static void custom_scalar_func_error(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Test sqlite3_result_error */
    sqlite3_result_error(ctx, "Custom fuzz error", -1);
}

static void custom_scalar_func_blob(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Test sqlite3_result_blob */
    const char *blob_data = "BLOB_FUZZ_DATA";
    int blob_len = (int)strlen(blob_data);
    sqlite3_result_blob(ctx, blob_data, blob_len, SQLITE_STATIC);
}

static void custom_scalar_func_text16be(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Test sqlite3_result_text16be with a simple UTF-16BE string (2 bytes per char) */
    /* "FUZZ" in UTF-16BE: 0x0046 0x0055 0x005A 0x005A */
    const unsigned char utf16be[] = {0x00, 0x46, 0x00, 0x55, 0x00, 0x5A, 0x00, 0x5A};
    sqlite3_result_text16be(ctx, utf16be, 8, SQLITE_STATIC);
}

static void custom_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Accumulate nothing, just to allow finalize to run */
}

static void custom_aggregate_finalize(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Test sqlite3_aggregate_count */
    int count = sqlite3_aggregate_count(ctx);
    sqlite3_result_int(ctx, count);
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }

    /* Register custom functions */
    rc = sqlite3_create_function(db, "fuzz_scalar", 0, SQLITE_UTF8, NULL, custom_scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_error", 0, SQLITE_UTF8, NULL, custom_scalar_func_error, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_blob", 0, SQLITE_UTF8, NULL, custom_scalar_func_blob, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_text16be", 0, SQLITE_UTF8, NULL, custom_scalar_func_text16be, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL, NULL, custom_aggregate_step, custom_aggregate_finalize);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table for aggregate testing */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some rows based on fuzz input to vary aggregate count */
    const char *insert_sql = "INSERT INTO fuzz_table (id) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        for (size_t i = 0; i < Size && i < 1000; i++) {
            sqlite3_bind_int(stmt, 1, Data[i]);
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
                break;
            }
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Execute queries to trigger custom functions */
    const char *query_scalar = "SELECT fuzz_scalar();";
    const char *query_error = "SELECT fuzz_error();";
    const char *query_blob = "SELECT fuzz_blob();";
    const char *query_text16be = "SELECT fuzz_text16be();";
    const char *query_agg = "SELECT fuzz_agg(id) FROM fuzz_table;";

    const char *queries[] = {query_scalar, query_error, query_blob, query_text16be, query_agg};
    int num_queries = sizeof(queries) / sizeof(queries[0]);

    for (int i = 0; i < num_queries; i++) {
        rc = sqlite3_prepare_v2(db, queries[i], -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                /* Consume rows if any */
            }
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /* Additional edge case: call functions with NULL context simulation via error handling */
    /* Note: We cannot directly call APIs with NULL context safely in user code, 
       but the internal API armor handles it. We rely on the registered functions to be called correctly. */

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}