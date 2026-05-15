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
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_cleanup(void *ptr) {
    // Do nothing - just a placeholder for the destructor
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Initialize database and statement
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Create in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and prepare a statement with parameters
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (a INT, b INT, c REAL, d TEXT, e INT);";
    if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with 5 parameters
    const char *insert_sql = "INSERT INTO test VALUES (?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract values from fuzzer input
    // We'll use the first bytes for various parameters
    
    int param1 = 1;
    sqlite3_int64 param2 = 0;
    double param3 = 0.0;
    int param5 = 2;
    
    // Get values from Data if we have enough bytes
    if (Size >= sizeof(int)) {
        memcpy(&param1, Data, sizeof(int));
    }
    
    if (Size >= sizeof(int) + sizeof(sqlite3_int64)) {
        memcpy(&param2, Data + sizeof(int), sizeof(sqlite3_int64));
    }
    
    if (Size >= sizeof(int) + sizeof(sqlite3_int64) + sizeof(double)) {
        memcpy(&param3, Data + sizeof(int) + sizeof(sqlite3_int64), sizeof(double));
    }
    
    if (Size >= sizeof(int) + sizeof(sqlite3_int64) + sizeof(double) + sizeof(int)) {
        memcpy(&param5, Data + sizeof(int) + sizeof(sqlite3_int64) + sizeof(double), sizeof(int));
    }
    
    // Calculate string data
    size_t text_offset = sizeof(int) + sizeof(sqlite3_int64) + sizeof(double) + sizeof(int);
    size_t text_size = 0;
    const char *text_data = NULL;
    
    if (Size > text_offset) {
        text_size = Size - text_offset;
        text_data = (const char*)(Data + text_offset);
    } else {
        // Use a default string if no data
        text_data = "default";
        text_size = 7;
    }
    
    // Call functions in the specified order
    sqlite3_bind_int(stmt, 1, param1);
    sqlite3_bind_int64(stmt, 2, param2);
    sqlite3_bind_double(stmt, 3, param3);
    sqlite3_bind_text64(stmt, 4, text_data, (sqlite3_uint64)text_size, 
                       dummy_cleanup, SQLITE_UTF8);
    sqlite3_bind_int(stmt, 5, param5);
    
    // Execute the statement to trigger the bindings
    sqlite3_step(stmt);
    
    // Cleanup
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}