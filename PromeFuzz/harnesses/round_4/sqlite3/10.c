// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
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
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:119215:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Initialize database and statement objects
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str_obj = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a dummy table and insert some data to have columns
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, name TEXT, value REAL);"
        "INSERT OR IGNORE INTO test_table VALUES (1, 'test1', 3.14);"
        "INSERT OR IGNORE INTO test_table VALUES (2, 'test2', 2.71);";
    
    char *err_msg = NULL;
    if (sqlite3_exec(db, create_table_sql, dummy_callback, NULL, &err_msg) != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a simple SELECT statement
    const char *select_sql = "SELECT * FROM test_table";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step once to get column data available
    sqlite3_step(stmt);
    
    // Create strings from fuzzer input for various functions
    // Use the first half and second half of input for different strings
    size_t half_size = Size / 2;
    const char *str1 = (const char*)Data;
    const char *str2 = (const char*)(Data + half_size);
    
    // Ensure we don't read out of bounds
    if (half_size == 0) {
        str1 = "";
        str2 = "";
    }
    
    // 1. sqlite3_strlike
    unsigned int escape_char = 0;
    if (Size > 0) {
        escape_char = Data[0];
    }
    sqlite3_strlike(str1, str2, escape_char);
    
    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 3. sqlite3_column_name (first call)
    const char *col_name1 = NULL;
    if (col_count > 0) {
        int col_idx1 = 0;
        if (Size > 0) {
            col_idx1 = Data[0] % col_count;
        }
        col_name1 = sqlite3_column_name(stmt, col_idx1);
    }
    
    // 4. sqlite3_column_name (second call)
    const char *col_name2 = NULL;
    if (col_count > 0) {
        int col_idx2 = 0;
        if (Size > 1) {
            col_idx2 = Data[1] % col_count;
        }
        col_name2 = sqlite3_column_name(stmt, col_idx2);
    }
    
    // 5. sqlite3_stricmp
    sqlite3_stricmp(col_name1 ? col_name1 : "", col_name2 ? col_name2 : "");
    
    // 6. sqlite3_column_type
    if (col_count > 0) {
        int col_idx = 0;
        if (Size > 2) {
            col_idx = Data[2] % col_count;
        }
        sqlite3_column_type(stmt, col_idx);
    }
    
    // 7. sqlite3_str_appendf (first call)
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // Create format string from fuzzer data
        char format_buf[256];
        size_t fmt_len = Size < 255 ? Size : 255;
        if (fmt_len > 0) {
            memcpy(format_buf, Data, fmt_len);
            format_buf[fmt_len] = '\0';
            // Replace any null bytes in the middle to keep it as a string
            for (size_t i = 0; i < fmt_len; i++) {
                if (format_buf[i] == '\0') format_buf[i] = 'A';
            }
        } else {
            strcpy(format_buf, "%s");
        }
        
        sqlite3_str_appendf(str_obj, format_buf, str1);
        
        // 8. sqlite3_str_appendf (second call)
        sqlite3_str_appendf(str_obj, ":%s", str2);
    }
    
    // 9. sqlite3_finalize
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // 10. sqlite3_str_appendf (third call - after finalize)
    if (str_obj) {
        sqlite3_str_appendf(str_obj, "-%zu", Size);
        
        // 11. sqlite3_str_finish
        char *result = sqlite3_str_finish(str_obj);
        if (result) {
            sqlite3_free(result);
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}