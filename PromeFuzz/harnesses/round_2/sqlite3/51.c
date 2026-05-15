// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Test sqlite3_errstr with various error codes derived from fuzzer input
    if (Size >= sizeof(int)) {
        int err_code;
        memcpy(&err_code, Data, sizeof(int));
        const char *err_str = sqlite3_errstr(err_code);
        (void)err_str;
    }

    // Create and open an in-memory database for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        // Test sqlite3_errmsg with valid database handle
        const char *err_msg = sqlite3_errmsg(db);
        (void)err_msg;
        
        // Test sqlite3_errcode with valid database handle
        int errcode = sqlite3_errcode(db);
        (void)errcode;
        
        // Test sqlite3_extended_errcode with valid database handle
        int extended_errcode = sqlite3_extended_errcode(db);
        (void)extended_errcode;
        
        // Test sqlite3_extended_result_codes - enable/disable based on input
        if (Size > 0) {
            int onoff = Data[0] & 1;
            sqlite3_extended_result_codes(db, onoff);
        }
        
        // Test sqlite3_set_errmsg with various inputs
        if (Size > sizeof(int)) {
            int custom_errcode;
            memcpy(&custom_errcode, Data, sizeof(int));
            
            size_t msg_len = Size - sizeof(int);
            if (msg_len > 0) {
                char *custom_msg = (char *)malloc(msg_len + 1);
                if (custom_msg) {
                    memcpy(custom_msg, Data + sizeof(int), msg_len);
                    custom_msg[msg_len] = '\0';
                    
                    int set_result = sqlite3_set_errmsg(db, custom_errcode, custom_msg);
                    (void)set_result;
                    
                    free(custom_msg);
                }
            }
        }
        
        // Test with NULL database handle for some functions
        const char *null_errmsg = sqlite3_errmsg(NULL);
        (void)null_errmsg;
        
        int null_errcode = sqlite3_errcode(NULL);
        (void)null_errcode;
        
        int null_extended_errcode = sqlite3_extended_errcode(NULL);
        (void)null_extended_errcode;
        
        // Test sqlite3_set_errmsg with NULL db
        if (Size > 0) {
            int set_null_result = sqlite3_set_errmsg(NULL, SQLITE_OK, "test");
            (void)set_null_result;
        }
        
        sqlite3_close(db);
    }
    
    // Test with file-based database
    if (Size > 0) {
        sqlite3 *file_db = NULL;
        rc = sqlite3_open("./dummy_file", &file_db);
        
        if (rc == SQLITE_OK && file_db != NULL) {
            char *zErrMsg = 0;
            rc = sqlite3_exec(file_db, "INVALID SQL STATEMENT", 0, 0, &zErrMsg);
            
            const char *exec_errmsg = sqlite3_errmsg(file_db);
            (void)exec_errmsg;
            
            int exec_errcode = sqlite3_errcode(file_db);
            (void)exec_errcode;
            
            int exec_extended_errcode = sqlite3_extended_errcode(file_db);
            (void)exec_extended_errcode;
            
            if (zErrMsg) {
                sqlite3_free(zErrMsg);
            }
            
            sqlite3_close(file_db);
        }
    }
    
    return 0;
}