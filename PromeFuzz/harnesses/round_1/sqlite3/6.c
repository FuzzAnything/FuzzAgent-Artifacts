// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_xDel(void *ptr) {
    // Do nothing - used for MEM_Dyn destructor
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with various column types
    const char *create_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "text_col TEXT, "
        "int_col INTEGER, "
        "blob_col BLOB, "
        "value_col BLOB"
        ");";
    
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some dummy data using fuzzer input
    const char *insert_sql = "INSERT INTO test_table (text_col, int_col, blob_col, value_col) VALUES (?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        if (stmt) sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Bind parameters with fuzzer data
    if (Size > 0) {
        // Bind text (truncate if needed)
        int text_len = Size > 100 ? 100 : (int)Size;
        sqlite3_bind_text(stmt, 1, (const char*)Data, text_len, SQLITE_TRANSIENT);
        
        // Bind integer (use first byte as integer)
        int int_val = Size > 0 ? Data[0] : 0;
        sqlite3_bind_int(stmt, 2, int_val);
        
        // Bind blob (use fuzzer data)
        int blob_len = Size > 50 ? 50 : (int)Size;
        sqlite3_bind_blob(stmt, 3, Data, blob_len, SQLITE_TRANSIENT);
        
        // Bind value (create a dummy sqlite3_value)
        sqlite3_bind_int(stmt, 4, 42);
    } else {
        // Bind NULL values if no data
        sqlite3_bind_null(stmt, 1);
        sqlite3_bind_null(stmt, 2);
        sqlite3_bind_null(stmt, 3);
        sqlite3_bind_null(stmt, 4);
    }
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // Prepare a SELECT statement to test column functions
    const char *select_sql = "SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        if (stmt) sqlite3_finalize(stmt);
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
    
    // Now call the API functions in the specified order
    // Use column indices based on fuzzer data to explore different states
    int col_count = sqlite3_column_count(stmt);
    if (col_count == 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Generate column indices from fuzzer data
    int idx1 = Size > 0 ? Data[0] % col_count : 0;
    int idx2 = Size > 1 ? Data[1] % col_count : 0;
    int idx3 = Size > 2 ? Data[2] % col_count : 0;
    int idx4 = Size > 3 ? Data[3] % col_count : 0;
    int idx5 = Size > 4 ? Data[4] % col_count : 0;
    
    // 1. sqlite3_column_text
    const unsigned char *text_result = sqlite3_column_text(stmt, idx1);
    (void)text_result;
    
    // 2. sqlite3_bind_value
    // First we need to get a value to bind
    sqlite3_value *value1 = sqlite3_column_value(stmt, idx2);
    
    // Prepare another statement for binding
    const char *insert2_sql = "INSERT INTO test_table (text_col) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert2_sql, -1, &stmt2, NULL);
    if (rc == SQLITE_OK && stmt2) {
        sqlite3_bind_value(stmt2, 1, value1);
        sqlite3_finalize(stmt2);
        stmt2 = NULL;
    }
    
    // 3. sqlite3_column_value
    sqlite3_value *value2 = sqlite3_column_value(stmt, idx3);
    (void)value2;
    
    // 4. sqlite3_bind_value
    // Prepare another statement for second binding
    rc = sqlite3_prepare_v2(db, insert2_sql, -1, &stmt2, NULL);
    if (rc == SQLITE_OK && stmt2) {
        sqlite3_bind_value(stmt2, 1, value2);
        sqlite3_finalize(stmt2);
        stmt2 = NULL;
    }
    
    // 5. sqlite3_column_value
    sqlite3_value *value3 = sqlite3_column_value(stmt, idx4);
    (void)value3;
    
    // 6. sqlite3_step
    rc = sqlite3_step(stmt);
    
    // If we got SQLITE_ROW, we can continue with column functions
    if (rc == SQLITE_ROW) {
        // 7. sqlite3_column_int
        int int_result = sqlite3_column_int(stmt, idx5);
        (void)int_result;
    }
    
    // 8. sqlite3_reset
    rc = sqlite3_reset(stmt);
    
    // Step again after reset
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Generate new indices for remaining calls
        int idx6 = Size > 5 ? Data[5] % col_count : 0;
        int idx7 = Size > 6 ? Data[6] % col_count : 0;
        int idx8 = Size > 7 ? Data[7] % col_count : 0;
        int idx9 = Size > 8 ? Data[8] % col_count : 0;
        int idx10 = Size > 9 ? Data[9] % col_count : 0;
        int idx11 = Size > 10 ? Data[10] % col_count : 0;
        
        // 9. sqlite3_column_bytes16
        int bytes16_1 = sqlite3_column_bytes16(stmt, idx6);
        (void)bytes16_1;
        
        // 10. sqlite3_column_blob
        const void *blob1 = sqlite3_column_blob(stmt, idx7);
        (void)blob1;
        
        // 11. sqlite3_column_bytes16
        int bytes16_2 = sqlite3_column_bytes16(stmt, idx8);
        (void)bytes16_2;
        
        // 12. sqlite3_column_blob
        const void *blob2 = sqlite3_column_blob(stmt, idx9);
        (void)blob2;
        
        // 13. sqlite3_column_bytes
        int bytes1 = sqlite3_column_bytes(stmt, idx10);
        (void)bytes1;
        
        // 14. sqlite3_column_blob
        const void *blob3 = sqlite3_column_blob(stmt, idx11);
        (void)blob3;
        
        // 15. sqlite3_column_bytes
        int bytes2 = sqlite3_column_bytes(stmt, idx6);
        (void)bytes2;
        
        // 16. sqlite3_column_blob
        const void *blob4 = sqlite3_column_blob(stmt, idx7);
        (void)blob4;
    }
    
    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close(db);
    
    return 0;
}

#ifdef __cplusplus
}
#endif