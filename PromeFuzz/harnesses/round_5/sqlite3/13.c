// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static char* my_vmprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a null-terminated string from the fuzzer input
    char* input_str = (char*)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';
    
    // Open an in-memory database
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        free(input_str);
        return 0;
    }
    
    // 1. sqlite3_vmprintf - format a string using the input
    char* formatted_str = my_vmprintf("%s", input_str);
    
    // 2. sqlite3_prepare_v2 - prepare a statement
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    rc = sqlite3_prepare_v2(db, formatted_str, -1, &stmt, &tail);
    
    // 3. sqlite3_extended_errcode - get extended error code
    int extended_err = sqlite3_extended_errcode(db);
    
    // 4. sqlite3_errmsg - get error message
    const char* err_msg = sqlite3_errmsg(db);
    
    // 5. sqlite3_step - execute the prepared statement
    if (stmt != NULL) {
        int step_rc = sqlite3_step(stmt);
        
        // 6. sqlite3_reset - reset the statement
        int reset_rc = sqlite3_reset(stmt);
        
        // 7. sqlite3_extended_errcode - get extended error code again
        extended_err = sqlite3_extended_errcode(db);
        
        // 8. sqlite3_errmsg - get error message again
        err_msg = sqlite3_errmsg(db);
        
        // 9. sqlite3_free - free the formatted string
        if (formatted_str != NULL) {
            sqlite3_free(formatted_str);
        }
        
        // 10. sqlite3_finalize - finalize the statement
        int finalize_rc = sqlite3_finalize(stmt);
    } else {
        // If stmt is NULL, still free the formatted string
        if (formatted_str != NULL) {
            sqlite3_free(formatted_str);
        }
    }
    
    // Cleanup
    free(input_str);
    sqlite3_close(db);
    
    return 0;
}