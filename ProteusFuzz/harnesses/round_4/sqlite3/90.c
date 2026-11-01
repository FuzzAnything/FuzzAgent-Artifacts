// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Custom SQL function implementation to fuzz sqlite3_get_auxdata,
 * sqlite3_result_text16be, sqlite3_result_blob, sqlite3_result_int,
 * sqlite3_aggregate_count, and sqlite3_result_error.
 */

static void aux_data_destructor(void *p) {
    /* Free memory if it was allocated dynamically */
    if (p) {
        free(p);
    }
}

static void my_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) {
        return;
    }

    /* Fuzz sqlite3_get_auxdata */
    void *aux = sqlite3_get_auxdata(ctx, 0);
    if (aux) {
        /* Use aux data if available */
        (void)aux;
    }

    /* Fuzz sqlite3_result_text16be */
    static const char utf16be_data[] = {0x00, 0x41, 0x00, 0x42, 0x00, 0x43};
    sqlite3_result_text16be(ctx, utf16be_data, sizeof(utf16be_data), SQLITE_STATIC);

    /* Fuzz sqlite3_result_blob */
    static const unsigned char blob_data[] = {0x01, 0x02, 0x03, 0x04};
    sqlite3_result_blob(ctx, blob_data, sizeof(blob_data), SQLITE_STATIC);

    /* Fuzz sqlite3_result_int */
    sqlite3_result_int(ctx, 42);

    /* Fuzz sqlite3_result_error */
    sqlite3_result_error(ctx, "Fuzzed error message", -1);
}

static void my_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) {
        return;
    }

    /* Fuzz sqlite3_set_auxdata and sqlite3_get_auxdata */
    void *aux = sqlite3_get_auxdata(ctx, 0);
    if (!aux) {
        aux = malloc(10);
        if (aux) {
            memset(aux, 0, 10);
            sqlite3_set_auxdata(ctx, 0, aux, aux_data_destructor);
        }
    }
}

static void my_aggregate_final(sqlite3_context *ctx) {
    if (!ctx) {
        return;
    }

    /* Fuzz sqlite3_aggregate_count */
    int count = sqlite3_aggregate_count(ctx);
    (void)count;

    /* Fuzz sqlite3_result_int with count */
    sqlite3_result_int(ctx, count);
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    /* Initialize SQLite in memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to test with */
    const char *create_table_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom functions */
    rc = sqlite3_create_function(db, "my_function", -1, SQLITE_UTF8, NULL, my_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "my_aggregate", -1, SQLITE_UTF8, NULL, NULL, my_aggregate_step, my_aggregate_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement to test the custom functions */
    const char *sql = "SELECT my_function(), my_aggregate(data) FROM test;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    (void)rc;

    /* Finalize the statement */
    sqlite3_finalize(stmt);

    /* Close the database */
    sqlite3_close(db);

    return 0;
}