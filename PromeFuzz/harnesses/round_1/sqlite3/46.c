// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_db_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    // Use fuzz data to influence database operations
    if (size > 0) {
        // Try to execute some SQL based on fuzz input
        char sql[256];
        size_t sql_len = size < sizeof(sql) - 1 ? size : sizeof(sql) - 1;
        memcpy(sql, data, sql_len);
        sql[sql_len] = '\0';
        
        // Execute SQL to potentially generate errors
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }
}

static void fuzz_error_functions(sqlite3 *db, const uint8_t *data, size_t size) {
    // Test sqlite3_extended_result_codes with random on/off
    if (size > 0) {
        int onoff = data[0] & 1;
        sqlite3_extended_result_codes(db, onoff);
    }
    
    // Test sqlite3_errcode
    int errcode = sqlite3_errcode(db);
    
    // Test sqlite3_extended_errcode
    int extended_errcode = sqlite3_extended_errcode(db);
    
    // Test sqlite3_errmsg
    const char *errmsg = sqlite3_errmsg(db);
    
    // Test sqlite3_errstr with various error codes
    if (size > 1) {
        // Use fuzz data to select error codes to look up
        for (size_t i = 0; i < size && i < 10; i++) {
            int test_errcode = data[i] * 100; // Scale to get different error codes
            const char *errstr = sqlite3_errstr(test_errcode);
        }
    }
    
    // Test sqlite3_set_errmsg with fuzz data
    if (size > 20) {
        // Create error message from fuzz data
        char custom_err[256];
        size_t err_len = size - 10 < sizeof(custom_err) - 1 ? size - 10 : sizeof(custom_err) - 1;
        memcpy(custom_err, data + 10, err_len);
        custom_err[err_len] = '\0';
        
        // Use fuzz data to determine error code
        int custom_errcode = (data[0] << 8) | data[1];
        
        sqlite3_set_errmsg(db, custom_errcode, custom_err);
        
        // Verify the custom error was set
        const char *new_errmsg = sqlite3_errmsg(db);
        int new_errcode = sqlite3_errcode(db);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Perform some database operations that might generate errors
    fuzz_db_operations(db, Data, Size);
    
    // Test all the error-related functions
    fuzz_error_functions(db, Data, Size);
    
    // Test with NULL database handle for some functions
    if (Size > 0 && (Data[0] & 1)) {
        sqlite3 *null_db = NULL;
        sqlite3_errcode(null_db);
        sqlite3_extended_errcode(null_db);
        sqlite3_errmsg(null_db);
        sqlite3_extended_result_codes(null_db, 1);
        sqlite3_set_errmsg(null_db, SQLITE_ERROR, "test");
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}