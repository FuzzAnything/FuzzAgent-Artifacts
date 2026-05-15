// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:173641:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

static void fuzz_sqlite3_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    if (db == NULL) return;
    
    // Use first byte to determine which operations to perform
    uint8_t op_selector = size > 0 ? data[0] : 0;
    
    // Test sqlite3_extended_result_codes with random on/off value
    int enable_extended = (op_selector & 0x01) ? 1 : 0;
    sqlite3_extended_result_codes(db, enable_extended);
    
    // Test sqlite3_errcode and sqlite3_extended_errcode
    int errcode = sqlite3_errcode(db);
    int extended_errcode = sqlite3_extended_errcode(db);
    
    // Test sqlite3_errmsg
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; // Use variable to avoid unused warning
    
    // Test sqlite3_error_offset
    int error_offset = sqlite3_error_offset(db);
    (void)error_offset;
    
    // Test sqlite3_set_errmsg if we have enough data
    if (size > 10) {
        // Create a null-terminated error message from fuzz data
        size_t msg_len = size < 100 ? size : 100;
        char *custom_msg = (char *)malloc(msg_len + 1);
        if (custom_msg) {
            memcpy(custom_msg, data, msg_len);
            custom_msg[msg_len] = '\0';
            
            // Use second byte as error code (mask to valid range)
            int custom_errcode = (data[1] % 100) + 1; // Avoid SQLITE_OK(0)
            sqlite3_set_errmsg(db, custom_errcode, custom_msg);
            free(custom_msg);
        }
    }
    
    // Try to cause some errors to test error reporting
    if (size > 2 && (op_selector & 0x02)) {
        // Attempt invalid SQL to generate error
        char *invalid_sql = "INVALID SQL STATEMENT;";
        sqlite3_exec(db, invalid_sql, NULL, NULL, NULL);
        
        // Test error functions again after error
        (void)sqlite3_errcode(db);
        (void)sqlite3_extended_errcode(db);
        (void)sqlite3_errmsg(db);
        (void)sqlite3_error_offset(db);
    }
    
    // Toggle extended result codes again
    sqlite3_extended_result_codes(db, !enable_extended);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Perform some basic operations to initialize state
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", NULL, NULL, NULL);
    
    // Execute the fuzzing operations
    fuzz_sqlite3_operations(db, data, size);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}