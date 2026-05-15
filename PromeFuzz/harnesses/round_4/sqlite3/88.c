// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:78699:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - used for MEM_Dyn values
}

static void test_func_int(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int64(ctx, 42);
}

static void test_func_real(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_double(ctx, 3.14);
}

static void test_func_text(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_text(ctx, "test", 4, SQLITE_STATIC);
}

static void test_func_blob(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    static const uint8_t blob_data[] = {0x01, 0x02, 0x03};
    sqlite3_result_blob(ctx, blob_data, sizeof(blob_data), SQLITE_STATIC);
}

static void test_func_null(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_null(ctx);
}

static sqlite3_value* create_test_value(const uint8_t *data, size_t size, int value_type) {
    sqlite3_value* result_value = NULL;
    
    // Create a temporary database connection
    sqlite3 *temp_db = NULL;
    if (sqlite3_open(":memory:", &temp_db) != SQLITE_OK) {
        return NULL;
    }
    
    // Register the appropriate function based on value_type
    switch (value_type % 5) {
        case 0: // Integer
            sqlite3_create_function(temp_db, "test_func", 0, SQLITE_UTF8, NULL, test_func_int, NULL, NULL);
            break;
        case 1: // Real
            sqlite3_create_function(temp_db, "test_func", 0, SQLITE_UTF8, NULL, test_func_real, NULL, NULL);
            break;
        case 2: // Text
            sqlite3_create_function(temp_db, "test_func", 0, SQLITE_UTF8, NULL, test_func_text, NULL, NULL);
            break;
        case 3: // Blob
            sqlite3_create_function(temp_db, "test_func", 0, SQLITE_UTF8, NULL, test_func_blob, NULL, NULL);
            break;
        case 4: // Null
            sqlite3_create_function(temp_db, "test_func", 0, SQLITE_UTF8, NULL, test_func_null, NULL, NULL);
            break;
    }
    
    // Execute the function and get its result
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func()";
    
    if (sqlite3_prepare_v2(temp_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the value from the result
            sqlite3_value* column_value = sqlite3_column_value(stmt, 0);
            // Duplicate the value so we can use it after statement cleanup
            if (column_value) {
                result_value = sqlite3_value_dup(column_value);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(temp_db);
    return result_value;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, val TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare first statement (INSERT)
    const char *sql1 = "INSERT INTO test(val) VALUES (?);";
    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL);
    if (rc != SQLITE_OK || stmt1 == NULL) {
        if (stmt1) sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement (INSERT with same number of parameters)
    const char *sql2 = "INSERT INTO test(val) VALUES (?);";
    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    if (rc != SQLITE_OK || stmt2 == NULL) {
        if (stmt2) sqlite3_finalize(stmt2);
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare third statement (SELECT to get values)
    const char *sql3 = "SELECT val FROM test LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql3, -1, &stmt3, NULL);
    
    // Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt1);
    
    // Test sqlite3_bind_int64 with fuzzer data
    if (param_count > 0) {
        sqlite3_int64 int_value = 0;
        if (Size >= sizeof(sqlite3_int64)) {
            memcpy(&int_value, Data, sizeof(sqlite3_int64));
        } else if (Size > 0) {
            int_value = (sqlite3_int64)Data[0];
        }
        sqlite3_bind_int64(stmt1, 1, int_value);
    }
    
    // Test sqlite3_bind_value with a created value
    if (param_count > 0 && Size > 0) {
        int value_type = Data[0] % 5;
        sqlite3_value *test_value = create_test_value(Data, Size, value_type);
        if (test_value) {
            sqlite3_bind_value(stmt1, 1, test_value);
            sqlite3_value_free(test_value);
        }
    }
    
    // Test sqlite3_transfer_bindings
    if (stmt1 && stmt2) {
        sqlite3_transfer_bindings(stmt1, stmt2);
    }
    
    // Test sqlite3_expired
    if (stmt1) {
        int expired = sqlite3_expired(stmt1);
        (void)expired; // Use variable to avoid unused warning
    }
    
    // Test sqlite3_reset
    if (stmt1) {
        sqlite3_reset(stmt1);
    }
    if (stmt2) {
        sqlite3_reset(stmt2);
    }
    
    // Try to step the statements to trigger execution
    if (Size > 0 && (Data[0] % 2 == 0)) {
        if (stmt1) sqlite3_step(stmt1);
        if (stmt2) sqlite3_step(stmt2);
        
        // Reset again after stepping
        if (stmt1) sqlite3_reset(stmt1);
        if (stmt2) sqlite3_reset(stmt2);
    }
    
    // Cleanup
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt3) sqlite3_finalize(stmt3);
    if (db) sqlite3_close(db);
    
    return 0;
}