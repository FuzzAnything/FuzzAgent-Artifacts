// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dummy_sql = "SELECT 1, 2.5, 'test', x'0102', NULL;";
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a simple statement
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL);
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
    
    // Now we have a row, call the functions in the specified order
    // Use column indices 0-4 (since our SELECT has 5 columns)
    int col_count = sqlite3_column_count(stmt);
    
    // Call functions with different column indices based on fuzzer input
    // Use Data to determine which column to access (mod col_count)
    int col_idx = 0;
    if (Size > 0 && col_count > 0) {
        col_idx = Data[0] % col_count;
    }
    
    // 1. sqlite3_column_count
    // Already called above
    
    // 2. sqlite3_column_type
    int col_type = sqlite3_column_type(stmt, col_idx);
    
    // 3. sqlite3_column_int64
    sqlite3_int64 int_val = sqlite3_column_int64(stmt, col_idx);
    
    // 4. sqlite3_column_double
    double double_val = sqlite3_column_double(stmt, col_idx);
    
    // 5. sqlite3_value_encoding
    // First get a value from sqlite3_column_value
    sqlite3_value *value = sqlite3_column_value(stmt, col_idx);
    int encoding = sqlite3_value_encoding(value);
    
    // 6. sqlite3_column_value (already called above for encoding)
    // Call again with different column index
    int col_idx2 = 1;
    if (Size > 1 && col_count > 0) {
        col_idx2 = Data[1 % Size] % col_count;
    }
    sqlite3_value *value2 = sqlite3_column_value(stmt, col_idx2);
    
    // 7. sqlite3_column_bytes
    int bytes = sqlite3_column_bytes(stmt, col_idx);
    
    // 8. sqlite3_column_blob
    const void *blob = sqlite3_column_blob(stmt, col_idx);
    
    // 9. sqlite3_column_bytes16
    int bytes16 = sqlite3_column_bytes16(stmt, col_idx);
    
    // 10. sqlite3_column_blob (again)
    const void *blob2 = sqlite3_column_blob(stmt, col_idx2);
    
    // 11. sqlite3_column_bytes16 (again)
    int bytes16_2 = sqlite3_column_bytes16(stmt, col_idx2);
    
    // 12. sqlite3_column_blob (again)
    const void *blob3 = sqlite3_column_blob(stmt, 0);
    
    // 13. sqlite3_value_encoding (again)
    int encoding2 = sqlite3_value_encoding(value2);
    
    // 14. sqlite3_column_value (again)
    sqlite3_value *value3 = sqlite3_column_value(stmt, 0);
    
    // 15. sqlite3_column_bytes (again)
    int bytes2 = sqlite3_column_bytes(stmt, col_idx2);
    
    // 16. sqlite3_column_blob (again)
    const void *blob4 = sqlite3_column_blob(stmt, col_idx2);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    // Use the results to avoid compiler optimizations
    (void)col_count;
    (void)col_type;
    (void)int_val;
    (void)double_val;
    (void)encoding;
    (void)value2;
    (void)bytes;
    (void)blob;
    (void)bytes16;
    (void)blob2;
    (void)bytes16_2;
    (void)blob3;
    (void)encoding2;
    (void)value3;
    (void)bytes2;
    (void)blob4;
    
    return 0;
}