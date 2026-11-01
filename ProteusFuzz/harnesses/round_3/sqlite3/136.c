// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to safely close a database connection if it exists.
 */
static void safe_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Custom destructor for auxiliary data to ensure cleanup.
 */
static void aux_data_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Custom destructor for blob/text data.
 */
static void data_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Implementation of a custom scalar function to test:
 * sqlite3_result_int, sqlite3_result_error, sqlite3_result_blob,
 * sqlite3_result_text16be, sqlite3_get_auxdata, sqlite3_set_auxdata.
 */
static void custom_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;

    /* Fuzzing: Try to get auxdata from various indices */
    int idx = 0;
    if (argc > 0 && argv[0]) {
        int val = sqlite3_value_int(argv[0]);
        idx = val % 10; /* Keep index small */
    }
    
    void *aux = sqlite3_get_auxdata(ctx, idx);
    if (aux) {
        /* If aux data exists, return it as an int result */
        sqlite3_result_int(ctx, 1);
        return;
    }

    /* Fuzzing: Set aux data based on input */
    if (argc > 0 && argv[0]) {
        int len = sqlite3_value_bytes(argv[0]);
        if (len > 0 && len < 1024) {
            void *data = malloc(len + 1);
            if (data) {
                memcpy(data, sqlite3_value_text(argv[0]), len);
                ((char*)data)[len] = '\0';
                sqlite3_set_auxdata(ctx, idx, data, aux_data_destructor);
            }
        }
    }

    /* Fuzzing: Try to return different result types based on input */
    if (argc > 1 && argv[1]) {
        int type = sqlite3_value_int(argv[1]);
        
        switch (type % 5) {
            case 0:
                /* Return an integer */
                sqlite3_result_int(ctx, sqlite3_value_int(argv[0]));
                break;
            case 1:
                /* Return an error */
                sqlite3_result_error(ctx, "Fuzzed error message", -1);
                break;
            case 2:
                /* Return a blob */
                if (argc > 0 && argv[0]) {
                    const void *blob_data = sqlite3_value_blob(argv[0]);
                    int blob_len = sqlite3_value_bytes(argv[0]);
                    if (blob_len > 0 && blob_len < 1024) {
                        void *copy = malloc(blob_len);
                        if (copy) {
                            memcpy(copy, blob_data, blob_len);
                            sqlite3_result_blob(ctx, copy, blob_len, data_destructor);
                        }
                    }
                }
                break;
            case 3:
                /* Return UTF-16BE text */
                if (argc > 0 && argv[0]) {
                    const char *text = (const char *)sqlite3_value_text(argv[0]);
                    int len = sqlite3_value_bytes(argv[0]);
                    if (len > 0 && len < 1024) {
                        /* Ensure even length for UTF-16 */
                        if (len % 2 != 0) len--;
                        if (len > 0) {
                            void *utf16_copy = malloc(len);
                            if (utf16_copy) {
                                memcpy(utf16_copy, text, len);
                                sqlite3_result_text16be(ctx, utf16_copy, len, data_destructor);
                            }
                        }
                    }
                }
                break;
            case 4:
                /* Return NULL */
                sqlite3_result_null(ctx);
                break;
        }
    } else {
        /* Default: return 0 */
        sqlite3_result_int(ctx, 0);
    }
}

/*
 * Implementation of a custom aggregate function step callback.
 */
static void custom_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;
    /* Just accumulate something simple */
    void *p = sqlite3_aggregate_context(ctx, 0);
    if (p) {
        int *count = (int *)p;
        (*count)++;
    }
}

/*
 * Implementation of a custom aggregate function finalize callback.
 * This is where sqlite3_aggregate_count is tested.
 */
static void custom_agg_finalize(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Test sqlite3_aggregate_count */
    int count = sqlite3_aggregate_count(ctx);
    
    /* Return the count as an integer */
    sqlite3_result_int(ctx, count);
}

/*
 * LLVM Fuzzer Test One Input
 */
int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a table to work with */
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        safe_close_db(db);
        return 0;
    }

    /* Register custom scalar function */
    rc = sqlite3_create_function(db, "fuzz_scalar", -1, SQLITE_UTF8, NULL, custom_scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        safe_close_db(db);
        return 0;
    }

    /* Register custom aggregate function */
    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, NULL, custom_agg_step, custom_agg_finalize);
    if (rc != SQLITE_OK) {
        safe_close_db(db);
        return 0;
    }

    /* Prepare a statement to insert data */
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        safe_close_db(db);
        return 0;
    }

    /* Insert fuzzed data into the table */
    if (Size > 0) {
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            safe_close_db(db);
            return 0;
        }
        sqlite3_reset(stmt);
    }

    /* Test various queries that invoke the custom functions */
    
    /* Query 1: Scalar function with integer argument */
    const char *q1 = "SELECT fuzz_scalar(1, 0)";
    rc = sqlite3_prepare_v2(db, q1, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is already handled in callback */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Query 2: Scalar function with error trigger */
    const char *q2 = "SELECT fuzz_scalar('error', 1)";
    rc = sqlite3_prepare_v2(db, q2, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is already handled in callback */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Query 3: Scalar function with blob result */
    const char *q3 = "SELECT fuzz_scalar('blob_data', 2)";
    rc = sqlite3_prepare_v2(db, q3, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is already handled in callback */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Query 4: Scalar function with UTF-16BE result */
    const char *q4 = "SELECT fuzz_scalar('utf16_data', 3)";
    rc = sqlite3_prepare_v2(db, q4, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is already handled in callback */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Query 5: Aggregate function */
    const char *q5 = "SELECT fuzz_agg(data) FROM test_table";
    rc = sqlite3_prepare_v2(db, q5, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is already handled in callback */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Query 6: Aggregate function on empty table (edge case) */
    const char *q6 = "SELECT fuzz_agg(data) FROM test_table WHERE 1=0";
    rc = sqlite3_prepare_v2(db, q6, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is already handled in callback */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Cleanup: Only finalize if stmt is valid */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    safe_close_db(db);

    return 0;
}