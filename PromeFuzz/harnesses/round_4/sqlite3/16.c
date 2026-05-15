// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *dummy_file = "./dummy_file";
    
    // Use first 4 bytes for busy_timeout parameter
    int busy_timeout = 0;
    if (Size >= sizeof(int)) {
        memcpy(&busy_timeout, Data, sizeof(int));
    }
    
    // Use next 4 bytes for sleep parameter
    int sleep_ms = 0;
    if (Size >= 2 * sizeof(int)) {
        memcpy(&sleep_ms, Data + sizeof(int), sizeof(int));
    }
    
    // Open database
    rc = sqlite3_open(dummy_file, &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set busy timeout
    sqlite3_busy_timeout(db, busy_timeout);
    
    // Create a simple table and insert some data if it doesn't exist
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, value INTEGER);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    
    // Insert some data
    const char *insert_sql = "INSERT OR IGNORE INTO test(value) VALUES(42);";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    
    // Prepare a statement
    const char *select_sql = "SELECT value FROM test WHERE id = 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt) {
        // Step through the statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Get column value
            int col_value = sqlite3_column_int(stmt, 0);
            (void)col_value; // Use value to avoid unused variable warning
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Sleep
    sqlite3_sleep(sleep_ms);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}