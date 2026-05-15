// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO test VALUES (?, ?, ?, ?, ?)";
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a test table with appropriate columns
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (col1 INTEGER, col2 INTEGER, col3 REAL, col4 TEXT, col5 INTEGER)", 
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract values from fuzzer input
    int bind_idx = 1;
    
    // First sqlite3_bind_int
    int int_val1 = 0;
    if (Size >= sizeof(int)) {
        memcpy(&int_val1, Data, sizeof(int));
    }
    sqlite3_bind_int(stmt, bind_idx++, int_val1);
    
    // sqlite3_bind_int64
    sqlite3_int64 int64_val = 0;
    if (Size >= sizeof(int) + sizeof(sqlite3_int64)) {
        memcpy(&int64_val, Data + sizeof(int), sizeof(sqlite3_int64));
    }
    sqlite3_bind_int64(stmt, bind_idx++, int64_val);
    
    // sqlite3_bind_double
    double double_val = 0.0;
    if (Size >= sizeof(int) + sizeof(sqlite3_int64) + sizeof(double)) {
        memcpy(&double_val, Data + sizeof(int) + sizeof(sqlite3_int64), sizeof(double));
    }
    sqlite3_bind_double(stmt, bind_idx++, double_val);
    
    // sqlite3_bind_text64
    const char *text_data = NULL;
    sqlite3_uint64 text_len = 0;
    unsigned char encoding = SQLITE_UTF8;
    
    if (Size > sizeof(int) + sizeof(sqlite3_int64) + sizeof(double)) {
        size_t text_offset = sizeof(int) + sizeof(sqlite3_int64) + sizeof(double);
        text_data = (const char *)(Data + text_offset);
        text_len = Size - text_offset;
        
        // Use a small subset for text to avoid excessive memory usage
        if (text_len > 1024) {
            text_len = 1024;
        }
        
        // Determine encoding from input data
        if (text_len > 0 && (text_data[0] & 0x80)) {
            encoding = SQLITE_UTF16;
        }
    }
    
    sqlite3_bind_text64(stmt, bind_idx++, text_data, text_len, 
                       SQLITE_TRANSIENT, encoding);
    
    // Second sqlite3_bind_int
    int int_val2 = 0;
    if (Size >= sizeof(int)) {
        // Use same value as first int for simplicity
        memcpy(&int_val2, Data, sizeof(int));
    }
    sqlite3_bind_int(stmt, bind_idx++, int_val2);
    
    // Execute the statement
    sqlite3_step(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}