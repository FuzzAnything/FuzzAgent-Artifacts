// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

static void dummy_error_callback(void *pArg, int errCode, const char *zMsg) {
    // Dummy callback to trigger error conditions
    (void)pArg;
    (void)errCode;
    (void)zMsg;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Limit size to avoid excessive memory usage
    if (Size > 1024) {
        Size = 1024;
    }
    
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK && db == NULL) {
        // Try to open with dummy file as fallback
        rc = sqlite3_open("./dummy_file", &db);
    }
    
    // Call functions in specified order
    
    // 1. sqlite3_errcode
    int errcode = sqlite3_errcode(db);
    (void)errcode; // Use result to avoid unused variable warning
    
    // 2. sqlite3_mprintf - use fuzzer data for format string
    char *formatted = NULL;
    if (Size > 0) {
        // Create a null-terminated copy of the input for format string
        char *format_str = (char*)malloc(Size + 1);
        if (format_str) {
            memcpy(format_str, Data, Size);
            format_str[Size] = '\0';
            
            // Try different mprintf variations with the fuzzer data
            formatted = sqlite3_mprintf("%s", format_str);
            if (formatted) {
                sqlite3_free(formatted);
                formatted = NULL;
            }
            
            // Try with additional arguments
            int int_arg = (Size > 0) ? Data[0] : 0;
            double double_arg = (Size > 1) ? (double)Data[1] : 0.0;
            formatted = sqlite3_mprintf("Int: %d, Double: %f, Str: %s", 
                                        int_arg, double_arg, format_str);
            
            free(format_str);
        }
    } else {
        // Empty input case
        formatted = sqlite3_mprintf("Empty input");
    }
    
    // Free the formatted string if allocated
    if (formatted) {
        sqlite3_free(formatted);
    }
    
    // 3. sqlite3_errmsg
    const char *errmsg = sqlite3_errmsg(db);
    (void)errmsg; // Use result to avoid unused variable warning
    
    // 4. sqlite3_extended_errcode
    int extended_errcode = sqlite3_extended_errcode(db);
    (void)extended_errcode; // Use result to avoid unused variable warning
    
    // Try to trigger different error states by performing operations
    if (db) {
        // Try to execute invalid SQL to trigger errors
        char *err_msg = NULL;
        rc = sqlite3_exec(db, "INVALID SQL STATEMENT", NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
        }
        
        // Call the functions again with potentially different state
        errcode = sqlite3_errcode(db);
        errmsg = sqlite3_errmsg(db);
        extended_errcode = sqlite3_extended_errcode(db);
        
        // Try another mprintf with error information
        formatted = sqlite3_mprintf("Error: %d, Msg: %s", errcode, errmsg);
        if (formatted) {
            sqlite3_free(formatted);
        }
        
        // Close the database
        sqlite3_close(db);
    }
    
    // Test with NULL database handle
    errcode = sqlite3_errcode(NULL);
    errmsg = sqlite3_errmsg(NULL);
    extended_errcode = sqlite3_extended_errcode(NULL);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}