// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:78603:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_functions(sqlite3_value* value) {
    if (value == NULL) {
        return;
    }
    
    // Call all target API functions on the provided value
    int type = sqlite3_value_type(value);
    int bytes16 = sqlite3_value_bytes16(value);
    int bytes = sqlite3_value_bytes(value);
    const void* blob = sqlite3_value_blob(value);
    int int_val = sqlite3_value_int(value);
    int from_bind = sqlite3_value_frombind(value);
    
    // Use the results to avoid dead code elimination
    volatile int dummy = type + bytes16 + bytes + int_val + from_bind;
    (void)dummy;
    (void)blob;
}

static sqlite3_value* create_test_value(const uint8_t* data, size_t size) {
    // Create a simple in-memory database
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a table and insert a value
    char* err_msg = NULL;
    const char* create_table = "CREATE TABLE test (col);";
    if (sqlite3_exec(db, create_table, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    // Prepare an INSERT statement with a bound parameter
    sqlite3_stmt* stmt = NULL;
    const char* insert_sql = "INSERT INTO test VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind the fuzzer data as a BLOB
    if (size > 0) {
        sqlite3_bind_blob(stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute the INSERT
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Prepare a SELECT statement to get a protected value
    const char* select_sql = "SELECT col FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Get the value
    sqlite3_step(stmt);
    sqlite3_value* value = sqlite3_column_value(stmt, 0);
    
    // Duplicate the value to get a protected sqlite3_value
    sqlite3_value* protected_value = NULL;
    if (value) {
        protected_value = sqlite3_value_dup(value);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected_value;
}

int LLVMFuzzerTestOneInput_94(const uint8_t* Data, size_t Size) {
    // Create a test value from fuzzer input
    sqlite3_value* test_value = create_test_value(Data, Size);
    
    if (test_value) {
        // Test all target functions
        test_functions(test_value);
        
        // Free the duplicated value
        sqlite3_value_free(test_value);
    }
    
    // Test with different value types by creating various SQLite values
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Create a table with various data types
        sqlite3_exec(db, "CREATE TABLE tst(a, b, c, d, e);", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO tst VALUES (NULL, 42, 3.14, 'text', x'0102');", NULL, NULL, NULL);
        
        sqlite3_stmt* stmt = NULL;
        if (sqlite3_prepare_v2(db, "SELECT * FROM tst;", -1, &stmt, NULL) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                for (int i = 0; i < sqlite3_column_count(stmt); i++) {
                    sqlite3_value* col_value = sqlite3_column_value(stmt, i);
                    if (col_value) {
                        sqlite3_value* protected_val = sqlite3_value_dup(col_value);
                        if (protected_val) {
                            test_functions(protected_val);
                            sqlite3_value_free(protected_val);
                        }
                    }
                }
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }
    
    return 0;
}