// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Initialize database and statement
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a prepared statement with enough parameters
    const char *sql = "SELECT ?, ?, ?, ?, ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract values from fuzzer input with bounds checking
    int param1 = 0;
    sqlite3_int64 param2 = 0;
    double param3 = 0.0;
    int param5 = 0;
    
    if (Size >= sizeof(int)) {
        memcpy(&param1, Data, sizeof(int));
        Data += sizeof(int);
        Size -= sizeof(int);
    }
    
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&param2, Data, sizeof(sqlite3_int64));
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
    }
    
    if (Size >= sizeof(double)) {
        memcpy(&param3, Data, sizeof(double));
        Data += sizeof(double);
        Size -= sizeof(double);
    }
    
    // For text parameter: use remaining data as string
    const char *text_data = (const char*)Data;
    sqlite3_uint64 text_len = Size > 0 ? Size : 0;
    
    // Extract last int parameter if enough data remains
    if (Size >= sizeof(int)) {
        memcpy(&param5, Data + Size - sizeof(int), sizeof(int));
    }
    
    // Bind parameters in specified order
    sqlite3_bind_int(stmt, 1, param1);
    sqlite3_bind_int64(stmt, 2, param2);
    sqlite3_bind_double(stmt, 3, param3);
    
    // Bind text with SQLITE_UTF8 encoding and SQLITE_STATIC destructor
    sqlite3_bind_text64(stmt, 4, text_data, text_len, 
                       SQLITE_STATIC, SQLITE_UTF8);
    
    sqlite3_bind_int(stmt, 5, param5);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}