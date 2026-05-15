// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Dummy function for sqlite3_result_value
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy function to get a protected sqlite3_value
    sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    
    // Prepare a statement to get a protected sqlite3_value
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func(?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind different types of values based on input data
    uint8_t value_type = Data[0] % 5;
    size_t data_offset = 1;
    
    switch (value_type) {
        case 0: // NULL
            sqlite3_bind_null(stmt, 1);
            break;
        case 1: // INTEGER
            if (data_offset + sizeof(int64_t) <= Size) {
                int64_t int_val;
                memcpy(&int_val, Data + data_offset, sizeof(int64_t));
                sqlite3_bind_int64(stmt, 1, int_val);
                data_offset += sizeof(int64_t);
            } else {
                sqlite3_bind_int(stmt, 1, 42);
            }
            break;
        case 2: // FLOAT/REAL
            if (data_offset + sizeof(double) <= Size) {
                double dbl_val;
                memcpy(&dbl_val, Data + data_offset, sizeof(double));
                sqlite3_bind_double(stmt, 1, dbl_val);
                data_offset += sizeof(double);
            } else {
                sqlite3_bind_double(stmt, 1, 3.14159);
            }
            break;
        case 3: // TEXT
            if (data_offset < Size) {
                size_t text_len = Size - data_offset;
                if (text_len > 1000) text_len = 1000; // Limit size
                char *text = (char *)malloc(text_len + 1);
                if (text) {
                    memcpy(text, Data + data_offset, text_len);
                    text[text_len] = '\0';
                    sqlite3_bind_text(stmt, 1, text, -1, SQLITE_TRANSIENT);
                    free(text);
                } else {
                    sqlite3_bind_text(stmt, 1, "test", -1, SQLITE_TRANSIENT);
                }
            } else {
                sqlite3_bind_text(stmt, 1, "test", -1, SQLITE_TRANSIENT);
            }
            break;
        case 4: // BLOB
            if (data_offset < Size) {
                size_t blob_len = Size - data_offset;
                if (blob_len > 1000) blob_len = 1000; // Limit size
                sqlite3_bind_blob(stmt, 1, Data + data_offset, blob_len, SQLITE_TRANSIENT);
            } else {
                const char dummy_blob[] = {0x01, 0x02, 0x03};
                sqlite3_bind_blob(stmt, 1, dummy_blob, sizeof(dummy_blob), SQLITE_TRANSIENT);
            }
            break;
    }
    
    // Execute the statement to get a protected sqlite3_value
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_value *value = sqlite3_column_value(stmt, 0);
        
        // Now we have a protected value, duplicate it to use with the APIs
        sqlite3_value *protected_value = sqlite3_value_dup(value);
        
        if (protected_value) {
            // Test all target API functions
            int type = sqlite3_value_type(protected_value);
            int numeric_type = sqlite3_value_numeric_type(protected_value);
            int bytes16 = sqlite3_value_bytes16(protected_value);
            int int_val = sqlite3_value_int(protected_value);
            const void *blob_data = sqlite3_value_blob(protected_value);
            
            // Test sqlite3_result_value - we need to create a context
            // We'll create another dummy function that uses sqlite3_result_value
            // and call it with our protected value
            
            // Prepare another statement that uses our dummy function
            sqlite3_stmt *stmt2 = NULL;
            const char *sql2 = "SELECT dummy_func(?)";
            if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) == SQLITE_OK) {
                // Bind the protected value
                sqlite3_bind_value(stmt2, 1, protected_value);
                
                // Execute - this will call our dummy_func which uses sqlite3_result_value
                if (sqlite3_step(stmt2) == SQLITE_ROW) {
                    // Function executed successfully
                }
                sqlite3_finalize(stmt2);
            }
            
            sqlite3_value_free(protected_value);
        }
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}