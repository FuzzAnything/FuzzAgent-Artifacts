// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

static char* my_vmprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for database
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open database
    sqlite3* db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Call sqlite3_vmprintf
    char* formatted_str = my_vmprintf("%.*s", (int)Size, Data);
    
    // Call sqlite3_prepare_v2
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    rc = sqlite3_prepare_v2(db, formatted_str, -1, &stmt, &tail);
    
    // First call to sqlite3_extended_errcode
    int errcode1 = sqlite3_extended_errcode(db);
    
    // First call to sqlite3_errmsg
    const char* errmsg1 = sqlite3_errmsg(db);
    
    // Call sqlite3_step if prepare succeeded
    if (rc == SQLITE_OK && stmt) {
        int step_rc = sqlite3_step(stmt);
        // Keep stepping until done or error
        while (step_rc == SQLITE_ROW) {
            step_rc = sqlite3_step(stmt);
        }
    }
    
    // Call sqlite3_reset if we have a statement
    if (stmt) {
        int reset_rc = sqlite3_reset(stmt);
    }
    
    // Second call to sqlite3_extended_errcode
    int errcode2 = sqlite3_extended_errcode(db);
    
    // Second call to sqlite3_errmsg
    const char* errmsg2 = sqlite3_errmsg(db);
    
    // Call sqlite3_free on the formatted string
    sqlite3_free(formatted_str);
    
    // Call sqlite3_finalize
    if (stmt) {
        int finalize_rc = sqlite3_finalize(stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}