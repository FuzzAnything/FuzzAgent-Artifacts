// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

// Define minimal structures based on the provided definitions
// We need just enough to make the context usable
typedef struct Mem Mem;
typedef struct sqlite3_context sqlite3_context;

// Simple wrapper structure to hold what we need
typedef struct {
    sqlite3 *db;
    void *user_data;
} SimpleContext;

// Custom function that will be called by SQLite
static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite with a valid context
    // We'll use the user_data to pass fuzzing data
    SimpleContext *simple_ctx = (SimpleContext *)sqlite3_user_data(context);
    
    if (simple_ctx && simple_ctx->user_data) {
        uint8_t *data = (uint8_t *)simple_ctx->user_data;
        size_t size = ((size_t *)simple_ctx->user_data)[0];
        
        if (size > sizeof(size_t)) {
            uint8_t *actual_data = data + sizeof(size_t);
            size_t actual_size = size - sizeof(size_t);
            
            if (actual_size > 0) {
                uint8_t selector = actual_data[0] % 6;
                size_t data_offset = 1;
                
                switch (selector) {
                    case 0: {
                        // sqlite3_result_zeroblob
                        int n = 0;
                        if (actual_size >= data_offset + sizeof(int)) {
                            memcpy(&n, actual_data + data_offset, sizeof(int));
                        } else if (actual_size > data_offset) {
                            n = actual_data[data_offset];
                        }
                        sqlite3_result_zeroblob(context, n);
                        break;
                    }
                    case 1: {
                        // sqlite3_result_error
                        const char *err_msg = "Fuzzer error";
                        int len = -1;
                        if (actual_size > data_offset) {
                            // Use remaining data as error message (limit to 256 chars)
                            size_t msg_len = actual_size - data_offset;
                            if (msg_len > 256) msg_len = 256;
                            char *msg = (char *)malloc(msg_len + 1);
                            if (msg) {
                                memcpy(msg, actual_data + data_offset, msg_len);
                                msg[msg_len] = '\0';
                                sqlite3_result_error(context, msg, (int)msg_len);
                                free(msg);
                            } else {
                                sqlite3_result_error(context, err_msg, len);
                            }
                        } else {
                            sqlite3_result_error(context, err_msg, len);
                        }
                        break;
                    }
                    case 2:
                        // sqlite3_result_error_toobig
                        sqlite3_result_error_toobig(context);
                        break;
                    case 3:
                        // sqlite3_result_null
                        sqlite3_result_null(context);
                        break;
                    case 4:
                        // sqlite3_result_error_nomem
                        sqlite3_result_error_nomem(context);
                        break;
                    case 5: {
                        // sqlite3_result_int
                        int value = 0;
                        if (actual_size >= data_offset + sizeof(int)) {
                            memcpy(&value, actual_data + data_offset, sizeof(int));
                        } else if (actual_size > data_offset) {
                            value = actual_data[data_offset];
                        }
                        sqlite3_result_int(context, value);
                        break;
                    }
                }
            }
        }
    }
    
    // Free the user_data
    if (simple_ctx && simple_ctx->user_data) {
        free(simple_ctx->user_data);
        simple_ctx->user_data = NULL;
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    static sqlite3 *db = NULL;
    
    // Initialize database if needed
    if (!db) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
    }
    
    // Create a SimpleContext to pass data
    SimpleContext simple_ctx;
    simple_ctx.db = db;
    
    // Allocate memory for size + data
    size_t total_size = sizeof(size_t) + Size;
    uint8_t *user_data = (uint8_t *)malloc(total_size);
    if (!user_data) {
        return 0;
    }
    
    // Store size followed by data
    memcpy(user_data, &Size, sizeof(size_t));
    if (Size > 0) {
        memcpy(user_data + sizeof(size_t), Data, Size);
    }
    simple_ctx.user_data = user_data;
    
    // Create a temporary function with our context
    sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, &simple_ctx, 
                           test_function, NULL, NULL);
    
    // Prepare and execute a statement that calls our function
    sqlite3_stmt *stmt;
    const char *sql = "SELECT fuzz_func()";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Step once to trigger the function call
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Clean up the function
    sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, NULL, NULL, NULL, NULL);
    
    // Reset the database to clean up any state
    sqlite3_close(db);
    db = NULL;
    
    return 0;
}