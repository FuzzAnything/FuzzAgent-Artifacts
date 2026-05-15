// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_unlock_notify at sqlite3.c:176214:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void unlock_notify_callback(void **apArg, int nArg) {
    // Empty callback for testing
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    const char *errmsg;
    int errcode;
    int extended_errcode;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Open a second database for unlock_notify testing
    rc = sqlite3_open("./dummy_file", &db2);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use the fuzzer input to influence behavior
    if (Size > 0) {
        // Enable/disable extended result codes based on first byte
        int enable_extended = Data[0] & 1;
        sqlite3_extended_result_codes(db, enable_extended);
        
        // Try to set error message using input data
        if (Size > 10) {
            // Create a null-terminated string from input for error message
            char *err_str = malloc(Size + 1);
            if (err_str) {
                memcpy(err_str, Data, Size);
                err_str[Size] = '\0';
                
                // Use a portion of input as error code
                int custom_errcode = (Size > 1) ? Data[1] : SQLITE_ERROR;
                sqlite3_set_errmsg(db, custom_errcode, err_str);
                free(err_str);
            }
        }
        
        // Perform some operations that might generate errors
        if (Size > 2) {
            // Create a simple SQL statement from input
            char *sql = malloc(Size + 1);
            if (sql) {
                memcpy(sql, Data, Size);
                sql[Size] = '\0';
                
                // Try to execute the SQL
                char *zErrMsg = NULL;
                rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
                if (zErrMsg) {
                    sqlite3_free(zErrMsg);
                }
                free(sql);
            }
        }
    }
    
    // Test sqlite3_errcode
    errcode = sqlite3_errcode(db);
    
    // Test sqlite3_extended_errcode
    extended_errcode = sqlite3_extended_errcode(db);
    
    // Test sqlite3_errmsg
    errmsg = sqlite3_errmsg(db);
    
    // Test sqlite3_unlock_notify
    // First try to create a lock situation
    if (db2) {
        // Start a transaction to potentially lock
        sqlite3_exec(db2, "BEGIN IMMEDIATE", NULL, NULL, NULL);
        
        // Try unlock_notify with various parameters based on input
        if (Size > 3) {
            int use_callback = Data[2] & 1;
            if (use_callback) {
                sqlite3_unlock_notify(db, unlock_notify_callback, NULL);
            } else {
                sqlite3_unlock_notify(db, NULL, NULL);
            }
        }
        
        // End transaction
        sqlite3_exec(db2, "COMMIT", NULL, NULL, NULL);
    }
    
    // Test with NULL database handle
    if (Size > 4 && (Data[3] & 1)) {
        sqlite3_errcode(NULL);
        sqlite3_errmsg(NULL);
        sqlite3_extended_errcode(NULL);
        sqlite3_set_errmsg(NULL, SQLITE_ERROR, "test");
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_close(db2);
    
    return 0;
}