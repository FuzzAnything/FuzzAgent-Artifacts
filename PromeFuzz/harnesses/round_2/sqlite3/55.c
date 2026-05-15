// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
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

static void fuzz_sqlite3_errstr(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int err_code;
    memcpy(&err_code, data, sizeof(int));
    
    const char *err_str = sqlite3_errstr(err_code);
    (void)err_str; // Use result to avoid dead code elimination
}

static void fuzz_sqlite3_errmsg(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    // Try to open an in-memory database
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Try to cause an error by executing invalid SQL
        if (size > 0) {
            // Use up to 256 bytes for SQL command
            size_t sql_len = size > 256 ? 256 : size;
            char *sql = malloc(sql_len + 1);
            if (sql) {
                memcpy(sql, data, sql_len);
                sql[sql_len] = '\0';
                
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                free(sql);
            }
        }
        
        const char *err_msg = sqlite3_errmsg(db);
        (void)err_msg;
        
        // Test with NULL database handle
        const char *null_err = sqlite3_errmsg(NULL);
        (void)null_err;
        
        sqlite3_close(db);
    }
}

static void fuzz_sqlite3_extended_errcode(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Try to cause an error
        if (size > 0) {
            size_t sql_len = size > 128 ? 128 : size;
            char *sql = malloc(sql_len + 1);
            if (sql) {
                memcpy(sql, data, sql_len);
                sql[sql_len] = '\0';
                
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                free(sql);
            }
        }
        
        int extended_err = sqlite3_extended_errcode(db);
        (void)extended_err;
        
        // Test with NULL handle
        int null_extended_err = sqlite3_extended_errcode(NULL);
        (void)null_extended_err;
        
        sqlite3_close(db);
    }
}

static void fuzz_sqlite3_extended_result_codes(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Use first byte to determine on/off state
        int onoff = 0;
        if (size > 0) {
            onoff = data[0] & 1; // 0 or 1
        }
        
        int result = sqlite3_extended_result_codes(db, onoff);
        (void)result;
        
        // Test with different onoff values
        sqlite3_extended_result_codes(db, 0);
        sqlite3_extended_result_codes(db, 1);
        
        // Test with NULL handle
        sqlite3_extended_result_codes(NULL, onoff);
        
        sqlite3_close(db);
    }
}

static void fuzz_sqlite3_set_errmsg(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Extract error code from data if available
        int errcode = SQLITE_ERROR;
        if (size >= sizeof(int)) {
            memcpy(&errcode, data, sizeof(int));
        }
        
        // Extract error message from data if available
        const char *zErrMsg = NULL;
        if (size > sizeof(int)) {
            size_t msg_len = size - sizeof(int);
            if (msg_len > 256) msg_len = 256;
            
            char *msg = malloc(msg_len + 1);
            if (msg) {
                memcpy(msg, data + sizeof(int), msg_len);
                msg[msg_len] = '\0';
                zErrMsg = msg;
                
                int result = sqlite3_set_errmsg(db, errcode, zErrMsg);
                (void)result;
                
                free(msg);
            }
        } else {
            // Test with NULL error message
            sqlite3_set_errmsg(db, errcode, NULL);
        }
        
        // Test with NULL database handle
        sqlite3_set_errmsg(NULL, errcode, "test error");
        
        sqlite3_close(db);
    }
}

static void fuzz_sqlite3_errcode(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Try to cause different error conditions
        if (size > 0) {
            size_t sql_len = size > 128 ? 128 : size;
            char *sql = malloc(sql_len + 1);
            if (sql) {
                memcpy(sql, data, sql_len);
                sql[sql_len] = '\0';
                
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                free(sql);
            }
        }
        
        int errcode = sqlite3_errcode(db);
        (void)errcode;
        
        // Test with NULL handle
        int null_errcode = sqlite3_errcode(NULL);
        (void)null_errcode;
        
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Use first byte to determine which functions to call
    uint8_t selector = data[0];
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size > 1 ? size - 1 : 0;
    
    // Call different functions based on selector bits
    if (selector & 0x01) {
        fuzz_sqlite3_errstr(remaining_data, remaining_size);
    }
    
    if (selector & 0x02) {
        fuzz_sqlite3_errmsg(remaining_data, remaining_size);
    }
    
    if (selector & 0x04) {
        fuzz_sqlite3_extended_errcode(remaining_data, remaining_size);
    }
    
    if (selector & 0x08) {
        fuzz_sqlite3_extended_result_codes(remaining_data, remaining_size);
    }
    
    if (selector & 0x10) {
        fuzz_sqlite3_set_errmsg(remaining_data, remaining_size);
    }
    
    if (selector & 0x20) {
        fuzz_sqlite3_errcode(remaining_data, remaining_size);
    }
    
    // If no bits set, call all functions
    if (selector == 0) {
        fuzz_sqlite3_errstr(data, size);
        fuzz_sqlite3_errmsg(data, size);
        fuzz_sqlite3_extended_errcode(data, size);
        fuzz_sqlite3_extended_result_codes(data, size);
        fuzz_sqlite3_set_errmsg(data, size);
        fuzz_sqlite3_errcode(data, size);
    }
    
    return 0;
}