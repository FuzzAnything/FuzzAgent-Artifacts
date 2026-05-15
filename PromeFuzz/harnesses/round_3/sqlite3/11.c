// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:79659:25 in sqlite3.h
// sqlite3_column_double at sqlite3.c:79649:19 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:79659:25 in sqlite3.h
// sqlite3_column_double at sqlite3.c:79649:19 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *dummy_file = "./dummy_file";
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and insert some data
    char *err_msg = NULL;
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT, value BLOB);"
        "INSERT INTO test VALUES (1, 'test1', x'010203');"
        "INSERT INTO test VALUES (2, 'test2', x'040506');"
        "INSERT INTO test VALUES (3, 'test3', NULL);", 
        callback, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement
    const char *sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt) {
        int col_count = sqlite3_column_count(stmt);
        
        // Step to get first row
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Call functions in specified order
            if (col_count > 0) {
                // Use column 0 for all calls
                int col_index = 0;
                
                // 1. sqlite3_column_count
                int count = sqlite3_column_count(stmt);
                
                // 2. sqlite3_column_type
                int col_type = sqlite3_column_type(stmt, col_index);
                
                // 3. sqlite3_column_int64
                sqlite3_int64 int_val = sqlite3_column_int64(stmt, col_index);
                
                // 4. sqlite3_column_double
                double double_val = sqlite3_column_double(stmt, col_index);
                
                // 5. sqlite3_value_encoding - Need value first, but order says this comes before column_value
                // We'll get a value and call encoding on it
                sqlite3_value *value1 = sqlite3_column_value(stmt, col_index);
                int encoding1 = sqlite3_value_encoding(value1);
                
                // 6. sqlite3_column_value
                sqlite3_value *value2 = sqlite3_column_value(stmt, col_index);
                
                // 7. sqlite3_column_bytes
                int bytes1 = sqlite3_column_bytes(stmt, col_index);
                
                // 8. sqlite3_column_blob
                const void *blob1 = sqlite3_column_blob(stmt, col_index);
                
                // 9. sqlite3_column_bytes16
                int bytes16_1 = sqlite3_column_bytes16(stmt, col_index);
                
                // 10. sqlite3_column_blob
                const void *blob2 = sqlite3_column_blob(stmt, col_index);
                
                // 11. sqlite3_column_bytes16
                int bytes16_2 = sqlite3_column_bytes16(stmt, col_index);
                
                // 12. sqlite3_column_blob
                const void *blob3 = sqlite3_column_blob(stmt, col_index);
                
                // 13. sqlite3_value_encoding
                sqlite3_value *value3 = sqlite3_column_value(stmt, col_index);
                int encoding2 = sqlite3_value_encoding(value3);
                
                // 14. sqlite3_column_value
                sqlite3_value *value4 = sqlite3_column_value(stmt, col_index);
                
                // 15. sqlite3_column_bytes
                int bytes2 = sqlite3_column_bytes(stmt, col_index);
                
                // 16. sqlite3_column_blob
                const void *blob4 = sqlite3_column_blob(stmt, col_index);
                
                // Use the values to avoid compiler warnings
                (void)count;
                (void)col_type;
                (void)int_val;
                (void)double_val;
                (void)encoding1;
                (void)value2;
                (void)bytes1;
                (void)blob1;
                (void)bytes16_1;
                (void)blob2;
                (void)bytes16_2;
                (void)blob3;
                (void)encoding2;
                (void)value4;
                (void)bytes2;
                (void)blob4;
            }
        }
        
        // Try to explore more states - step through remaining rows
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Call functions on different rows
            if (col_count > 0) {
                int col_index = 0;
                int t = sqlite3_column_type(stmt, col_index);
                sqlite3_int64 i = sqlite3_column_int64(stmt, col_index);
                double d = sqlite3_column_double(stmt, col_index);
                const void *b = sqlite3_column_blob(stmt, col_index);
                int bytes = sqlite3_column_bytes(stmt, col_index);
                int bytes16 = sqlite3_column_bytes16(stmt, col_index);
                sqlite3_value *val = sqlite3_column_value(stmt, col_index);
                
                (void)t;
                (void)i;
                (void)d;
                (void)b;
                (void)bytes;
                (void)bytes16;
                (void)val;
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    // Remove dummy file
    remove(dummy_file);
    
    return 0;
}