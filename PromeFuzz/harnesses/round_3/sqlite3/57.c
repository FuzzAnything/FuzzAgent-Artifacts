// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const void *result;
    int col_count;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with various column types
    const char *create_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB, "
        "note VARCHAR(100)"
        ");";
    
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some sample data
    const char *insert_sql = 
        "INSERT INTO test_table (name, value, data, note) "
        "VALUES ('sample', 3.14159, x'01020304', 'test note');";
    
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to get first row
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Get column count using sqlite3_data_count
        col_count = sqlite3_data_count(stmt);
        
        // Use fuzzer data to determine which columns to query
        // Ensure indices are within bounds
        for (int i = 0; i < col_count && i < 5; i++) {
            int col_idx = (Data[i % Size] + i) % col_count;
            
            // Call target functions with the column index
            result = sqlite3_column_decltype16(stmt, col_idx);
            result = sqlite3_column_text16(stmt, col_idx);
            result = sqlite3_column_table_name16(stmt, col_idx);
            result = sqlite3_column_database_name16(stmt, col_idx);
            result = sqlite3_column_name16(stmt, col_idx);
        }
        
        // Also test with some out-of-bounds indices (using fuzzer data)
        if (Size > 1) {
            int out_of_bounds_idx = Data[0] + col_count;
            result = sqlite3_column_decltype16(stmt, out_of_bounds_idx);
            result = sqlite3_column_text16(stmt, out_of_bounds_idx);
            result = sqlite3_column_table_name16(stmt, out_of_bounds_idx);
            result = sqlite3_column_database_name16(stmt, out_of_bounds_idx);
            result = sqlite3_column_name16(stmt, out_of_bounds_idx);
        }
        
        // Test negative indices
        if (Size > 2) {
            int neg_idx = -(int)(Data[1] % 10);
            result = sqlite3_column_decltype16(stmt, neg_idx);
            result = sqlite3_column_text16(stmt, neg_idx);
            result = sqlite3_column_table_name16(stmt, neg_idx);
            result = sqlite3_column_database_name16(stmt, neg_idx);
            result = sqlite3_column_name16(stmt, neg_idx);
        }
    }
    
    // Reset and step again to test different states
    sqlite3_reset(stmt);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Test with valid indices again
        col_count = sqlite3_data_count(stmt);
        if (col_count > 0) {
            for (int i = 0; i < 3 && i < col_count; i++) {
                result = sqlite3_column_decltype16(stmt, i);
                result = sqlite3_column_text16(stmt, i);
                result = sqlite3_column_table_name16(stmt, i);
                result = sqlite3_column_database_name16(stmt, i);
                result = sqlite3_column_name16(stmt, i);
            }
        }
    }
    
    // Step to completion
    while (rc == SQLITE_ROW) {
        rc = sqlite3_step(stmt);
    }
    
    // Test functions after completion (should return 0 for data_count)
    col_count = sqlite3_data_count(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}