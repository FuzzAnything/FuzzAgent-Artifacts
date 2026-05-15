// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_step_called = 0;

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    dummy_step_called = 1;
    
    // Use fuzz data to determine nBytes for aggregate context
    int nBytes = 0;
    if (argc > 0) {
        const unsigned char *text = sqlite3_value_text(argv[0]);
        if (text) {
            // Use first byte of text as nBytes value
            nBytes = text[0];
        }
    }
    
    // Call sqlite3_aggregate_context
    void *agg_ctx = sqlite3_aggregate_context(ctx, nBytes);
    
    // Call sqlite3_value_bytes on each argument
    for (int i = 0; i < argc; i++) {
        int bytes = sqlite3_value_bytes(argv[i]);
        (void)bytes; // Use bytes to avoid unused variable warning
    }
    
    // Call sqlite3_value_text on each argument
    for (int i = 0; i < argc; i++) {
        const unsigned char *text = sqlite3_value_text(argv[i]);
        (void)text; // Use text to avoid unused variable warning
    }
}

static void dummy_final(sqlite3_context *ctx) {
    // Final call - get aggregate context with N=0
    void *agg_ctx = sqlite3_aggregate_context(ctx, 0);
    (void)agg_ctx;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Open an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a test table
    char *err_msg = NULL;
    const char *create_table = "CREATE TABLE test (id INTEGER, value TEXT);";
    if (sqlite3_exec(db, create_table, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some test data using the fuzz input
    sqlite3_stmt *stmt = NULL;
    const char *insert = "INSERT INTO test VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insert, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, 1);
        
        // Use fuzz data for the text value
        if (Size > 0) {
            // Ensure null termination
            char *text_data = (char*)malloc(Size + 1);
            if (text_data) {
                memcpy(text_data, Data, Size);
                text_data[Size] = '\0';
                sqlite3_bind_text(stmt, 2, text_data, -1, SQLITE_TRANSIENT);
                free(text_data);
            } else {
                sqlite3_bind_text(stmt, 2, "", -1, SQLITE_TRANSIENT);
            }
        } else {
            sqlite3_bind_text(stmt, 2, "", -1, SQLITE_TRANSIENT);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Register the aggregate function
    if (sqlite3_create_function(db, "dummy_agg", 1, SQLITE_UTF8, NULL, 
                                NULL, dummy_step, dummy_final) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Execute a query that uses the aggregate function
    const char *query = "SELECT dummy_agg(value) FROM test;";
    if (sqlite3_exec(db, query, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    
    // Also test with a prepared statement approach
    stmt = NULL;
    const char *query2 = "SELECT value FROM test;";
    if (sqlite3_prepare_v2(db, query2, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the value as a protected sqlite3_value
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            
            // Call sqlite3_value_bytes
            int bytes = sqlite3_value_bytes(val);
            (void)bytes;
            
            // Call sqlite3_value_text
            const unsigned char *text = sqlite3_value_text(val);
            (void)text;
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}