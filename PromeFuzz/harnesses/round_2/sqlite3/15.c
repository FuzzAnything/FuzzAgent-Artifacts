// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdarg.h>

static void dummy_file_init(void) {
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fputs("CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);\n", fp);
        fputs("INSERT INTO test(name) VALUES('test1');\n", fp);
        fputs("INSERT INTO test(name) VALUES('test2');\n", fp);
        fclose(fp);
    }
}

static char* helper_vmprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = sqlite3_vmprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Initialize dummy file
    dummy_file_init();
    
    // Open database
    sqlite3* db = NULL;
    if (sqlite3_open("./dummy_file", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create SQL string using sqlite3_vmprintf
    char* sql = NULL;
    if (Size > 0) {
        // Use first byte as format specifier choice
        uint8_t format_choice = Data[0] % 3;
        const char* formats[] = {"%s", "SELECT %s", "INSERT INTO test(name) VALUES(%s)"};
        
        // Create a null-terminated string from the data (skip first byte)
        size_t str_size = (Size - 1) > 0 ? (Size - 1) : 1;
        char* input_str = (char*)malloc(str_size + 1);
        if (input_str) {
            size_t copy_size = (Size - 1) > 0 ? (Size - 1) : 0;
            if (copy_size > 0) {
                memcpy(input_str, Data + 1, copy_size);
            }
            input_str[copy_size] = '\0';
            
            sql = helper_vmprintf(formats[format_choice], input_str);
            free(input_str);
        }
    } else {
        // Default SQL if no data
        sql = helper_vmprintf("SELECT * FROM test");
    }
    
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare statement
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    int prep_result = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    // Get error info after prepare
    int errcode1 = sqlite3_extended_errcode(db);
    const char* errmsg1 = sqlite3_errmsg(db);
    
    // Step through results if prepare succeeded
    if (prep_result == SQLITE_OK && stmt) {
        int step_result;
        do {
            step_result = sqlite3_step(stmt);
        } while (step_result == SQLITE_ROW);
        
        // Reset statement
        int reset_result = sqlite3_reset(stmt);
        
        // Get error info after reset
        int errcode2 = sqlite3_extended_errcode(db);
        const char* errmsg2 = sqlite3_errmsg(db);
        
        // Use error codes and messages to avoid optimization removal
        if (errcode1 != 0 || errcode2 != 0) {
            // Force compiler to consider error messages used
            volatile const char* dummy1 = errmsg1;
            volatile const char* dummy2 = errmsg2;
            (void)dummy1;
            (void)dummy2;
        }
    }
    
    // Clean up
    sqlite3_free(sql);
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    
    return 0;
}