// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
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

static void dummy_callback(void *pArg, int argc, char **argv, char **colName) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const void *result;
    int col_count;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    // Create a simple table with some columns
    const char *create_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data using the fuzzer input
    char insert_sql[256];
    // Use first few bytes as values, ensuring we don't overflow
    int int_val = (Size > 0) ? Data[0] : 0;
    double real_val = (Size > 1) ? Data[1] / 256.0 : 0.0;
    
    snprintf(insert_sql, sizeof(insert_sql),
             "INSERT INTO test_table (id, name, value, data) "
             "VALUES (%d, 'test', %f, x'%02x%02x');",
             int_val, real_val,
             (Size > 2) ? Data[2] : 0,
             (Size > 3) ? Data[3] : 0);
    
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to get a row
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Get column count using sqlite3_data_count
        col_count = sqlite3_data_count(stmt);
        
        // For each column, call the target functions
        for (int i = 0; i < col_count && i < 4; i++) {
            // Use fuzzer data to determine which column index to test
            int col_idx = i;
            if (Size > 4 + i) {
                col_idx = Data[4 + i] % col_count;
            }
            
            // Call each target function
            result = sqlite3_column_database_name16(stmt, col_idx);
            result = sqlite3_column_decltype16(stmt, col_idx);
            result = sqlite3_column_table_name16(stmt, col_idx);
            result = sqlite3_column_text16(stmt, col_idx);
            result = sqlite3_column_name16(stmt, col_idx);
        }
        
        // Also test with potentially invalid column indices
        if (Size > 8) {
            int invalid_idx = Data[8] % 10;  // Could be out of bounds
            result = sqlite3_column_database_name16(stmt, invalid_idx);
            result = sqlite3_column_decltype16(stmt, invalid_idx);
            result = sqlite3_column_table_name16(stmt, invalid_idx);
            result = sqlite3_column_text16(stmt, invalid_idx);
            result = sqlite3_column_name16(stmt, invalid_idx);
        }
        
        // Test sqlite3_data_count with NULL statement
        col_count = sqlite3_data_count(NULL);
    }
    
    // Reset and step again to test different states
    sqlite3_reset(stmt);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        col_count = sqlite3_data_count(stmt);
        if (col_count > 0) {
            result = sqlite3_column_database_name16(stmt, 0);
            result = sqlite3_column_decltype16(stmt, 0);
        }
    }
    
    // Step until done to test after completion
    while (rc == SQLITE_ROW) {
        rc = sqlite3_step(stmt);
    }
    
    // Test functions after SQLITE_DONE
    col_count = sqlite3_data_count(stmt);
    if (col_count > 0) {
        result = sqlite3_column_database_name16(stmt, 0);
        result = sqlite3_column_decltype16(stmt, 0);
    }
    
    // Create another table with attached database if we have enough data
    if (Size > 16) {
        // Use a file database for attachment testing
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 100 ? 100 : Size, fp);
            fclose(fp);
            
            sqlite3 *db2 = NULL;
            rc = sqlite3_open("./dummy_file", &db2);
            if (rc == SQLITE_OK) {
                // Attach the database
                char attach_sql[256];
                snprintf(attach_sql, sizeof(attach_sql),
                         "ATTACH DATABASE './dummy_file' AS attached_db;");
                sqlite3_exec(db, attach_sql, NULL, NULL, NULL);
                
                // Create a multi-table query
                const char *multi_sql = 
                    "SELECT test_table.id, test_table.name "
                    "FROM test_table, attached_db.sqlite_master;";
                
                sqlite3_stmt *stmt2 = NULL;
                rc = sqlite3_prepare_v2(db, multi_sql, -1, &stmt2, NULL);
                if (rc == SQLITE_OK) {
                    if (sqlite3_step(stmt2) == SQLITE_ROW) {
                        col_count = sqlite3_data_count(stmt2);
                        for (int i = 0; i < col_count && i < 2; i++) {
                            result = sqlite3_column_database_name16(stmt2, i);
                            result = sqlite3_column_table_name16(stmt2, i);
                        }
                    }
                    sqlite3_finalize(stmt2);
                }
                
                sqlite3_close(db2);
            }
            remove("./dummy_file");
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}