// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) return;
    
    sqlite3_value *value = argv[0];
    
    // Call functions in specified order
    const unsigned char *text = sqlite3_value_text(value);
    const void *blob = sqlite3_value_blob(value);
    int bytes = sqlite3_value_bytes(value);
    
    // Use the results to influence the fuzzing
    sqlite3_int64 result = 0;
    if (text) result += (sqlite3_int64)text[0];
    if (blob) result += ((const unsigned char *)blob)[0];
    result += bytes;
    
    sqlite3_result_int64(ctx, result);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register our test function
        sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, 
                               fuzz_function, NULL, NULL);
        initialized = 1;
    }
    
    if (!db || Size == 0) return 0;
    
    // Prepare a statement that calls our function with the fuzzer input
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    // Bind the fuzzer data as a blob
    if (sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return 0;
    }
    
    // Execute the statement (this will call our function)
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Function was called, results are handled internally
    }
    
    sqlite3_finalize(stmt);
    
    // Also test with text binding
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Create a null-terminated string from the data
        char *text_data = malloc(Size + 1);
        if (text_data) {
            memcpy(text_data, Data, Size);
            text_data[Size] = '\0';
            
            sqlite3_bind_text(stmt, 1, text_data, (int)Size, free);
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Function was called again
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    // Reset database to clean state for next iteration
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    
    return 0;
}