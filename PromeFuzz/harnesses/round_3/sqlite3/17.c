// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Use a fixed database file as per instructions
    const char *filename = "./dummy_file";
    
    // Remove any existing dummy file to start fresh
    remove(filename);
    
    // Write some data to the file to make it non-empty
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        // Write a minimal SQLite database header (magic string)
        const char header[] = "SQLite format 3";
        fwrite(header, 1, sizeof(header)-1, fp);
        fclose(fp);
    }
    
    // 1. sqlite3_open
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        // If open fails, we can't proceed with other operations
        if (db) {
            sqlite3_close(db);
        }
        remove(filename);
        return 0;
    }
    
    // 2. sqlite3_busy_timeout
    // Use first byte of fuzz data (if available) for timeout value
    int timeout_ms = 0;
    if (Size > 0) {
        timeout_ms = Data[0];
    }
    sqlite3_busy_timeout(db, timeout_ms);
    
    // Create a simple table and prepare a statement for sqlite3_step
    // Use a fixed SQL statement
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, value INTEGER);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Insert a row if table creation succeeded
    if (rc == SQLITE_OK) {
        const char *insert_sql = "INSERT INTO test (value) VALUES (42);";
        rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    }
    
    // Prepare a SELECT statement for sqlite3_step
    const char *select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    
    // 3. sqlite3_step (only if prepare succeeded)
    if (rc == SQLITE_OK && stmt) {
        rc = sqlite3_step(stmt);
        
        // 4. sqlite3_column_int (only if we got a row)
        if (rc == SQLITE_ROW) {
            // Try to get column 0 and column 1
            sqlite3_column_int(stmt, 0);
            if (sqlite3_column_count(stmt) > 1) {
                sqlite3_column_int(stmt, 1);
            }
        }
        
        // Reset the statement to allow finalization
        sqlite3_reset(stmt);
        
        // 5. sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // 6. sqlite3_sleep
    // Use second byte of fuzz data (if available) for sleep duration
    int sleep_ms = 0;
    if (Size > 1) {
        sleep_ms = Data[1];
    }
    sqlite3_sleep(sleep_ms);
    
    // 7. sqlite3_close
    sqlite3_close(db);
    
    // Clean up the dummy file
    remove(filename);
    
    return 0;
}