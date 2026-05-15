// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    if (Size > 0) {
        dummy_file_write(Data, Size);
    }
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const char *err_msg = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare statement using fuzzer input as SQL
    const char *sql = (const char *)Data;
    int sql_len;
    
    // Handle size conversion safely without INT_MAX
    if (Size == 0) {
        sql_len = 0;
    } else if (Size > 1048576) {  // Cap at 1MB to avoid overflow
        sql_len = 1048576;
    } else {
        sql_len = (int)Size;
    }
    
    int prep_result = sqlite3_prepare_v2(db, sql, sql_len, &stmt, &tail);
    
    // Get error message
    err_msg = sqlite3_errmsg(db);
    
    if (stmt) {
        // First step
        int step_result1 = sqlite3_step(stmt);
        
        // Second step (as per specification)
        int step_result2 = sqlite3_step(stmt);
        
        // Get column count
        int col_count = sqlite3_column_count(stmt);
        
        // If we have columns and got a row, process column data
        if (col_count > 0 && step_result1 == SQLITE_ROW) {
            // Process first column
            int col_type = sqlite3_column_type(stmt, 0);
            const char *col_name = sqlite3_column_name(stmt, 0);
            
            // Get column text three times as specified
            const unsigned char *col_text1 = sqlite3_column_text(stmt, 0);
            const unsigned char *col_text2 = sqlite3_column_text(stmt, 0);
            const unsigned char *col_text3 = sqlite3_column_text(stmt, 0);
            
            // Get column bytes
            int col_bytes = sqlite3_column_bytes(stmt, 0);
            
            // Process remaining columns if any
            for (int i = 1; i < col_count && i < 10; i++) {
                // Just call the functions to exercise them
                (void)sqlite3_column_type(stmt, i);
                (void)sqlite3_column_name(stmt, i);
                (void)sqlite3_column_text(stmt, i);
                (void)sqlite3_column_bytes(stmt, i);
            }
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}