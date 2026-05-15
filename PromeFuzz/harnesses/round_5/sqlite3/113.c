// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
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

static void dummy_unlock_notify_callback(void **apArg, int nArg) {
    // Do nothing - this is just a dummy callback for testing
    (void)apArg;
    (void)nArg;
}

static void fuzz_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    // Use data to influence operations
    int enable_extended = (size > 0) ? (data[0] % 2) : 0;
    
    // Enable/disable extended result codes
    sqlite3_extended_result_codes(db, enable_extended);
    
    // Try to cause various error states
    if (size > 1) {
        // Execute some SQL that might fail
        char *err_msg = NULL;
        char sql[256];
        
        // Create a simple table if it doesn't exist
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER);", NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
        
        // Try various operations based on input data
        if (size > 2) {
            int op = data[1] % 4;
            switch (op) {
                case 0:
                    // Valid insert
                    sqlite3_exec(db, "INSERT INTO test VALUES(1);", NULL, NULL, &err_msg);
                    break;
                case 1:
                    // Invalid SQL
                    sqlite3_exec(db, "INVALID SQL STATEMENT;", NULL, NULL, &err_msg);
                    break;
                case 2:
                    // Another valid operation
                    sqlite3_exec(db, "SELECT * FROM test;", NULL, NULL, &err_msg);
                    break;
                case 3:
                    // Try to cause constraint error
                    sqlite3_exec(db, "INSERT INTO test VALUES(NULL);", NULL, NULL, &err_msg);
                    break;
            }
            if (err_msg) {
                sqlite3_free(err_msg);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Try to open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // If we can't open a database, we can still test some functions with NULL
        sqlite3_errcode(NULL);
        sqlite3_errmsg(NULL);
        sqlite3_extended_errcode(NULL);
        sqlite3_set_errmsg(NULL, SQLITE_ERROR, NULL);
        sqlite3_unlock_notify(NULL, NULL, NULL);
        return 0;
    }
    
    // Perform some operations to create error states
    fuzz_operations(db, data, size);
    
    // Test all target API functions
    int errcode = sqlite3_errcode(db);
    (void)errcode; // Use result to avoid unused variable warning
    
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; // Use result
    
    int extended_errcode = sqlite3_extended_errcode(db);
    (void)extended_errcode; // Use result
    
    // Test sqlite3_set_errmsg with various inputs
    if (size > 0) {
        int custom_errcode = SQLITE_ERROR + (data[0] % 100);
        const char *custom_msg = (size > 10 && data[1] % 2) ? "Custom error message" : NULL;
        sqlite3_set_errmsg(db, custom_errcode, custom_msg);
    }
    
    // Test sqlite3_unlock_notify
    if (size > 0) {
        int use_callback = (size > 1) ? (data[1] % 2) : 0;
        if (use_callback) {
            sqlite3_unlock_notify(db, dummy_unlock_notify_callback, NULL);
        } else {
            sqlite3_unlock_notify(db, NULL, NULL);
        }
    }
    
    // Test extended result codes again with different setting
    if (size > 2) {
        int new_setting = (data[2] % 2);
        sqlite3_extended_result_codes(db, new_setting);
        
        // Get error codes again after changing settings
        sqlite3_errcode(db);
        sqlite3_extended_errcode(db);
    }
    
    // Try to cause another error state
    if (size > 3) {
        char *err_msg = NULL;
        sqlite3_exec(db, "INVALID SQL TO CAUSE ERROR;", NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
        }
        
        // Test error functions again after new error
        sqlite3_errcode(db);
        sqlite3_errmsg(db);
        sqlite3_extended_errcode(db);
    }
    
    // Test with file-based database as well
    if (size > 4 && (data[3] % 2)) {
        sqlite3 *file_db = NULL;
        rc = sqlite3_open("./dummy_file", &file_db);
        if (rc == SQLITE_OK) {
            // Perform some operations
            sqlite3_exec(file_db, "CREATE TABLE t2(x INTEGER);", NULL, NULL, NULL);
            sqlite3_exec(file_db, "INSERT INTO t2 VALUES(42);", NULL, NULL, NULL);
            
            // Test APIs on file-based database
            sqlite3_errcode(file_db);
            sqlite3_errmsg(file_db);
            sqlite3_extended_errcode(file_db);
            
            sqlite3_close(file_db);
            remove("./dummy_file");
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}