// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3 API functions.
 * This driver creates a custom SQL function to exercise:
 * - sqlite3_aggregate_count
 * - sqlite3_get_auxdata / sqlite3_set_auxdata
 * - sqlite3_result_text16be
 * - sqlite3_result_error
 * - sqlite3_result_int
 * - sqlite3_result_blob
 */

static void auxdata_destructor(void *p) {
    if (p) {
        free(p);
    }
}

static void custom_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (!ctx) return;

    /* Test sqlite3_get_auxdata */
    void *aux = sqlite3_get_auxdata(ctx, 0);
    if (aux) {
        /* If we have aux data, return it as an int (just for testing flow) */
        sqlite3_result_int(ctx, (int)(uintptr_t)aux);
        return;
    }

    /* Test sqlite3_set_auxdata with some dummy data */
    char *dummy = malloc(10);
    if (dummy) {
        memset(dummy, 'A', 10);
        sqlite3_set_auxdata(ctx, 0, dummy, auxdata_destructor);
    }

    /* Test sqlite3_result_error */
    sqlite3_result_error(ctx, "Fuzzing error", -1);
}

static void custom_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (!ctx) return;

    /* Do not set a result in step for aggregates, let the accumulator handle it */
    /* The aggregate context is managed by sqlite3_aggregate_context implicitly or explicitly */
}

static void custom_agg_finalize(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Test sqlite3_aggregate_count */
    int count = sqlite3_aggregate_count(ctx);
    
    /* Test sqlite3_result_int with the count */
    sqlite3_result_int(ctx, count);
}

static void custom_text_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (!ctx) return;

    /* Test sqlite3_result_text16be */
    /* Ensure the string is valid UTF-16BE and length is even */
    /* "Hello" in UTF-16BE: 0x00 0x48 0x00 0x65 0x00 0x6C 0x00 0x6C 0x00 0x6F */
    const char utf16be[] = {0x00, 0x48, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x6C, 0x00, 0x6F};
    int len = sizeof(utf16be);
    
    /* Verify length is even to avoid undefined behavior */
    if (len % 2 == 0) {
        sqlite3_result_text16be(ctx, utf16be, len, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_error(ctx, "Invalid UTF-16BE length", -1);
    }
}

static void custom_blob_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (!ctx) return;

    /* Test sqlite3_result_blob */
    const unsigned char blob_data[] = {0x01, 0x02, 0x03, 0x04};
    sqlite3_result_blob(ctx, blob_data, 4, SQLITE_TRANSIENT);
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to work with */
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some fuzz data */
    char *insert_sql = sqlite3_mprintf("INSERT INTO test_table (data) VALUES (?)");
    if (insert_sql) {
        sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
        if (stmt) {
            sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        sqlite3_free(insert_sql);
    }

    /* Register custom functions */
    sqlite3_create_function(db, "fuzz_scalar", 0, SQLITE_UTF8, NULL, custom_scalar_func, NULL, NULL);
    sqlite3_create_function(db, "fuzz_text", 0, SQLITE_UTF8, NULL, custom_text_func, NULL, NULL);
    sqlite3_create_function(db, "fuzz_blob", 0, SQLITE_UTF8, NULL, custom_blob_func, NULL, NULL);
    sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL, NULL, custom_agg_step, custom_agg_finalize);

    /* Execute queries using the custom functions */
    const char *queries[] = {
        "SELECT fuzz_scalar();",
        "SELECT fuzz_text();",
        "SELECT fuzz_blob();",
        "SELECT fuzz_agg() FROM test_table;",
        "SELECT fuzz_agg() FROM test_table LIMIT 1;",
        "SELECT fuzz_agg() FROM test_table WHERE 1=0;",
        "SELECT fuzz_scalar(), fuzz_text(), fuzz_blob() FROM test_table LIMIT 1;"
    };
    int num_queries = sizeof(queries) / sizeof(queries[0]);

    for (int i = 0; i < num_queries; i++) {
        rc = sqlite3_prepare_v2(db, queries[i], -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Consume results to ensure all paths are exercised */
                for (int j = 0; j < sqlite3_column_count(stmt); j++) {
                    switch (sqlite3_column_type(stmt, j)) {
                        case SQLITE_INTEGER:
                            sqlite3_column_int(stmt, j);
                            break;
                        case SQLITE_TEXT:
                            sqlite3_column_text(stmt, j);
                            break;
                        case SQLITE_BLOB:
                            sqlite3_column_blob(stmt, j);
                            break;
                        default:
                            break;
                    }
                }
            }
            sqlite3_finalize(stmt);
        }
    }

    /* Clean up */
    sqlite3_close(db);
    return 0;
}