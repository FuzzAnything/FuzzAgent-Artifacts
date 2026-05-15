// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:78913:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Global variables to pass fuzzer data to callback
static const uint8_t* g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

// Dummy user function that will be called by SQLite
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (g_fuzz_size == 0 || g_fuzz_data == NULL) {
        sqlite3_result_null(ctx);
        return;
    }
    
    // Use fuzzer input to decide which function to test
    uint8_t selector = g_fuzz_data[0];
    size_t offset = 1;
    
    switch (selector % 6) {
        case 0: {
            // Test sqlite3_result_zeroblob64
            if (offset + sizeof(sqlite3_uint64) <= g_fuzz_size) {
                sqlite3_uint64 n;
                memcpy(&n, g_fuzz_data + offset, sizeof(n));
                sqlite3_result_zeroblob64(ctx, n);
            } else {
                sqlite3_result_zeroblob64(ctx, 0);
            }
            break;
        }
        case 1: {
            // Test sqlite3_result_zeroblob
            if (offset + sizeof(int) <= g_fuzz_size) {
                int n;
                memcpy(&n, g_fuzz_data + offset, sizeof(n));
                sqlite3_result_zeroblob(ctx, n);
            } else {
                sqlite3_result_zeroblob(ctx, 0);
            }
            break;
        }
        case 2: {
            // Test sqlite3_result_error
            if (offset < g_fuzz_size) {
                int len = g_fuzz_data[offset] % 256;
                offset++;
                const char *msg = (const char *)(g_fuzz_data + offset);
                if (offset + len <= g_fuzz_size) {
                    sqlite3_result_error(ctx, msg, len);
                } else {
                    sqlite3_result_error(ctx, "fuzz error", -1);
                }
            } else {
                sqlite3_result_error(ctx, "test error", -1);
            }
            break;
        }
        case 3: {
            // Test sqlite3_result_value
            if (argc > 0) {
                sqlite3_result_value(ctx, argv[0]);
            } else {
                sqlite3_result_null(ctx);
            }
            break;
        }
        case 4: {
            // Test sqlite3_result_blob
            if (offset < g_fuzz_size) {
                int len = g_fuzz_data[offset] % 256;
                offset++;
                if (offset + len <= g_fuzz_size) {
                    sqlite3_result_blob(ctx, g_fuzz_data + offset, len, SQLITE_TRANSIENT);
                } else {
                    sqlite3_result_null(ctx);
                }
            } else {
                sqlite3_result_null(ctx);
            }
            break;
        }
        case 5: {
            // Test sqlite3_get_auxdata
            if (offset + sizeof(int) <= g_fuzz_size) {
                int n;
                memcpy(&n, g_fuzz_data + offset, sizeof(n));
                // Get auxdata (will likely be NULL since we haven't set any)
                void* aux = sqlite3_get_auxdata(ctx, n);
                // Return the auxdata pointer as a blob if not NULL
                if (aux != NULL) {
                    sqlite3_result_blob(ctx, &aux, sizeof(void*), SQLITE_TRANSIENT);
                } else {
                    sqlite3_result_null(ctx);
                }
            } else {
                sqlite3_result_null(ctx);
            }
            break;
        }
        default:
            sqlite3_result_null(ctx);
            break;
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Set global fuzzer data for callback
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *err_msg = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    // Create a dummy table to have some data
    rc = sqlite3_exec(db, "CREATE TABLE t1(x); INSERT INTO t1 VALUES(1), (2), (3)", 
                      NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Create our test function
    rc = sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, 
                                 dummy_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare and execute a statement that uses our function
    rc = sqlite3_prepare_v2(db, "SELECT dummy_func(x) FROM t1", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        // Execute the statement once
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Do nothing with the result
        }
        sqlite3_finalize(stmt);
    }
    
    // Also test with a simple scalar query
    rc = sqlite3_prepare_v2(db, "SELECT dummy_func(42)", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Do nothing with the result
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Reset global fuzzer data
    g_fuzz_data = NULL;
    g_fuzz_size = 0;
    
    return 0;
}