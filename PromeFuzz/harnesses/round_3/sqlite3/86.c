// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Test sqlite3_user_data
    void *user_data = sqlite3_user_data(ctx);
    
    // Test sqlite3_aggregate_context with various sizes
    // Use fuzzer data to determine sizes
    if (user_data) {
        // Use the user_data pointer to get some bytes for size determination
        // We'll use a simple approach: if user_data points to at least 4 bytes, use them
        int nbytes = 64; // Default size
        void *agg_ctx = sqlite3_aggregate_context(ctx, nbytes);
        if (agg_ctx && nbytes > 0) {
            // Initialize with some data
            memset(agg_ctx, 0xAA, nbytes);
        }
        
        // Also test with zero bytes
        sqlite3_aggregate_context(ctx, 0);
        
        // Test with negative bytes (should return NULL)
        sqlite3_aggregate_context(ctx, -1);
    }
    
    // Test sqlite3_get_auxdata for various indices
    for (int i = 0; i < argc && i < 8; i++) {
        void *auxdata = sqlite3_get_auxdata(ctx, i);
        (void)auxdata; // Use auxdata to avoid unused variable warning
    }
    
    // Test sqlite3_result_null
    sqlite3_result_null(ctx);
    
    // Test sqlite3_result_subtype with various values
    unsigned int subtype = 0;
    if (user_data) {
        // Use first byte of user_data as subtype
        subtype = *((unsigned char*)user_data);
    }
    sqlite3_result_subtype(ctx, subtype);
    
    // Test sqlite3_result_double
    double d = 3.14159;
    if (user_data) {
        // Try to extract a double from the data
        // We'll use a simple approach: use first 8 bytes if available
        memcpy(&d, user_data, sizeof(double) > 8 ? 8 : sizeof(double));
    }
    sqlite3_result_double(ctx, d);
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // For aggregate function testing
    void *agg_ctx = sqlite3_aggregate_context(ctx, 64);
    if (agg_ctx) {
        // Store something in the aggregate context
        static int counter = 0;
        *(int *)agg_ctx = counter++;
    }
    
    // Also test with different sizes
    sqlite3_aggregate_context(ctx, 128);
    sqlite3_aggregate_context(ctx, 0);
    sqlite3_aggregate_context(ctx, -5); // Negative size test
}

static void dummy_xFinal(sqlite3_context *ctx) {
    void *agg_ctx = sqlite3_aggregate_context(ctx, 0);
    if (agg_ctx) {
        int value = *(int *)agg_ctx;
        sqlite3_result_double(ctx, (double)value);
    } else {
        sqlite3_result_null(ctx);
    }
}

static void dummy_destructor(void *data) {
    // Destructor for auxiliary data
    free(data);
}

static void test_auxdata_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Test sqlite3_get_auxdata and sqlite3_set_auxdata
    if (argc > 0) {
        void *auxdata = sqlite3_get_auxdata(ctx, 0);
        if (!auxdata) {
            // Create some auxiliary data
            char *data = malloc(16);
            if (data) {
                memset(data, 0x42, 16);
                // Note: sqlite3_set_auxdata is not in our target list, but we need it for testing get_auxdata
                // We'll use it if available, but skip if not
                // sqlite3_set_auxdata(ctx, 0, data, dummy_destructor);
            }
        }
    }
    
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    if (!db) {
        return 0;
    }
    
    // Create a dummy table for testing
    char *err_msg = NULL;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Insert some test data using fuzzer input
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "INSERT INTO test VALUES (?, ?);", -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, 1);
        // Limit size to avoid excessive memory usage
        size_t insert_size = Size > 1024 ? 1024 : Size;
        sqlite3_bind_blob(stmt, 2, Data, (int)insert_size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Register scalar function with user data pointing to our fuzzer input
    // We need to ensure Data pointer remains valid, so we copy it
    uint8_t *user_data_copy = NULL;
    if (Size > 0) {
        user_data_copy = malloc(Size > 256 ? 256 : Size);
        if (user_data_copy) {
            memcpy(user_data_copy, Data, Size > 256 ? 256 : Size);
        }
    }
    
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8 | SQLITE_RESULT_SUBTYPE, 
                           user_data_copy, test_function, NULL, NULL);
    
    // Register aggregate function
    sqlite3_create_function(db, "test_agg", 1, SQLITE_UTF8, NULL, NULL, dummy_xStep, dummy_xFinal);
    
    // Register function for testing auxdata
    sqlite3_create_function(db, "test_aux", 1, SQLITE_UTF8, NULL, test_auxdata_function, NULL, NULL);
    
    // Execute queries that will trigger our functions
    sqlite3_exec(db, "SELECT test_func(data) FROM test;", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    sqlite3_exec(db, "SELECT test_agg(data) FROM test;", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    sqlite3_exec(db, "SELECT test_aux(data) FROM test;", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }
    
    // Test with prepared statement
    sqlite3_stmt *stmt2;
    if (sqlite3_prepare_v2(db, "SELECT test_func(data) FROM test;", -1, &stmt2, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            // Just iterate through results
        }
        sqlite3_finalize(stmt2);
    }
    
    // Clean up
    sqlite3_exec(db, "DELETE FROM test;", NULL, NULL, NULL);
    
    // Free user_data_copy
    if (user_data_copy) {
        free(user_data_copy);
    }
    
    return 0;
}