// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *errmsg;
    int errcode;
    int extended_errcode;
    const char *errstr;
    
    // Open an in-memory database for fuzzing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // If we can't even open a database, just return
        return 0;
    }
    
    // Use fuzz data to decide whether to enable extended result codes
    if (Size > 0) {
        int enable = Data[0] & 1;
        sqlite3_extended_result_codes(db, enable);
    }
    
    // Try to cause some errors by executing invalid SQL
    if (Size > 1) {
        // Use part of fuzz data as SQL string (ensure null termination)
        size_t sql_len = Size - 1;
        if (sql_len > 1024) sql_len = 1024; // Limit SQL length
        char *sql = malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data + 1, sql_len);
            sql[sql_len] = '\0';
            
            // Execute potentially invalid SQL
            char *err = NULL;
            rc = sqlite3_exec(db, sql, NULL, NULL, &err);
            
            // Test error functions after SQL execution
            errcode = sqlite3_errcode(db);
            extended_errcode = sqlite3_extended_errcode(db);
            errmsg = sqlite3_errmsg(db);
            
            // Test sqlite3_errstr with various error codes
            if (Size > 2) {
                // Use fuzz data to generate error codes
                for (size_t i = 0; i < Size - 2 && i < 10; i++) {
                    int test_err = Data[i + 2] * 256 + Data[i + 1];
                    errstr = sqlite3_errstr(test_err);
                }
            }
            
            // Test sqlite3_set_errmsg if we have enough data
            if (Size > 20) {
                // Extract error code and message from fuzz data
                int custom_errcode = Data[10] * 256 + Data[11];
                size_t msg_len = Size - 20;
                if (msg_len > 100) msg_len = 100;
                char *custom_msg = malloc(msg_len + 1);
                if (custom_msg) {
                    memcpy(custom_msg, Data + 20, msg_len);
                    custom_msg[msg_len] = '\0';
                    sqlite3_set_errmsg(db, custom_errcode, custom_msg);
                    free(custom_msg);
                }
                
                // Check error functions after setting custom error
                errcode = sqlite3_errcode(db);
                extended_errcode = sqlite3_extended_errcode(db);
                errmsg = sqlite3_errmsg(db);
            }
            
            if (err) {
                sqlite3_free(err);
            }
            free(sql);
        }
    }
    
    // Test with NULL database handle (edge case)
    errmsg = sqlite3_errmsg(NULL);
    errcode = sqlite3_errcode(NULL);
    extended_errcode = sqlite3_extended_errcode(NULL);
    
    // Test sqlite3_set_errmsg with NULL db
    if (Size > 5) {
        sqlite3_set_errmsg(NULL, Data[3], (const char*)(Data + 4));
    }
    
    // Test sqlite3_extended_result_codes with NULL db
    if (Size > 0) {
        sqlite3_extended_result_codes(NULL, Data[0] & 1);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}