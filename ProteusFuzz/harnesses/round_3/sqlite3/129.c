// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_aggregate_count
 * - sqlite3_result_blob
 * - sqlite3_result_error
 * - sqlite3_get_auxdata
 * - sqlite3_result_text16be
 * - sqlite3_result_int
 *
 * This driver creates a custom aggregate and scalar function,
 * registers them, and executes queries to trigger the target APIs.
 */

static void *aggregate_step_data;
static int aggregate_step_count;

static void aggregate_step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Retrieve or create aggregate context */
    void *p = sqlite3_aggregate_context(ctx, sizeof(int));
    if (p) {
        int *count = (int *)p;
        (*count)++;
    }
}

static void aggregate_finalize_func(sqlite3_context *ctx) {
    int count = 0;
    
    /* Target API: sqlite3_aggregate_count */
    count = sqlite3_aggregate_count(ctx);
    
    /* Use the count to set result */
    sqlite3_result_int(ctx, count);
}

static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Fuzz logic based on input data */
    if (argc > 0) {
        const void *blob_data = sqlite3_value_blob(argv[0]);
        int blob_len = sqlite3_value_bytes(argv[0]);
        
        /* Target API: sqlite3_result_blob */
        if (blob_data && blob_len > 0) {
            sqlite3_result_blob(ctx, blob_data, blob_len, SQLITE_TRANSIENT);
            return;
        }
        
        /* Target API: sqlite3_result_error */
        if (blob_len < 0) {
            sqlite3_result_error(ctx, "Invalid blob length", -1);
            return;
        }
        
        /* Target API: sqlite3_get_auxdata */
        void *aux = sqlite3_get_auxdata(ctx, 0);
        if (aux) {
            /* If aux data exists, return it as text */
            sqlite3_result_text16be(ctx, "AUX_DATA_FOUND", 14, SQLITE_STATIC);
            return;
        }
        
        /* Set aux data for next call */
        static char aux_marker[] = "MARKER";
        sqlite3_set_auxdata(ctx, 0, aux_marker, NULL);
    }
    
    /* Target API: sqlite3_result_text16be */
    sqlite3_result_text16be(ctx, "DEFAULT", 14, SQLITE_STATIC);
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    char *error_msg = NULL;
    
    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Register custom aggregate function */
    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, 
                                  aggregate_step_func, aggregate_step_func, aggregate_finalize_func);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Register custom scalar function */
    rc = sqlite3_create_function(db, "fuzz_scalar", 1, SQLITE_UTF8, NULL, scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Create a table for testing */
    sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, sql, NULL, NULL, &error_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return 0;
    }
    
    /* Insert fuzz data into the table */
    if (Size > 0) {
        char *insert_sql = sqlite3_mprintf("INSERT INTO test_table (data) VALUES (X'%s');", 
                                           (const char *)Data);
        if (insert_sql) {
            /* Convert raw data to hex string for SQL */
            char hex[Size * 2 + 1];
            for (size_t i = 0; i < Size; i++) {
                sprintf(&hex[i * 2], "%02x", Data[i]);
            }
            hex[Size * 2] = '\0';
            
            char *final_sql = sqlite3_mprintf("INSERT INTO test_table (data) VALUES (X'%s');", hex);
            if (final_sql) {
                rc = sqlite3_exec(db, final_sql, NULL, NULL, &error_msg);
                sqlite3_free(final_sql);
            }
            sqlite3_free(insert_sql);
        }
    }
    
    /* Execute query using the aggregate function */
    sql = "SELECT fuzz_agg(data) FROM test_table;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Result is already handled in aggregate_finalize_func */
        }
        sqlite3_finalize(stmt);
    }
    
    /* Execute query using the scalar function */
    sql = "SELECT fuzz_scalar(data) FROM test_table;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Result is already handled in scalar_func */
        }
        sqlite3_finalize(stmt);
    }
    
    /* Execute a query that might trigger error handling */
    sql = "SELECT fuzz_scalar(NULL) FROM test_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Result is already handled in scalar_func */
        }
        sqlite3_finalize(stmt);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    return 0;
}