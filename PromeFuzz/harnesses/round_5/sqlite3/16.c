// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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
    
    // Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set busy timeout - use first 4 bytes of input as timeout value
    int timeout = 0;
    if (Size >= sizeof(int)) {
        memcpy(&timeout, Data, sizeof(int));
    }
    sqlite3_busy_timeout(db, timeout);
    
    // Create a simple table and prepare a statement
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement - use input data to influence SQL if possible
    const char *insert_sql = "INSERT INTO test VALUES(?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind a value - use first 4 bytes as integer value
    int bind_value = 0;
    if (Size >= sizeof(int)) {
        memcpy(&bind_value, Data, sizeof(int));
    }
    sqlite3_bind_int(stmt, 1, bind_value);
    
    // Execute the statement
    rc = sqlite3_step(stmt);
    
    // Finalize the insert statement
    sqlite3_finalize(stmt);
    
    // Prepare and execute a SELECT statement
    const char *select_sql = "SELECT id FROM test";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Get column value
            int col_value = sqlite3_column_int(stmt, 0);
            (void)col_value; // Use value to avoid unused variable warning
        }
        sqlite3_finalize(stmt);
    }
    
    // Sleep - use another 4 bytes as sleep time
    int sleep_time = 0;
    if (Size >= 2 * sizeof(int)) {
        memcpy(&sleep_time, Data + sizeof(int), sizeof(int));
    }
    sqlite3_sleep(sleep_time);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}