// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
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
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static const char* db_name = "./dummy_file";
    
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *result = NULL;
    int rc;
    
    // Write data to dummy file
    FILE *fp = fopen(db_name, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Open database
    rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table with some columns
    const char *create_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, create_sql, callback, 0, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data
    const char *insert_sql = 
        "INSERT OR REPLACE INTO test_table (id, name, value, data) "
        "VALUES (1, 'test', 3.14, ?);";
    
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_blob(stmt, 1, Data, Size > 1024 ? 1024 : Size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Prepare a SELECT statement
    const char *select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step through the statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        
        // Use the fuzzer input to determine which column to test
        // Use modulo to ensure we stay within bounds
        int col_index = Data[0] % (col_count > 0 ? col_count : 1);
        if (col_index < 0) col_index = 0;
        
        // Test all target functions with the selected column
        result = sqlite3_column_text16(stmt, col_index);
        result = sqlite3_column_table_name16(stmt, col_index);
        result = sqlite3_column_database_name16(stmt, col_index);
        result = sqlite3_column_name16(stmt, col_index);
        result = sqlite3_column_origin_name16(stmt, col_index);
        result = sqlite3_column_decltype16(stmt, col_index);
        
        // Try with different column indices based on fuzzer data
        for (int i = 0; i < 3 && i < col_count; i++) {
            int alt_col = (col_index + i) % (col_count > 0 ? col_count : 1);
            result = sqlite3_column_text16(stmt, alt_col);
            result = sqlite3_column_table_name16(stmt, alt_col);
            result = sqlite3_column_database_name16(stmt, alt_col);
            result = sqlite3_column_name16(stmt, alt_col);
            result = sqlite3_column_origin_name16(stmt, alt_col);
            result = sqlite3_column_decltype16(stmt, alt_col);
        }
        
        // Test edge cases: negative index and out-of-bounds index
        result = sqlite3_column_text16(stmt, -1);
        result = sqlite3_column_text16(stmt, col_count);
        result = sqlite3_column_text16(stmt, col_count + 1);
    }
    
    // Reset and step again to test different states
    sqlite3_reset(stmt);
    rc = sqlite3_step(stmt);
    
    // Final cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}