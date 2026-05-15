// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

typedef struct {
    int count;
    char data[256];
} AggContext;

static void xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // First call: sqlite3_aggregate_context
    AggContext *p = (AggContext*)sqlite3_aggregate_context(ctx, sizeof(AggContext));
    if (p) {
        p->count++;
        
        // Second call: sqlite3_value_bytes
        int bytes = sqlite3_value_bytes(argv[0]);
        
        // Third call: sqlite3_value_text
        const unsigned char *text = sqlite3_value_text(argv[0]);
        
        // Store some data if available
        if (text && bytes > 0 && bytes < 256) {
            memcpy(p->data, text, bytes);
            p->data[bytes] = '\0';
        }
    }
}

static void xFinal(sqlite3_context *ctx) {
    AggContext *p = (AggContext*)sqlite3_aggregate_context(ctx, 0);
    if (p && p->count > 0) {
        sqlite3_result_text(ctx, p->data, -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    // Initialize sqlite3
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create an aggregate function to get protected sqlite3_value objects
    sqlite3_create_function(db, "fuzz_agg", 1, SQLITE_UTF8, NULL, NULL, xStep, xFinal);
    
    // Prepare a statement that uses the aggregate function
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_agg(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind the fuzzer data
    if (Size > 0) {
        // Use the first part of data as text
        size_t bind_size = Size > 1024 ? 1024 : Size;
        char *text_data = malloc(bind_size + 1);
        if (text_data) {
            for (size_t i = 0; i < bind_size; i++) {
                text_data[i] = (char)(Data[i] % 128); // Ensure valid UTF-8 ASCII range
            }
            text_data[bind_size] = '\0';
            sqlite3_bind_text(stmt, 1, text_data, (int)bind_size, SQLITE_TRANSIENT);
            free(text_data);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute the statement - this will call xStep and xFinal
    // which in turn call the three target functions on protected values
    sqlite3_step(stmt);
    
    // Reset and execute again with different data to test more states
    sqlite3_reset(stmt);
    
    // Bind as blob this time
    if (Size > 0) {
        size_t bind_size = Size > 1024 ? 1024 : Size;
        sqlite3_bind_blob(stmt, 1, Data, (int)bind_size, SQLITE_STATIC);
        sqlite3_step(stmt);
    }
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}