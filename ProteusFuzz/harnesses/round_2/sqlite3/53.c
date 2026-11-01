// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:93512:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
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
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:93424:18 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 result functions and aggregate helpers.
 * Target APIs:
 * 1. sqlite3_result_blob
 * 2. sqlite3_result_int
 * 3. sqlite3_result_text16be
 * 4. sqlite3_result_error
 * 5. sqlite3_aggregate_count
 * 6. sqlite3_get_auxdata
 *
 * Strategy:
 * - Create a temporary in-memory database.
 * - Create a custom scalar function and an aggregate function.
 * - Invoke these functions via SQL queries using the fuzz input data.
 * - The custom function implementations will call the target APIs.
 * - Handle cleanup and errors gracefully.
 * - Ensure all sqlite3 calls check for NULL/validity to prevent ASAN errors.
 */

static void aux_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/* Scalar function that tests sqlite3_result_blob, sqlite3_result_int, 
 * sqlite3_result_text16be, sqlite3_result_error, and sqlite3_get_auxdata.
 */
static void scalar_test_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;

    /* Use first argument to decide which test to run if possible */
    if (argc > 0) {
        int type = sqlite3_value_type(argv[0]);
        
        /* Test sqlite3_get_auxdata and sqlite3_set_auxdata */
        if (argc > 1) {
            void *aux = sqlite3_get_auxdata(ctx, 1);
            if (!aux) {
                /* Create some dummy aux data based on argument 1 */
                int len = sqlite3_value_int(argv[1]);
                if (len > 0 && len < 10000) {
                    void *ptr = malloc(len);
                    if (ptr) {
                        memset(ptr, 0, len);
                        sqlite3_set_auxdata(ctx, 1, ptr, aux_destructor);
                    }
                }
            }
        }

        /* Branch based on type to call different result functions */
        switch (type) {
            case SQLITE_BLOB:
                {
                    const void *blob = sqlite3_value_blob(argv[0]);
                    int len = sqlite3_value_bytes(argv[0]);
                    if (len < 0) len = 0;
                    if (len > 1000000) len = 1000000; /* Safety cap */
                    
                    if (argc > 2 && sqlite3_value_int(argv[2]) == 1) {
                        sqlite3_result_blob(ctx, blob, len, SQLITE_STATIC);
                    } else if (argc > 2 && sqlite3_value_int(argv[2]) == 2) {
                        sqlite3_result_blob(ctx, blob, len, NULL);
                    } else {
                        sqlite3_result_blob(ctx, blob, len, SQLITE_TRANSIENT);
                    }
                }
                break;
            case SQLITE_INTEGER:
                {
                    int val = sqlite3_value_int(argv[0]);
                    sqlite3_result_int(ctx, val);
                }
                break;
            case SQLITE_TEXT:
                {
                    /* Try to interpret text as UTF-16BE for text16be test */
                    const unsigned char *text = (const unsigned char *)sqlite3_value_text(argv[0]);
                    int len = sqlite3_value_bytes(argv[0]);
                    /* Ensure even length for UTF-16BE */
                    if (len % 2 != 0) len--; 
                    if (len < 0) len = 0;
                    if (len > 1000000) len = 1000000; /* Safety cap */
                    
                    if (len > 0) {
                        sqlite3_result_text16be(ctx, text, len, SQLITE_TRANSIENT);
                    } else {
                        sqlite3_result_text16be(ctx, "", 0, SQLITE_STATIC);
                    }
                }
                break;
            case SQLITE_NULL:
                /* Trigger error path */
                sqlite3_result_error(ctx, "Fuzz induced error", -1);
                break;
            default:
                /* Fallback to int */
                sqlite3_result_int(ctx, 0);
                break;
        }
    } else {
        /* No arguments, test error handling */
        sqlite3_result_error(ctx, "No arguments provided", -1);
    }
}

/* Aggregate step function */
static void agg_step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;
    /* Just accumulate, maybe store aux data */
    if (argc > 0) {
        void *aux = sqlite3_get_auxdata(ctx, 0);
        if (!aux) {
            aux = malloc(1);
            if (aux) {
                sqlite3_set_auxdata(ctx, 0, aux, aux_destructor);
            }
        }
    }
}

/* Aggregate final function */
static void agg_final_func(sqlite3_context *ctx) {
    if (!ctx) return;
    
    /* Test sqlite3_aggregate_count */
    int count = sqlite3_aggregate_count(ctx);
    
    /* Return the count as result */
    sqlite3_result_int(ctx, count);
    
    /* Also test result_blob with a small static blob */
    const char *blob_data = "agg";
    sqlite3_result_blob(ctx, blob_data, 3, SQLITE_STATIC);
}

/*
 * Main Fuzz Entry Point
 */
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Register custom functions */
    rc = sqlite3_create_function(db, "fuzz_scalar", -1, SQLITE_UTF8, NULL, scalar_test_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, NULL, agg_step_func, agg_final_func);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy table to allow aggregate testing */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert fuzz data into the table if we have enough data */
    if (Size > 0) {
        /* Limit the amount of data we try to insert to avoid OOM or huge SQL strings */
        size_t max_hex_size = 10000; 
        if (Size > max_hex_size) Size = max_hex_size;
        
        char *hex_str = malloc(Size * 2 + 1);
        if (hex_str) {
            for (size_t i = 0; i < Size; i++) {
                sprintf(hex_str + i*2, "%02x", Data[i]);
            }
            hex_str[Size*2] = '\0';
            
            char *sql = malloc(256 + Size * 2 + 10);
            if (sql) {
                snprintf(sql, 256 + Size * 2 + 10, "INSERT INTO test_table (id, data) VALUES (1, X'%s');", hex_str);
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                free(sql);
            }
            free(hex_str);
        }
    }

    /* 2. Construct SQL queries to exercise the target APIs */
    
    /* Pattern 1: Call scalar function with blob data */
    const char *q1 = "SELECT fuzz_scalar(data, 1, 1) FROM test_table;";
    rc = sqlite3_prepare_v2(db, q1, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            if (val) {
                sqlite3_value_type(val);
            }
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Pattern 2: Call scalar function with integer */
    const char *q2 = "SELECT fuzz_scalar(12345, 10, 2);";
    rc = sqlite3_prepare_v2(db, q2, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Pattern 3: Call scalar function with text (to test text16be) */
    const char *q3 = "SELECT fuzz_scalar('Hello World', 0, 0);";
    rc = sqlite3_prepare_v2(db, q3, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Pattern 4: Call scalar function with NULL (to test error) */
    const char *q4 = "SELECT fuzz_scalar(NULL);";
    rc = sqlite3_prepare_v2(db, q4, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Pattern 5: Call aggregate function */
    const char *q5 = "SELECT fuzz_agg(id) FROM test_table;";
    rc = sqlite3_prepare_v2(db, q5, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* 3. Cleanup */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}