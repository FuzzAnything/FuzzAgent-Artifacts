// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void create_test_database(sqlite3 *db) {
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB"
        ");"
        "INSERT OR IGNORE INTO test_table (name, value) VALUES ('test1', 1.5);"
        "INSERT OR IGNORE INTO test_table (name, value) VALUES ('test2', 2.5);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create test table and data
    create_test_database(db);
    
    // Prepare a statement using fuzzer input as SQL if possible
    const char *sql = "SELECT * FROM test_table";
    if (Size > 0 && Data[0] % 2 == 0) {
        // Use a dummy file for some test cases
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
            
            // Try to prepare a statement from file content (simplified)
            const char *alt_sql = "SELECT id, name FROM test_table WHERE id > 0";
            if (sqlite3_prepare_v2(db, alt_sql, -1, &stmt, &tail) == SQLITE_OK) {
                // Step to get a row
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    // Test sqlite3_data_count
                    int col_count = sqlite3_data_count(stmt);
                    
                    // Test column functions with various indices
                    for (int i = 0; i < col_count; i++) {
                        // Use fuzzer data to determine which columns to access
                        if (Size > i && (Data[i] % 3) == 0) {
                            (void)sqlite3_column_text16(stmt, i);
                            (void)sqlite3_column_table_name16(stmt, i);
                            (void)sqlite3_column_database_name16(stmt, i);
                            (void)sqlite3_column_origin_name16(stmt, i);
                            (void)sqlite3_column_decltype16(stmt, i);
                        }
                    }
                    
                    // Also test with out-of-bounds indices (using fuzzer data)
                    if (Size > 0) {
                        int idx = Data[0] % 20;  // 0-19, some out of bounds
                        (void)sqlite3_column_text16(stmt, idx);
                        (void)sqlite3_column_table_name16(stmt, idx);
                        (void)sqlite3_column_database_name16(stmt, idx);
                        (void)sqlite3_column_origin_name16(stmt, idx);
                        (void)sqlite3_column_decltype16(stmt, idx);
                    }
                }
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }
    
    // Prepare another statement for more testing
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Step through multiple rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_data_count(stmt);
            
            // Test with valid column indices
            for (int i = 0; i < col_count; i++) {
                (void)sqlite3_column_text16(stmt, i);
                (void)sqlite3_column_table_name16(stmt, i);
                (void)sqlite3_column_database_name16(stmt, i);
                (void)sqlite3_column_origin_name16(stmt, i);
                (void)sqlite3_column_decltype16(stmt, i);
            }
            
            // Test edge cases with fuzzer data
            if (Size > 0) {
                // Use fuzzer data to create negative indices sometimes
                int idx = (int)Data[Size % Size] - 128;
                (void)sqlite3_data_count(stmt);  // Call again
                (void)sqlite3_column_text16(stmt, idx);
                (void)sqlite3_column_table_name16(stmt, idx);
            }
        }
        
        // Test data_count after completion (should return 0)
        (void)sqlite3_data_count(stmt);
        
        sqlite3_finalize(stmt);
    }
    
    // Test with NULL statement
    (void)sqlite3_data_count(NULL);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}