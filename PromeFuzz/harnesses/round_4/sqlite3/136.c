// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:78603:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_value_functions(sqlite3_value *value) {
    // Test the target API functions
    // 1. sqlite3_value_frombind
    int from_bind = sqlite3_value_frombind(value);
    (void)from_bind;
    
    // 2. sqlite3_value_bytes
    int bytes = sqlite3_value_bytes(value);
    (void)bytes;
    
    // 3. sqlite3_value_blob
    const void *blob = sqlite3_value_blob(value);
    (void)blob;
    
    // 4. sqlite3_value_bytes16
    int bytes16 = sqlite3_value_bytes16(value);
    (void)bytes16;
    
    // 5. sqlite3_value_int
    int int_val = sqlite3_value_int(value);
    (void)int_val;
    
    // 6. sqlite3_value_type
    int type = sqlite3_value_type(value);
    (void)type;
}

struct UserData {
    const uint8_t *data;
    size_t size;
};

static void fuzz_func_impl(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    struct UserData *ud = (struct UserData *)sqlite3_user_data(ctx);
    
    // Test the value passed as argument (this is a protected value)
    if (argc > 0) {
        test_value_functions(argv[0]);
    }
    
    // Return the fuzzer data as a blob
    if (ud->size > 0) {
        size_t return_size = ud->size > 1000 ? 1000 : ud->size;
        sqlite3_result_blob(ctx, ud->data, (int)return_size, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Prepare a statement to insert data
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test (id, data) VALUES (1, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer data as a blob (limit size to avoid excessive memory)
        size_t data_size = Size > 1000 ? 1000 : Size;
        if (data_size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)data_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        // Execute the statement
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Create a user-defined function to get protected sqlite3_value objects
    // This is the proper way to get protected values for testing
    struct UserData user_data = {Data, Size};
    
    // Define a scalar function that returns the fuzzer data
    sqlite3_create_function(
        db, "fuzz_func", 1, SQLITE_UTF8, &user_data,
        fuzz_func_impl,
        NULL, NULL
    );
    
    // Call the function with various inputs to test protected values
    const char *test_queries[] = {
        "SELECT fuzz_func(NULL)",
        "SELECT fuzz_func(123)",
        "SELECT fuzz_func(3.14)",
        "SELECT fuzz_func('test')",
        "SELECT fuzz_func(x'010203')"
    };
    
    for (int i = 0; i < 5; i++) {
        if (sqlite3_prepare_v2(db, test_queries[i], -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Also test with a virtual table to get protected values from sqlite3_vtab_rhs_value
    // We'll create a simple virtual table using eponymous-only virtual table
    const char *create_vtable_sql = 
        "CREATE VIRTUAL TABLE temp.vtab USING fuzzer;";
    sqlite3_exec(db, create_vtable_sql, NULL, NULL, NULL);
    
    // Query the data back and test with sqlite3_column_value (unprotected)
    const char *select_sql = "SELECT data FROM test WHERE id = 1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the column value (unprotected)
            sqlite3_value *col_value = sqlite3_column_value(stmt, 0);
            
            // Create a protected value from the unprotected one
            sqlite3_value *protected_value = sqlite3_value_dup(col_value);
            if (protected_value) {
                test_value_functions(protected_value);
                sqlite3_value_free(protected_value);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with bound parameters (protected values)
    const char *bind_sql = "SELECT ?1, ?2, ?3, ?4, ?5";
    if (sqlite3_prepare_v2(db, bind_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind various types of values
        sqlite3_bind_int(stmt, 1, 42);
        sqlite3_bind_double(stmt, 2, 3.14159);
        
        size_t bind_size = Size > 100 ? 100 : Size;
        if (bind_size > 0) {
            sqlite3_bind_blob(stmt, 3, Data, (int)bind_size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        
        sqlite3_bind_text(stmt, 4, "fuzz test", -1, SQLITE_TRANSIENT);
        sqlite3_bind_null(stmt, 5);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Test each bound value (these are protected when accessed in a user function)
            // We'll call our fuzz_func with each bound parameter
            for (int i = 0; i < 5; i++) {
                char query[50];
                snprintf(query, sizeof(query), "SELECT fuzz_func(?%d)", i+1);
                sqlite3_stmt *stmt2;
                if (sqlite3_prepare_v2(db, query, -1, &stmt2, NULL) == SQLITE_OK) {
                    // Re-bind the same values
                    switch(i) {
                        case 0: sqlite3_bind_int(stmt2, 1, 42); break;
                        case 1: sqlite3_bind_double(stmt2, 1, 3.14159); break;
                        case 2: 
                            if (bind_size > 0) {
                                sqlite3_bind_blob(stmt2, 1, Data, (int)bind_size, SQLITE_TRANSIENT);
                            } else {
                                sqlite3_bind_null(stmt2, 1);
                            }
                            break;
                        case 3: sqlite3_bind_text(stmt2, 1, "fuzz test", -1, SQLITE_TRANSIENT); break;
                        case 4: sqlite3_bind_null(stmt2, 1); break;
                    }
                    sqlite3_step(stmt2);
                    sqlite3_finalize(stmt2);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    return 0;
}