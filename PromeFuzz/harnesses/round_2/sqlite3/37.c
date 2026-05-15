// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static sqlite3* create_test_database() {
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return db;
}

static sqlite3_stmt* create_test_statement(sqlite3* db) {
    sqlite3_stmt* stmt = NULL;
    const char* sql = "CREATE TABLE test (id INTEGER, value TEXT); INSERT INTO test VALUES (?1, ?2); SELECT ?3, ?4, ?5 FROM test;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return stmt;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Initialize database and statement
    sqlite3* db = create_test_database();
    if (db == NULL) {
        return 0;
    }
    
    sqlite3_stmt* stmt = create_test_statement(db);
    if (stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Extract values from fuzzer input
    int int_val1 = 0;
    sqlite3_int64 int64_val = 0;
    double double_val = 0.0;
    const char* text_val = NULL;
    sqlite3_uint64 text_len = 0;
    int int_val2 = 0;
    
    // Use fuzzer data to populate values
    if (Size >= sizeof(int)) {
        memcpy(&int_val1, Data, sizeof(int));
    }
    if (Size >= sizeof(int) + sizeof(sqlite3_int64)) {
        memcpy(&int64_val, Data + sizeof(int), sizeof(sqlite3_int64));
    }
    if (Size >= sizeof(int) + sizeof(sqlite3_int64) + sizeof(double)) {
        memcpy(&double_val, Data + sizeof(int) + sizeof(sqlite3_int64), sizeof(double));
    }
    if (Size > sizeof(int) + sizeof(sqlite3_int64) + sizeof(double)) {
        size_t text_offset = sizeof(int) + sizeof(sqlite3_int64) + sizeof(double);
        text_val = (const char*)(Data + text_offset);
        text_len = (sqlite3_uint64)(Size - text_offset);
    }
    if (Size >= sizeof(int) + sizeof(sqlite3_int64) + sizeof(double) + sizeof(int)) {
        memcpy(&int_val2, Data + sizeof(int) + sizeof(sqlite3_int64) + sizeof(double), sizeof(int));
    }
    
    // Execute functions in specified order
    sqlite3_bind_int(stmt, 1, int_val1);
    sqlite3_bind_int64(stmt, 2, int64_val);
    sqlite3_bind_double(stmt, 3, double_val);
    
    if (text_val != NULL && text_len > 0) {
        sqlite3_bind_text64(stmt, 4, text_val, text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
    } else {
        sqlite3_bind_text64(stmt, 4, "", 0, SQLITE_STATIC, SQLITE_UTF8);
    }
    
    sqlite3_bind_int(stmt, 5, int_val2);
    
    // Step through the statement to execute it
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Continue stepping
    }
    
    // Reset and re-bind with different parameter indices to explore more states
    sqlite3_reset(stmt);
    sqlite3_bind_int(stmt, 2, int_val2);
    sqlite3_bind_int64(stmt, 3, int64_val);
    sqlite3_bind_double(stmt, 4, double_val);
    
    if (text_val != NULL && text_len > 0) {
        sqlite3_bind_text64(stmt, 1, text_val, text_len > 100 ? 100 : text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
    }
    
    sqlite3_bind_int(stmt, 5, int_val1);
    
    // Step again
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Continue stepping
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}