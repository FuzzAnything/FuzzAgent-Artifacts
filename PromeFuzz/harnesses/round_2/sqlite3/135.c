// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_subtype at sqlite3.c:78474:25 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for MEM_Dyn values
}

static sqlite3_value* create_test_value_from_bind(sqlite3 *db, const uint8_t *data, size_t size, int type) {
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *result = NULL;
    
    // Create a statement with a bound parameter
    if (sqlite3_prepare_v2(db, "SELECT ?", -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    
    // Bind data based on type
    switch (type % 6) {
        case 0: // NULL
            sqlite3_bind_null(stmt, 1);
            break;
        case 1: // INTEGER
            if (size >= sizeof(int)) {
                int int_val;
                memcpy(&int_val, data, sizeof(int));
                sqlite3_bind_int(stmt, 1, int_val);
            } else {
                sqlite3_bind_int(stmt, 1, (int)size);
            }
            break;
        case 2: // REAL
            if (size >= sizeof(double)) {
                double dbl_val;
                memcpy(&dbl_val, data, sizeof(double));
                sqlite3_bind_double(stmt, 1, dbl_val);
            } else {
                sqlite3_bind_double(stmt, 1, (double)size);
            }
            break;
        case 3: // TEXT
            if (size > 0) {
                char *text = (char*)malloc(size + 1);
                if (text) {
                    memcpy(text, data, size);
                    text[size] = '\0';
                    sqlite3_bind_text(stmt, 1, text, -1, free);
                } else {
                    sqlite3_bind_text(stmt, 1, "", -1, SQLITE_STATIC);
                }
            } else {
                sqlite3_bind_text(stmt, 1, "", -1, SQLITE_STATIC);
            }
            break;
        case 4: // BLOB
            if (size > 0) {
                sqlite3_bind_blob(stmt, 1, data, size > 1024 ? 1024 : (int)size, SQLITE_STATIC);
            } else {
                sqlite3_bind_blob(stmt, 1, "", 0, SQLITE_STATIC);
            }
            break;
        case 5: // Zero blob
            sqlite3_bind_zeroblob(stmt, 1, size > 1024 ? 1024 : (int)size);
            break;
    }
    
    // Execute and get column value
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get unprotected value from column
        sqlite3_value *unprotected = sqlite3_column_value(stmt, 0);
        
        // Duplicate to get a protected value
        if (unprotected) {
            result = sqlite3_value_dup(unprotected);
        }
    }
    
    sqlite3_finalize(stmt);
    return result;
}

static void test_functions_on_value(sqlite3_value *val) {
    if (!val) return;
    
    // Test sqlite3_value_int
    int int_result = sqlite3_value_int(val);
    (void)int_result;
    
    // Test sqlite3_value_text16
    const void *text16_result = sqlite3_value_text16(val);
    (void)text16_result;
    
    // Test sqlite3_value_blob
    const void *blob_result = sqlite3_value_blob(val);
    (void)blob_result;
    
    // Test sqlite3_value_subtype
    unsigned int subtype_result = sqlite3_value_subtype(val);
    (void)subtype_result;
    
    // Test sqlite3_value_text16le
    const void *text16le_result = sqlite3_value_text16le(val);
    (void)text16le_result;
}

static void test_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

static void test_result_value(sqlite3 *db) {
    // Register the function
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, test_func, NULL, NULL);
    
    // Call the function with various inputs
    const char* queries[] = {
        "SELECT test_func(NULL)",
        "SELECT test_func(42)",
        "SELECT test_func(3.14)",
        "SELECT test_func('hello')",
        "SELECT test_func(x'010203')"
    };
    
    for (int i = 0; i < 5; i++) {
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, queries[i], -1, &stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                // Function was called
                (void)sqlite3_column_type(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
    }
    
    // Clean up the function
    sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, NULL, NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Open an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Test with different value types
    for (int i = 0; i < 6; i++) {
        sqlite3_value *val = create_test_value_from_bind(db, Data, Size, i);
        if (val) {
            test_functions_on_value(val);
            sqlite3_value_free(val);
        }
    }
    
    // Test sqlite3_result_value
    test_result_value(db);
    
    // Test with NULL value using sqlite3_bind_value
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, "SELECT ?", -1, &stmt, NULL) == SQLITE_OK) {
        // Create a NULL value by binding NULL
        sqlite3_bind_null(stmt, 1);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
            sqlite3_value *protected_val = sqlite3_value_dup(col_val);
            
            if (protected_val) {
                test_functions_on_value(protected_val);
                sqlite3_value_free(protected_val);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with a value from a virtual table (simulated)
    // Create a table and insert some data
    sqlite3_exec(db, "CREATE TABLE test(id INTEGER, data BLOB)", NULL, NULL, NULL);
    
    if (Size > 0) {
        sqlite3_stmt *insert_stmt = NULL;
        if (sqlite3_prepare_v2(db, "INSERT INTO test VALUES(?, ?)", -1, &insert_stmt, NULL) == SQLITE_OK) {
            sqlite3_bind_int(insert_stmt, 1, 1);
            if (Size > 1024) {
                sqlite3_bind_blob(insert_stmt, 2, Data, 1024, SQLITE_STATIC);
            } else {
                sqlite3_bind_blob(insert_stmt, 2, Data, (int)Size, SQLITE_STATIC);
            }
            sqlite3_step(insert_stmt);
            sqlite3_finalize(insert_stmt);
        }
        
        // Query the data and test functions
        sqlite3_stmt *select_stmt = NULL;
        if (sqlite3_prepare_v2(db, "SELECT data FROM test WHERE id=1", -1, &select_stmt, NULL) == SQLITE_OK) {
            if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                sqlite3_value *col_val = sqlite3_column_value(select_stmt, 0);
                sqlite3_value *protected_val = sqlite3_value_dup(col_val);
                
                if (protected_val) {
                    test_functions_on_value(protected_val);
                    sqlite3_value_free(protected_val);
                }
            }
            sqlite3_finalize(select_stmt);
        }
    }
    
    sqlite3_close(db);
    return 0;
}