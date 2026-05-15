// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:78498:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:78478:18 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Empty function for testing sqlite3_result_value
}

static void test_result_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This is just a placeholder
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy function to get a context
    sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, 
                           dummy_function, NULL, NULL);
    
    // Prepare a statement to get a context
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func(?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind data based on first byte
    uint8_t type_selector = Data[0] % 5;
    size_t data_offset = 1;
    size_t remaining = (Size > data_offset) ? Size - data_offset : 0;
    
    switch (type_selector) {
        case 0: // Bind as blob
            if (remaining > 0) {
                sqlite3_bind_blob(stmt, 1, Data + data_offset, 
                                 (remaining > 1024) ? 1024 : remaining, 
                                 SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_blob(stmt, 1, NULL, 0, SQLITE_STATIC);
            }
            break;
        case 1: // Bind as text
            if (remaining > 0) {
                sqlite3_bind_text(stmt, 1, (const char*)(Data + data_offset), 
                                 (remaining > 1024) ? 1024 : remaining, 
                                 SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
            }
            break;
        case 2: // Bind as integer
            sqlite3_bind_int64(stmt, 1, (remaining > 0) ? Data[data_offset] : 0);
            break;
        case 3: // Bind as double
            sqlite3_bind_double(stmt, 1, (remaining > 0) ? (double)Data[data_offset] : 0.0);
            break;
        case 4: // Bind as null
            sqlite3_bind_null(stmt, 1);
            break;
    }
    
    // Step to execute and get context
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get the context from the function (though we can't directly access it)
        // We'll create a protected value using sqlite3_bind_pointer
        sqlite3_value *protected_value = NULL;
        
        // Create another statement to get a protected value
        sqlite3_stmt *stmt2 = NULL;
        const char *sql2 = "SELECT ?";
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) == SQLITE_OK) {
            // Bind a pointer value if we have enough data
            if (Size > 2) {
                const char *ptr_type = "test_type";
                if (Size > 10) {
                    // Use part of the data as type string (null-terminated)
                    char *type_str = malloc(11);
                    if (type_str) {
                        memcpy(type_str, Data, 10);
                        type_str[10] = '\0';
                        sqlite3_bind_pointer(stmt2, 1, (void*)Data, type_str, NULL);
                        free(type_str);
                    } else {
                        sqlite3_bind_pointer(stmt2, 1, (void*)Data, ptr_type, NULL);
                    }
                } else {
                    sqlite3_bind_pointer(stmt2, 1, (void*)Data, ptr_type, NULL);
                }
                
                if (sqlite3_step(stmt2) == SQLITE_ROW) {
                    protected_value = sqlite3_column_value(stmt2, 0);
                    // Duplicate to get a protected value
                    sqlite3_value *dup_value = sqlite3_value_dup(protected_value);
                    if (dup_value) {
                        // Test the target functions
                        const void *blob_result = sqlite3_value_blob(dup_value);
                        const void *text16_result = sqlite3_value_text16(dup_value);
                        const void *text16be_result = sqlite3_value_text16be(dup_value);
                        const void *text16le_result = sqlite3_value_text16le(dup_value);
                        
                        // Test sqlite3_value_pointer with different type strings
                        void *ptr_result = NULL;
                        if (Size > 20) {
                            // Create type string from data
                            char type_buf[21];
                            size_t type_len = (Size > 40) ? 20 : Size - 20;
                            memcpy(type_buf, Data + 20, type_len);
                            type_buf[type_len] = '\0';
                            ptr_result = sqlite3_value_pointer(dup_value, type_buf);
                        } else {
                            ptr_result = sqlite3_value_pointer(dup_value, "test_type");
                            ptr_result = sqlite3_value_pointer(dup_value, "different_type");
                            ptr_result = sqlite3_value_pointer(dup_value, NULL);
                        }
                        
                        // Test sqlite3_result_value - we need a context for this
                        // Create another function context
                        void *user_data = NULL;
                        sqlite3_create_function(db, "test_result", 0, SQLITE_UTF8, user_data,
                                               test_result_function, NULL, NULL);
                        
                        sqlite3_stmt *stmt3 = NULL;
                        if (sqlite3_prepare_v2(db, "SELECT test_result()", -1, &stmt3, NULL) == SQLITE_OK) {
                            if (sqlite3_step(stmt3) == SQLITE_ROW) {
                                // We can't easily get the context here, but we've tested other functions
                            }
                            sqlite3_finalize(stmt3);
                        }
                        
                        sqlite3_value_free(dup_value);
                    }
                }
            }
            sqlite3_finalize(stmt2);
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}