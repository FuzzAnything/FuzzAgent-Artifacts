// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_func_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 1) {
        sqlite3_result_value(ctx, argv[0]);
    }
}

static void dummy_destructor(void *ptr) {
    // Do nothing - used for MEM_Dyn values
}

static sqlite3_value *create_protected_value(const uint8_t *data, size_t size, int value_type) {
    // Create a database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a prepared statement to get a protected value through binding
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind the data based on value type
    int bind_result = SQLITE_OK;
    switch (value_type) {
        case SQLITE_BLOB:
            bind_result = sqlite3_bind_blob(stmt, 1, data, (int)(size > 0 ? size : 0), 
                                           size > 0 ? SQLITE_STATIC : SQLITE_TRANSIENT);
            break;
        case SQLITE_TEXT:
            bind_result = sqlite3_bind_text(stmt, 1, (const char*)data, (int)(size > 0 ? size : 0),
                                           size > 0 ? SQLITE_STATIC : SQLITE_TRANSIENT);
            break;
        case SQLITE_INTEGER:
            if (size >= sizeof(int)) {
                int int_val = *(int*)data;
                bind_result = sqlite3_bind_int(stmt, 1, int_val);
            } else {
                bind_result = sqlite3_bind_int(stmt, 1, 0);
            }
            break;
        case SQLITE_FLOAT:
            if (size >= sizeof(double)) {
                double double_val = *(double*)data;
                bind_result = sqlite3_bind_double(stmt, 1, double_val);
            } else {
                bind_result = sqlite3_bind_double(stmt, 1, 0.0);
            }
            break;
        default:
            bind_result = sqlite3_bind_null(stmt, 1);
            break;
    }
    
    if (bind_result != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    // Step to execute the statement
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    // Get the unprotected value from column
    sqlite3_value *unprotected_val = sqlite3_column_value(stmt, 0);
    
    // Duplicate it to get a protected value
    sqlite3_value *protected_val = sqlite3_value_dup(unprotected_val);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected_val;
}

static void test_functions_on_value(sqlite3_value *val) {
    if (!val) return;
    
    // Test sqlite3_value_bytes16
    int bytes16 = sqlite3_value_bytes16(val);
    (void)bytes16;
    
    // Test sqlite3_value_blob
    const void *blob = sqlite3_value_blob(val);
    int blob_size = sqlite3_value_bytes(val);
    (void)blob;
    (void)blob_size;
    
    // Test sqlite3_value_type
    int type = sqlite3_value_type(val);
    (void)type;
    
    // Test sqlite3_value_int
    int int_val = sqlite3_value_int(val);
    (void)int_val;
    
    // Test sqlite3_value_numeric_type
    int numeric_type = sqlite3_value_numeric_type(val);
    (void)numeric_type;
    
    // For sqlite3_result_value, we need a context
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Create a simple scalar function to get a context
        sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, 
                               test_func_impl, NULL, NULL);
        
        // Prepare and execute a statement to trigger the function
        sqlite3_stmt *stmt;
        const char *sql = "SELECT test_func(?)";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            // Bind a dummy value
            sqlite3_bind_int(stmt, 1, 42);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test with different value types
    int types_to_test[] = {SQLITE_BLOB, SQLITE_TEXT, SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_NULL};
    
    for (int i = 0; i < 5; i++) {
        sqlite3_value *val = create_protected_value(Data, Size, types_to_test[i]);
        if (val) {
            test_functions_on_value(val);
            sqlite3_value_free(val);
        }
    }
    
    // Also test with a file-based database for variety
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
        
        sqlite3 *db = NULL;
        if (sqlite3_open("./dummy_file", &db) == SQLITE_OK) {
            sqlite3_close(db);
        }
        
        remove("./dummy_file");
    }
    
    return 0;
}