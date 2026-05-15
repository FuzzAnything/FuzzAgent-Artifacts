// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:119215:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:22331:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_bindings(sqlite3_stmt* stmt) {
    // Helper to bind some dummy values to avoid SQLITE_MISUSE
    sqlite3_bind_int(stmt, 1, 42);
    sqlite3_bind_text(stmt, 2, "test", 4, SQLITE_STATIC);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;  // Need minimum data
    
    // Initialize database and statement
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    sqlite3_str* str_obj = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and prepare a statement
    const char* create_sql = "CREATE TABLE IF NOT EXISTS t1(a INTEGER, b TEXT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'hello');", NULL, NULL, NULL);
    
    const char* select_sql = "SELECT * FROM t1 WHERE a > ? AND b LIKE ?;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind dummy values
    dummy_bindings(stmt);
    
    // Execute step to make statement ready for column functions
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_step(stmt);
    
    // Extract data from fuzzer input for various parameters
    size_t offset = 0;
    
    // For sqlite3_strlike: need pattern, string, and escape char
    const char* pattern = (const char*)Data;
    offset = strnlen((const char*)Data, Size) + 1;
    if (offset >= Size) offset = Size - 1;
    
    const char* match_str = (const char*)Data + offset;
    size_t str_len = strnlen(match_str, Size - offset);
    offset += str_len + 1;
    if (offset >= Size) offset = Size - 1;
    
    unsigned int escape_char = 0;
    if (offset < Size) {
        escape_char = Data[offset];
        offset++;
    }
    
    // 1. sqlite3_strlike
    sqlite3_strlike(pattern, match_str, escape_char);
    
    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 3. & 4. sqlite3_column_name (called twice)
    const char* col_name1 = NULL;
    const char* col_name2 = NULL;
    if (col_count > 0) {
        int col_idx = 0;
        if (offset < Size) {
            col_idx = Data[offset] % col_count;
            offset++;
        }
        col_name1 = sqlite3_column_name(stmt, col_idx);
        col_name2 = sqlite3_column_name(stmt, col_idx % col_count);
    }
    
    // 5. sqlite3_stricmp
    const char* str1 = "test_string";
    const char* str2 = "TEST_STRING";
    if (col_name1 && col_name2) {
        sqlite3_stricmp(col_name1, col_name2);
    } else {
        sqlite3_stricmp(str1, str2);
    }
    
    // 6. sqlite3_column_type
    if (col_count > 0) {
        int col_idx = 0;
        if (offset < Size) {
            col_idx = Data[offset] % col_count;
            offset++;
        }
        sqlite3_column_type(stmt, col_idx);
    }
    
    // Create a string accumulator object
    str_obj = sqlite3_str_new(db);
    if (!str_obj) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare format strings from fuzzer data
    char fmt1[100] = "%s";
    char fmt2[100] = "Value: %d";
    char fmt3[100] = "Text: %s";
    
    if (offset < Size && Size - offset >= 10) {
        size_t fmt_len = (Data[offset] % 50) + 1;
        if (fmt_len > 99) fmt_len = 99;
        if (offset + fmt_len < Size) {
            memcpy(fmt1, Data + offset, fmt_len);
            fmt1[fmt_len] = '\0';
        }
        offset += fmt_len;
    }
    
    // 7. sqlite3_str_appendf (first call)
    sqlite3_str_appendf(str_obj, fmt1, "test");
    
    // 8. sqlite3_str_appendf (second call)
    int int_val = 42;
    if (offset < Size) {
        int_val = Data[offset];
        offset++;
    }
    sqlite3_str_appendf(str_obj, fmt2, int_val);
    
    // 9. sqlite3_finalize
    sqlite3_finalize(stmt);
    stmt = NULL;  // Set to NULL after finalize
    
    // 10. sqlite3_str_appendf (third call)
    const char* append_text = "append";
    if (offset < Size && Size - offset > 0) {
        size_t text_len = (Data[offset] % 50) + 1;
        if (text_len > Size - offset) text_len = Size - offset;
        if (text_len > 0) {
            char* temp_buf = (char*)malloc(text_len + 1);
            if (temp_buf) {
                memcpy(temp_buf, Data + offset, text_len);
                temp_buf[text_len] = '\0';
                sqlite3_str_appendf(str_obj, fmt3, temp_buf);
                free(temp_buf);
            }
        }
    } else {
        sqlite3_str_appendf(str_obj, fmt3, append_text);
    }
    
    // 11. sqlite3_str_finish
    char* finished_str = sqlite3_str_finish(str_obj);
    if (finished_str) {
        sqlite3_free(finished_str);
    }
    
    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}