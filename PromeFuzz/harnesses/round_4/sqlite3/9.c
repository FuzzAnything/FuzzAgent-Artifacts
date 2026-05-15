// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
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
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void dummy_destructor(void *ptr) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    const char *sql = "SELECT ?1";
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Prepare a statement with a single parameter */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Bind the fuzzer input as a BLOB parameter */
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, Size, dummy_destructor);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    /* Execute the statement to get a row */
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int col_count = 0;
        int col_type = 0;
        sqlite3_int64 int_val = 0;
        double double_val = 0.0;
        int encoding = 0;
        sqlite3_value *value = NULL;
        int bytes = 0;
        int bytes16 = 0;
        const void *blob = NULL;
        const void *blob2 = NULL;
        const void *blob3 = NULL;
        int bytes16_2 = 0;
        int encoding2 = 0;
        sqlite3_value *value2 = NULL;
        int bytes2 = 0;
        const void *blob4 = NULL;
        
        /* Call functions in specified order */
        col_count = sqlite3_column_count(stmt);
        
        if (col_count > 0) {
            int col_idx = 0;
            
            col_type = sqlite3_column_type(stmt, col_idx);
            int_val = sqlite3_column_int64(stmt, col_idx);
            double_val = sqlite3_column_double(stmt, col_idx);
            
            value = sqlite3_column_value(stmt, col_idx);
            if (value) {
                encoding = sqlite3_value_encoding(value);
            }
            
            bytes = sqlite3_column_bytes(stmt, col_idx);
            blob = sqlite3_column_blob(stmt, col_idx);
            
            bytes16 = sqlite3_column_bytes16(stmt, col_idx);
            blob2 = sqlite3_column_blob(stmt, col_idx);
            
            bytes16_2 = sqlite3_column_bytes16(stmt, col_idx);
            blob3 = sqlite3_column_blob(stmt, col_idx);
            
            value2 = sqlite3_column_value(stmt, col_idx);
            if (value2) {
                encoding2 = sqlite3_value_encoding(value2);
            }
            
            bytes2 = sqlite3_column_bytes(stmt, col_idx);
            blob4 = sqlite3_column_blob(stmt, col_idx);
            
            /* Use the results to avoid compiler warnings */
            (void)col_count;
            (void)col_type;
            (void)int_val;
            (void)double_val;
            (void)encoding;
            (void)value;
            (void)bytes;
            (void)blob;
            (void)bytes16;
            (void)blob2;
            (void)bytes16_2;
            (void)blob3;
            (void)encoding2;
            (void)value2;
            (void)bytes2;
            (void)blob4;
        }
    }
    
    /* Reset and finalize the statement */
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    
    /* Close the database */
    sqlite3_close(db);
    
    return 0;
}