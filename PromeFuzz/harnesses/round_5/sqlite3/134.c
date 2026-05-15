// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
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
    (void)argc;
    
    if (argv[0] != NULL) {
        // Test sqlite3_value_blob
        const void *blob_data = sqlite3_value_blob(argv[0]);
        (void)blob_data;
        
        // Test sqlite3_value_pointer
        void *ptr_data = sqlite3_value_pointer(argv[0], "TestType");
        (void)ptr_data;
        
        // Test sqlite3_value_text16le
        const void *text16le_data = sqlite3_value_text16le(argv[0]);
        (void)text16le_data;
        
        // Test sqlite3_value_text16be
        const void *text16be_data = sqlite3_value_text16be(argv[0]);
        (void)text16be_data;
        
        // Test sqlite3_value_text16
        const void *text16_data = sqlite3_value_text16(argv[0]);
        (void)text16_data;
        
        // Test sqlite3_result_value
        sqlite3_result_value(ctx, argv[0]);
    }
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    // Initialize database connection once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Create a custom function that will use the target APIs
        sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, 
                               fuzz_function, NULL, NULL);
        
        // Prepare a statement that calls our function
        if (sqlite3_prepare_v2(db, "SELECT fuzz_func(?)", -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (db == NULL || stmt == NULL) {
        return 0;
    }
    
    // Reset the statement for new execution
    sqlite3_reset(stmt);
    
    // Bind the fuzzer input as different types to test various code paths
    if (Size > 0) {
        // Try binding as BLOB
        sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
        sqlite3_step(stmt);
        
        // Reset and try as TEXT
        sqlite3_reset(stmt);
        char *text_data = malloc(Size + 1);
        if (text_data) {
            memcpy(text_data, Data, Size);
            text_data[Size] = '\0';
            sqlite3_bind_text(stmt, 1, text_data, -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            free(text_data);
        }
        
        // Reset and try as INTEGER
        sqlite3_reset(stmt);
        int int_val = 0;
        if (Size >= sizeof(int)) {
            memcpy(&int_val, Data, sizeof(int));
        }
        sqlite3_bind_int(stmt, 1, int_val);
        sqlite3_step(stmt);
        
        // Reset and try as DOUBLE
        sqlite3_reset(stmt);
        double double_val = 0.0;
        if (Size >= sizeof(double)) {
            memcpy(&double_val, Data, sizeof(double));
        }
        sqlite3_bind_double(stmt, 1, double_val);
        sqlite3_step(stmt);
        
        // Reset and try as NULL
        sqlite3_reset(stmt);
        sqlite3_bind_null(stmt, 1);
        sqlite3_step(stmt);
        
        // Reset and try as pointer (if supported)
        sqlite3_reset(stmt);
        void *ptr = (void *)Data;
        sqlite3_bind_pointer(stmt, 1, ptr, "TestType", NULL);
        sqlite3_step(stmt);
    }
    
    // Clean up any remaining resources from this iteration
    sqlite3_clear_bindings(stmt);
    
    return 0;
}