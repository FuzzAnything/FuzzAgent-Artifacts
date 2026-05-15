// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold user data for function callbacks
typedef struct {
    const uint8_t* fuzz_data;
    size_t fuzz_size;
    size_t data_offset;
} fuzz_user_data;

// Custom scalar function implementation
static void fuzz_scalar_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    fuzz_user_data* user_data = (fuzz_user_data*)sqlite3_user_data(context);
    if (!user_data || user_data->fuzz_size == 0) {
        sqlite3_result_null(context);
        return;
    }

    // Use fuzz data to decide which operations to perform
    size_t offset = user_data->data_offset % user_data->fuzz_size;
    uint8_t op_selector = user_data->fuzz_data[offset];
    
    // Test sqlite3_aggregate_context (even though this is a scalar function)
    int nBytes = (op_selector % 64) + 1; // 1-64 bytes
    void* agg_ctx = sqlite3_aggregate_context(context, nBytes);
    
    // Test different result types based on fuzz data
    switch (op_selector % 6) {
        case 0:
            sqlite3_result_null(context);
            break;
        case 1: {
            // Create error message from fuzz data
            size_t err_len = (user_data->fuzz_size - offset) % 256;
            if (err_len > 0) {
                char* err_msg = (char*)malloc(err_len + 1);
                if (err_msg) {
                    size_t copy_len = err_len;
                    if (offset + copy_len > user_data->fuzz_size) {
                        copy_len = user_data->fuzz_size - offset;
                    }
                    memcpy(err_msg, user_data->fuzz_data + offset, copy_len);
                    err_msg[copy_len] = '\0';
                    sqlite3_result_error(context, err_msg, (int)copy_len);
                    free(err_msg);
                } else {
                    sqlite3_result_error(context, "Memory allocation failed", -1);
                }
            } else {
                sqlite3_result_error(context, "Fuzz error", -1);
            }
            break;
        }
        case 2: {
            // Create text result from fuzz data
            size_t text_len = (user_data->fuzz_size - offset) % 1024;
            if (text_len > 0) {
                char* text = (char*)malloc(text_len + 1);
                if (text) {
                    size_t copy_len = text_len;
                    if (offset + copy_len > user_data->fuzz_size) {
                        copy_len = user_data->fuzz_size - offset;
                    }
                    memcpy(text, user_data->fuzz_data + offset, copy_len);
                    text[copy_len] = '\0';
                    sqlite3_result_text(context, text, (int)copy_len, SQLITE_TRANSIENT);
                    free(text);
                } else {
                    sqlite3_result_null(context);
                }
            } else {
                sqlite3_result_text(context, "", 0, SQLITE_STATIC);
            }
            break;
        }
        case 3: {
            // Create double result from fuzz data
            double dbl_val = 0.0;
            if (offset + sizeof(double) <= user_data->fuzz_size) {
                memcpy(&dbl_val, user_data->fuzz_data + offset, sizeof(double));
            } else {
                // Use simple conversion if not enough data
                dbl_val = (double)(op_selector);
            }
            sqlite3_result_double(context, dbl_val);
            break;
        }
        case 4:
            // Test with SQLITE_TRANSIENT destructor
            sqlite3_result_text(context, "transient test", 14, SQLITE_TRANSIENT);
            break;
        case 5:
            // Test with SQLITE_STATIC destructor
            sqlite3_result_text(context, "static test", 11, SQLITE_STATIC);
            break;
    }
    
    // Update offset for next call
    user_data->data_offset = (offset + 1) % user_data->fuzz_size;
}

// Custom aggregate function step implementation
static void fuzz_aggregate_step(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Get aggregate context
    int nBytes = 128; // Fixed size for simplicity
    void* agg_ctx = sqlite3_aggregate_context(context, nBytes);
    
    if (agg_ctx) {
        // Use the user data
        fuzz_user_data* user_data = (fuzz_user_data*)sqlite3_user_data(context);
        if (user_data && user_data->fuzz_size > 0) {
            // Store some fuzz data in aggregate context
            size_t offset = user_data->data_offset % user_data->fuzz_size;
            size_t copy_len = nBytes;
            if (offset + copy_len > user_data->fuzz_size) {
                copy_len = user_data->fuzz_size - offset;
            }
            memcpy(agg_ctx, user_data->fuzz_data + offset, copy_len);
        }
    }
}

// Custom aggregate function final implementation
static void fuzz_aggregate_final(sqlite3_context* context) {
    // Get aggregate context with size 0 to retrieve existing buffer
    void* agg_ctx = sqlite3_aggregate_context(context, 0);
    
    if (agg_ctx) {
        // Return the first 8 bytes as a double
        double result = 0.0;
        memcpy(&result, agg_ctx, sizeof(double) > 128 ? 128 : sizeof(double));
        sqlite3_result_double(context, result);
    } else {
        sqlite3_result_null(context);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    sqlite3* db = NULL;
    int rc;
    char* err_msg = NULL;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    // Prepare user data structure
    fuzz_user_data user_data = {Data, Size, 0};
    
    // Register scalar function
    rc = sqlite3_create_function(
        db,
        "fuzz_func",
        0,  // Number of arguments
        SQLITE_UTF8,
        &user_data,
        fuzz_scalar_func,
        NULL,
        NULL
    );
    
    if (rc == SQLITE_OK) {
        // Register aggregate function
        rc = sqlite3_create_function(
            db,
            "fuzz_agg",
            0,  // Number of arguments
            SQLITE_UTF8,
            &user_data,
            NULL,
            fuzz_aggregate_step,
            fuzz_aggregate_final
        );
    }
    
    if (rc == SQLITE_OK) {
        // Execute queries to trigger the functions
        sqlite3_exec(db, "SELECT fuzz_func()", NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
        
        // Test aggregate function
        sqlite3_exec(db, "SELECT fuzz_agg() FROM (SELECT 1 UNION SELECT 2)", NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
        
        // Test with multiple calls
        for (int i = 0; i < 3 && i < Size; i++) {
            user_data.data_offset = i;
            sqlite3_exec(db, "SELECT fuzz_func()", NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = NULL;
            }
        }
    }
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}