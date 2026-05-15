// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:119215:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_callback(void *arg, int argc, char **argv, char **col_names) {
    // Do nothing, return 0 to continue
    return 0;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize database and statement objects
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str_obj = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table and prepare a statement
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    sqlite3_exec(db, create_sql, dummy_callback, NULL, NULL);
    
    // Prepare a simple SELECT statement
    const char *select_sql = "SELECT * FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a dynamic string object
    str_obj = sqlite3_str_new(db);
    if (!str_obj) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare strings from fuzzer input
    char *pattern = NULL;
    char *text = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    
    if (Size >= 4) {
        // Split input into parts for different strings
        size_t quarter = Size / 4;
        
        pattern = (char *)malloc(quarter + 1);
        text = (char *)malloc(quarter + 1);
        str1 = (char *)malloc(quarter + 1);
        str2 = (char *)malloc(quarter + 1);
        
        if (pattern && text && str1 && str2) {
            memcpy(pattern, Data, quarter);
            pattern[quarter] = '\0';
            
            memcpy(text, Data + quarter, quarter);
            text[quarter] = '\0';
            
            memcpy(str1, Data + 2 * quarter, quarter);
            str1[quarter] = '\0';
            
            memcpy(str2, Data + 3 * quarter, quarter);
            str2[quarter] = '\0';
        }
    } else {
        // Use small dummy strings if input is too small
        pattern = strdup("test%");
        text = strdup("test123");
        str1 = strdup("Hello");
        str2 = strdup("hello");
    }
    
    // Call functions in the specified order
    
    // 1. sqlite3_strlike
    if (pattern && text) {
        unsigned int escape_char = (Size > 0) ? Data[0] : 0;
        sqlite3_strlike(pattern, text, escape_char);
    }
    
    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 3. sqlite3_column_name (first call)
    const char *col_name1 = NULL;
    if (col_count > 0) {
        col_name1 = sqlite3_column_name(stmt, 0);
    }
    
    // 4. sqlite3_column_name (second call)
    const char *col_name2 = NULL;
    if (col_count > 1) {
        col_name2 = sqlite3_column_name(stmt, 1);
    }
    
    // 5. sqlite3_stricmp
    if (str1 && str2) {
        sqlite3_stricmp(str1, str2);
    }
    
    // 6. sqlite3_column_type
    if (col_count > 0) {
        sqlite3_column_type(stmt, 0);
    }
    
    // 7. sqlite3_str_appendf (first call)
    if (str_obj && pattern) {
        sqlite3_str_appendf(str_obj, "Pattern: %s", pattern);
    }
    
    // 8. sqlite3_str_appendf (second call)
    if (str_obj && text) {
        sqlite3_str_appendf(str_obj, ", Text: %s", text);
    }
    
    // 9. sqlite3_finalize
    sqlite3_finalize(stmt);
    
    // 10. sqlite3_str_appendf (third call)
    if (str_obj) {
        sqlite3_str_appendf(str_obj, ", Size: %zu", Size);
    }
    
    // 11. sqlite3_str_finish
    char *result_str = sqlite3_str_finish(str_obj);
    if (result_str) {
        sqlite3_free(result_str);
    }
    
    // Cleanup
    free(pattern);
    free(text);
    free(str1);
    free(str2);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}