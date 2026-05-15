// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_error at sqlite3.c:78742:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite and provides a valid context
    // We'll use the first argument as our fuzz data
    
    if (argc < 1) {
        sqlite3_result_null(context);
        return;
    }
    
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (size == 0) {
        sqlite3_result_null(context);
        return;
    }
    
    // Use the first byte as selector
    uint8_t selector = data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0:
            sqlite3_result_null(context);
            break;
            
        case 1:
            sqlite3_result_error_toobig(context);
            break;
            
        case 2: {
            int n = 0;
            if (offset < (size_t)size) {
                n = (int)data[offset];
                offset++;
            }
            sqlite3_result_zeroblob(context, n);
            break;
        }
            
        case 3: {
            const char *error_msg = "Fuzzer generated error";
            int error_code = -1;
            
            if (offset + 1 < (size_t)size) {
                error_code = (int)((data[offset] << 8) | data[offset + 1]);
                offset += 2;
            }
            
            if (offset < (size_t)size && size - offset > 0) {
                size_t msg_len = size - offset;
                if (msg_len > 100) msg_len = 100;
                char *dynamic_msg = (char *)malloc(msg_len + 1);
                if (dynamic_msg) {
                    memcpy(dynamic_msg, data + offset, msg_len);
                    dynamic_msg[msg_len] = '\0';
                    sqlite3_result_error(context, dynamic_msg, error_code);
                    free(dynamic_msg);
                } else {
                    sqlite3_result_error(context, error_msg, error_code);
                }
            } else {
                sqlite3_result_error(context, error_msg, error_code);
            }
            break;
        }
            
        case 4: {
            int value = 0;
            if (offset < (size_t)size) {
                value = (int)data[offset];
                offset++;
            }
            sqlite3_result_int(context, value);
            break;
        }
            
        case 5:
            sqlite3_result_error_nomem(context);
            break;
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    if (!initialized) {
        if (sqlite3_initialize() != SQLITE_OK) {
            return 0;
        }
        
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            sqlite3_shutdown();
            return 0;
        }
        
        // Create a function that we can call with our fuzz data
        if (sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, 
                                    NULL, fuzz_func, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            sqlite3_shutdown();
            return 0;
        }
        
        initialized = 1;
    }
    
    if (db == NULL || Size == 0) {
        return 0;
    }
    
    // Create a prepared statement that calls our function with the fuzz data
    sqlite3_stmt *stmt;
    const char *sql = "SELECT fuzz_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzz data as a blob parameter
        sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
        
        // Execute the statement - this will call fuzz_func with a valid context
        sqlite3_step(stmt);
        
        sqlite3_finalize(stmt);
    }
    
    return 0;
}