// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_errstr(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int err_code;
    memcpy(&err_code, data, sizeof(int));
    
    const char *err_str = sqlite3_errstr(err_code);
    (void)err_str; // Use result to avoid compiler warnings
}

static void test_error_functions(sqlite3 *db, const uint8_t *data, size_t size) {
    if (!db) return;
    
    // Test sqlite3_errcode
    int errcode = sqlite3_errcode(db);
    (void)errcode;
    
    // Test sqlite3_errmsg
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg;
    
    // Test sqlite3_extended_errcode
    int extended_errcode = sqlite3_extended_errcode(db);
    (void)extended_errcode;
    
    // Test sqlite3_extended_result_codes
    int onoff = (size > 0) ? (data[0] & 1) : 0;
    sqlite3_extended_result_codes(db, onoff);
    
    // Test sqlite3_set_errmsg if we have enough data
    if (size >= sizeof(int) + 1) {
        int set_errcode;
        memcpy(&set_errcode, data, sizeof(int));
        
        // Create error message from remaining data
        size_t msg_len = (size - sizeof(int) > 256) ? 256 : size - sizeof(int);
        char *err_msg = (char*)malloc(msg_len + 1);
        if (err_msg) {
            memcpy(err_msg, data + sizeof(int), msg_len);
            err_msg[msg_len] = '\0';
            
            sqlite3_set_errmsg(db, set_errcode, err_msg);
            free(err_msg);
        }
    }
}

static sqlite3* create_test_db(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Try to open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    
    // Try to execute some SQL to potentially trigger errors
    if (size > 0) {
        // Create a simple table
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
        
        // Try to insert some data using the fuzzer input
        if (size < 1024) { // Limit size for SQL statement
            char sql[2048];
            snprintf(sql, sizeof(sql), "INSERT INTO test(data) VALUES('%.*s');", 
                    (int)(size > 256 ? 256 : size), (const char*)data);
            sqlite3_exec(db, sql, NULL, NULL, NULL);
        }
        
        // Try a potentially invalid SQL statement
        if (size > 10) {
            char invalid_sql[256];
            snprintf(invalid_sql, sizeof(invalid_sql), "INVALID SQL %.*s", 
                    (int)(size > 100 ? 100 : size), (const char*)data);
            sqlite3_exec(db, invalid_sql, NULL, NULL, NULL);
        }
    }
    
    return db;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    // Test sqlite3_errstr with random error codes
    test_errstr(data, size);
    
    // Create a test database connection
    sqlite3 *db = create_test_db(data, size);
    
    if (db) {
        // Test all error-related functions with the database
        test_error_functions(db, data, size);
        
        // Clean up
        sqlite3_close(db);
    }
    
    // Also test with NULL database handle
    test_error_functions(NULL, data, size);
    
    return 0;
}