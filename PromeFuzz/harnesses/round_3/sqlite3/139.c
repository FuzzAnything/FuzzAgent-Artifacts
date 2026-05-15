// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void create_dummy_db(sqlite3 **db, sqlite3_stmt **stmt1, sqlite3_stmt **stmt2) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
        *stmt1 = NULL;
        *stmt2 = NULL;
        return;
    }
    
    // Create a table and prepare statements with parameters
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
    sqlite3_exec(*db, create_sql, 0, 0, 0);
    
    // First statement with parameters
    const char *sql1 = "INSERT INTO test (id, name) VALUES (?, ?);";
    sqlite3_prepare_v2(*db, sql1, -1, stmt1, 0);
    
    // Second statement with different number of parameters
    const char *sql2 = "SELECT * FROM test WHERE id = ? OR name = ?;";
    sqlite3_prepare_v2(*db, sql2, -1, stmt2, 0);
}

static sqlite3_value *create_test_value(const uint8_t *data, size_t size) {
    // Create a temporary database to generate sqlite3_value
    sqlite3 *temp_db = NULL;
    sqlite3_stmt *temp_stmt = NULL;
    sqlite3_value *result = NULL;
    
    if (sqlite3_open(":memory:", &temp_db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a table and insert data
    sqlite3_exec(temp_db, "CREATE TABLE temp (val BLOB);", 0, 0, 0);
    
    // Prepare insert statement
    const char *insert_sql = "INSERT INTO temp (val) VALUES (?);";
    if (sqlite3_prepare_v2(temp_db, insert_sql, -1, &temp_stmt, 0) == SQLITE_OK) {
        sqlite3_bind_blob(temp_stmt, 1, data, size > 100 ? 100 : size, SQLITE_TRANSIENT);
        sqlite3_step(temp_stmt);
        sqlite3_reset(temp_stmt);
    }
    
    // Prepare select statement to get a value
    const char *select_sql = "SELECT val FROM temp;";
    sqlite3_finalize(temp_stmt);
    if (sqlite3_prepare_v2(temp_db, select_sql, -1, &temp_stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(temp_stmt) == SQLITE_ROW) {
            result = sqlite3_column_value(temp_stmt, 0);
            // Duplicate the value so it survives after statement finalization
            result = sqlite3_value_dup(result);
        }
    }
    
    sqlite3_finalize(temp_stmt);
    sqlite3_close(temp_db);
    return result;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    
    // Create database and prepared statements
    create_dummy_db(&db, &stmt1, &stmt2);
    if (!db || !stmt1 || !stmt2) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use fuzz data to determine which operations to perform
    if (Size > 0) {
        uint8_t op = Data[0] % 12; // 6 operations * 2 statements
        size_t data_offset = 1;
        
        // Get parameter count for both statements
        int count1 = sqlite3_bind_parameter_count(stmt1);
        int count2 = sqlite3_bind_parameter_count(stmt2);
        
        // Create a test value for binding
        sqlite3_value *test_value = NULL;
        if (Size > data_offset) {
            size_t value_size = Size - data_offset;
            if (value_size > 0) {
                test_value = create_test_value(Data + data_offset, value_size);
            }
        }
        
        switch (op) {
            case 0:
                // Test sqlite3_bind_parameter_count on stmt1
                (void)sqlite3_bind_parameter_count(stmt1);
                break;
            case 1:
                // Test sqlite3_bind_parameter_count on stmt2
                (void)sqlite3_bind_parameter_count(stmt2);
                break;
            case 2:
                // Test sqlite3_expired on stmt1
                (void)sqlite3_expired(stmt1);
                break;
            case 3:
                // Test sqlite3_expired on stmt2
                (void)sqlite3_expired(stmt2);
                break;
            case 4:
                // Test sqlite3_bind_value on stmt1
                if (test_value && count1 > 0) {
                    int idx = (Data[data_offset] % count1) + 1;
                    sqlite3_bind_value(stmt1, idx, test_value);
                }
                break;
            case 5:
                // Test sqlite3_bind_value on stmt2
                if (test_value && count2 > 0) {
                    int idx = (Data[data_offset] % count2) + 1;
                    sqlite3_bind_value(stmt2, idx, test_value);
                }
                break;
            case 6:
                // Test sqlite3_transfer_bindings from stmt1 to stmt2
                sqlite3_transfer_bindings(stmt1, stmt2);
                break;
            case 7:
                // Test sqlite3_transfer_bindings from stmt2 to stmt1
                sqlite3_transfer_bindings(stmt2, stmt1);
                break;
            case 8:
                // Test sqlite3_bind_null on stmt1
                if (count1 > 0) {
                    int idx = (Data[data_offset] % count1) + 1;
                    sqlite3_bind_null(stmt1, idx);
                }
                break;
            case 9:
                // Test sqlite3_bind_null on stmt2
                if (count2 > 0) {
                    int idx = (Data[data_offset] % count2) + 1;
                    sqlite3_bind_null(stmt2, idx);
                }
                break;
            case 10:
                // Test sqlite3_reset on stmt1
                sqlite3_reset(stmt1);
                break;
            case 11:
                // Test sqlite3_reset on stmt2
                sqlite3_reset(stmt2);
                break;
        }
        
        // Free the test value if created
        if (test_value) {
            sqlite3_value_free(test_value);
        }
        
        // Try to step the statements to explore more states
        if (Size > 1 && (Data[1] % 3 == 0)) {
            sqlite3_step(stmt1);
            sqlite3_reset(stmt1);
        }
        if (Size > 2 && (Data[2] % 3 == 0)) {
            sqlite3_step(stmt2);
            sqlite3_reset(stmt2);
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}