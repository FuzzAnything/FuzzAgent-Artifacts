// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:173725:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:173622:16 in sqlite3.h
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

static void dummy_callback(void *p) {
    (void)p;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *errmsg;
    const char *errstr;
    int errcode;
    int extended_errcode;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Use the first byte to control extended result codes (0 or 1)
    int enable_extended = (Size > 0) ? (Data[0] & 1) : 0;
    sqlite3_extended_result_codes(db, enable_extended);
    
    // Use second byte to control whether we set custom error messages
    int set_custom_error = (Size > 1) ? (Data[1] & 1) : 0;
    
    // Perform some operations that might generate errors
    // Execute a malformed SQL statement to generate an error
    char *sql_err = NULL;
    if (Size > 2) {
        // Use up to 256 bytes for SQL, but ensure null termination
        size_t sql_len = (Size - 2 > 256) ? 256 : Size - 2;
        sql_err = (char *)malloc(sql_len + 1);
        if (sql_err) {
            memcpy(sql_err, Data + 2, sql_len);
            sql_err[sql_len] = '\0';
            sqlite3_exec(db, sql_err, NULL, NULL, NULL);
            free(sql_err);
        }
    }
    
    // Get error information using various functions
    errcode = sqlite3_errcode(db);
    extended_errcode = sqlite3_extended_errcode(db);
    errmsg = sqlite3_errmsg(db);
    
    // Use sqlite3_errstr with various error codes
    // Try with the current error code and some common codes
    errstr = sqlite3_errstr(errcode);
    errstr = sqlite3_errstr(SQLITE_OK);
    errstr = sqlite3_errstr(SQLITE_ERROR);
    errstr = sqlite3_errstr(SQLITE_INTERNAL);
    errstr = sqlite3_errstr(SQLITE_PERM);
    errstr = sqlite3_errstr(SQLITE_ABORT);
    
    // Try some random error codes based on input data
    if (Size > 10) {
        for (size_t i = 3; i < Size && i < 20; i++) {
            int random_code = Data[i];
            errstr = sqlite3_errstr(random_code);
        }
    }
    
    // Set custom error message if requested
    if (set_custom_error) {
        // Use a portion of the input data as error message
        size_t msg_len = (Size > 50) ? 50 : (Size > 0 ? Size : 1);
        char *custom_msg = (char *)malloc(msg_len + 1);
        if (custom_msg) {
            if (Size > 0) {
                size_t copy_len = (msg_len < Size) ? msg_len : Size;
                memcpy(custom_msg, Data, copy_len);
                custom_msg[copy_len] = '\0';
            } else {
                custom_msg[0] = '\0';
            }
            
            // Try setting with various error codes
            sqlite3_set_errmsg(db, errcode, custom_msg);
            sqlite3_set_errmsg(db, SQLITE_ERROR, custom_msg);
            sqlite3_set_errmsg(db, SQLITE_NOMEM, NULL); // Test with NULL message
            
            free(custom_msg);
        }
    }
    
    // Toggle extended result codes again
    sqlite3_extended_result_codes(db, !enable_extended);
    
    // Get error information again after toggling
    errcode = sqlite3_errcode(db);
    extended_errcode = sqlite3_extended_errcode(db);
    errmsg = sqlite3_errmsg(db);
    
    // Test with NULL database handle for some functions
    // (These should handle NULL gracefully)
    sqlite3_extended_result_codes(NULL, 1);
    (void)sqlite3_extended_errcode(NULL);
    (void)sqlite3_errmsg(NULL);
    sqlite3_set_errmsg(NULL, SQLITE_ERROR, "test");
    (void)sqlite3_errcode(NULL);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}