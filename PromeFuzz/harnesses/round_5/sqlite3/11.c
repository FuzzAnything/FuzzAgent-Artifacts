// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:79659:25 in sqlite3.h
// sqlite3_column_double at sqlite3.c:79649:19 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    int iCol;
    int colCount;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with various data types
    const char *create_sql = 
        "CREATE TABLE test ("
        "id INTEGER PRIMARY KEY, "
        "int_col INTEGER, "
        "real_col REAL, "
        "text_col TEXT, "
        "blob_col BLOB, "
        "null_col INTEGER"
        ");";
    
    rc = sqlite3_exec(db, create_sql, callback, 0, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some data using the fuzzer input
    // Use the first part of input for integer values
    sqlite3_int64 int_val = 0;
    double real_val = 0.0;
    const char *text_val = "test";
    const void *blob_data = Data;
    int blob_size = Size > 100 ? 100 : Size;
    
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&int_val, Data, sizeof(sqlite3_int64));
    }
    if (Size >= sizeof(sqlite3_int64) + sizeof(double)) {
        memcpy(&real_val, Data + sizeof(sqlite3_int64), sizeof(double));
    }
    
    // Prepare an INSERT statement
    const char *insert_sql = 
        "INSERT INTO test (int_col, real_col, text_col, blob_col) "
        "VALUES (?1, ?2, ?3, ?4);";
    
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind values
    sqlite3_bind_int64(stmt, 1, int_val);
    sqlite3_bind_double(stmt, 2, real_val);
    sqlite3_bind_text(stmt, 3, text_val, -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 4, blob_data, blob_size, SQLITE_STATIC);
    
    // Execute the INSERT
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // Prepare a SELECT statement to test the column functions
    const char *select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step to get a row
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Get column count
    colCount = sqlite3_column_count(stmt);
    
    // Test functions on each column
    for (iCol = 0; iCol < colCount; iCol++) {
        // 1. sqlite3_column_count (already called above)
        // 2. sqlite3_column_type
        int colType = sqlite3_column_type(stmt, iCol);
        
        // 3. sqlite3_column_int64
        sqlite3_int64 int64_val = sqlite3_column_int64(stmt, iCol);
        
        // 4. sqlite3_column_double
        double double_val = sqlite3_column_double(stmt, iCol);
        
        // 5. sqlite3_value_encoding - need a value first
        sqlite3_value *value = sqlite3_column_value(stmt, iCol);
        int encoding = sqlite3_value_encoding(value);
        
        // 6. sqlite3_column_value (already called above)
        // 7. sqlite3_column_bytes
        int bytes = sqlite3_column_bytes(stmt, iCol);
        
        // 8. sqlite3_column_blob
        const void *blob = sqlite3_column_blob(stmt, iCol);
        
        // 9. sqlite3_column_bytes16
        int bytes16 = sqlite3_column_bytes16(stmt, iCol);
        
        // 10. sqlite3_column_blob (again)
        blob = sqlite3_column_blob(stmt, iCol);
        
        // 11. sqlite3_column_bytes16 (again)
        bytes16 = sqlite3_column_bytes16(stmt, iCol);
        
        // 12. sqlite3_column_blob (again)
        blob = sqlite3_column_blob(stmt, iCol);
        
        // 13. sqlite3_value_encoding (again)
        encoding = sqlite3_value_encoding(value);
        
        // 14. sqlite3_column_value (again)
        value = sqlite3_column_value(stmt, iCol);
        
        // 15. sqlite3_column_bytes (again)
        bytes = sqlite3_column_bytes(stmt, iCol);
        
        // 16. sqlite3_column_blob (again)
        blob = sqlite3_column_blob(stmt, iCol);
        
        // Use the values to avoid compiler warnings
        (void)colType;
        (void)int64_val;
        (void)double_val;
        (void)encoding;
        (void)bytes;
        (void)blob;
        (void)bytes16;
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}