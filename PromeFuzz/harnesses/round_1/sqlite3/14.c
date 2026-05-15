// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

// Helper function to create a va_list for sqlite3_vmprintf
static char* vmprintf_helper(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *formatted_str = NULL;
    const char *errmsg = NULL;
    int rc;
    
    // Limit input size to prevent excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    // Create a dummy table for testing
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER, data TEXT);", NULL, NULL, NULL);
    
    // 1. sqlite3_vmprintf - format a string using the fuzzer input
    if (Size > 0) {
        // Create a format string using the fuzzer data
        char *format = (char*)malloc(Size + 1);
        if (format) {
            memcpy(format, Data, Size);
            format[Size] = '\0';
            formatted_str = vmprintf_helper("%s", format);
            free(format);
        }
    } else {
        formatted_str = vmprintf_helper("SELECT * FROM test;");
    }
    
    // 2. sqlite3_prepare_v2 - prepare a statement
    const char *tail = NULL;
    if (formatted_str) {
        rc = sqlite3_prepare_v2(db, formatted_str, -1, &stmt, &tail);
    } else {
        rc = sqlite3_prepare_v2(db, "SELECT * FROM test;", -1, &stmt, &tail);
    }
    
    // 3. sqlite3_extended_errcode - get extended error code
    int errcode1 = sqlite3_extended_errcode(db);
    
    // 4. sqlite3_errmsg - get error message
    errmsg = sqlite3_errmsg(db);
    
    // 5. sqlite3_step - execute the prepared statement
    if (stmt) {
        int step_rc = sqlite3_step(stmt);
        
        // Only step through a few rows to avoid infinite loops
        int max_rows = 10;
        while (step_rc == SQLITE_ROW && max_rows-- > 0) {
            step_rc = sqlite3_step(stmt);
        }
    }
    
    // 6. sqlite3_reset - reset the statement
    if (stmt) {
        rc = sqlite3_reset(stmt);
    }
    
    // 7. sqlite3_extended_errcode - get extended error code again
    int errcode2 = sqlite3_extended_errcode(db);
    
    // 8. sqlite3_errmsg - get error message again
    errmsg = sqlite3_errmsg(db);
    
    // 9. sqlite3_free - free the formatted string
    if (formatted_str) {
        sqlite3_free(formatted_str);
    }
    
    // 10. sqlite3_finalize - finalize the statement
    if (stmt) {
        rc = sqlite3_finalize(stmt);
    }
    
    // Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}