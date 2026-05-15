// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

static char* helper_vmprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *formatted_str = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare format string from fuzzer input
    // Ensure null termination for safety
    char *format = (char *)malloc(Size + 1);
    if (!format) {
        sqlite3_close(db);
        return 0;
    }
    if (Size > 0) {
        memcpy(format, Data, Size);
    }
    format[Size] = '\0';
    
    // Use sqlite3_vmprintf with the format string via helper function
    formatted_str = helper_vmprintf(format);
    
    free(format);
    
    // Call sqlite3_prepare_v2 with the formatted string (or NULL if formatting failed)
    const char *sql = formatted_str ? formatted_str : "";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    // Get extended error code
    int extended_err = sqlite3_extended_errcode(db);
    
    // Get error message
    const char *err_msg = sqlite3_errmsg(db);
    
    // Execute the statement if preparation succeeded
    if (rc == SQLITE_OK && stmt) {
        int step_rc = sqlite3_step(stmt);
        
        // Reset the statement
        int reset_rc = sqlite3_reset(stmt);
        
        // Get extended error code again after reset
        extended_err = sqlite3_extended_errcode(db);
        
        // Get error message again
        err_msg = sqlite3_errmsg(db);
    }
    
    // Free the formatted string
    if (formatted_str) {
        sqlite3_free(formatted_str);
    }
    
    // Finalize the statement
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}