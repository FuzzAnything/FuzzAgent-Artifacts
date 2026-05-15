// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    char *err_msg = NULL;
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, value TEXT);", callback, 0, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with parameters
    const char *sql = "INSERT INTO test VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input for binding values
    // We'll extract values from the fuzzer input in a simple way
    // First 4 bytes: int for first bind
    // Next 8 bytes: int64 for second bind
    // Next 8 bytes: double for third bind
    // Remaining bytes: text for fourth bind
    // Last 4 bytes: int for fifth bind
    
    int int_val1 = 0;
    sqlite3_int64 int64_val = 0;
    double double_val = 0.0;
    int int_val2 = 0;
    
    if (Size >= 4) {
        memcpy(&int_val1, Data, 4);
    }
    if (Size >= 12) {
        memcpy(&int64_val, Data + 4, 8);
    }
    if (Size >= 20) {
        memcpy(&double_val, Data + 12, 8);
    }
    
    // Calculate text parameters
    size_t text_offset = 20;
    size_t text_size = 0;
    const char *text_data = NULL;
    
    if (Size > text_offset) {
        text_size = Size - text_offset;
        if (text_size >= 4) {
            // Reserve last 4 bytes for the second int
            text_size -= 4;
            if (text_size > 0) {
                text_data = (const char *)(Data + text_offset);
            }
        } else {
            text_size = 0;
        }
    }
    
    if (Size >= text_offset + text_size + 4) {
        memcpy(&int_val2, Data + text_offset + text_size, 4);
    }
    
    // Bind parameters in the specified order
    // First sqlite3_bind_int
    sqlite3_bind_int(stmt, 1, int_val1);
    
    // Second sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 2, int64_val);
    
    // Third sqlite3_bind_double
    sqlite3_bind_double(stmt, 1, double_val);
    
    // Fourth sqlite3_bind_text64
    if (text_data && text_size > 0) {
        sqlite3_bind_text64(stmt, 2, text_data, text_size, 
                           SQLITE_TRANSIENT, SQLITE_UTF8);
    } else {
        sqlite3_bind_text64(stmt, 2, "fuzz", 4, SQLITE_TRANSIENT, SQLITE_UTF8);
    }
    
    // Fifth sqlite3_bind_int
    sqlite3_bind_int(stmt, 1, int_val2);
    
    // Execute the statement
    sqlite3_step(stmt);
    
    // Reset for potential reuse
    sqlite3_reset(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}