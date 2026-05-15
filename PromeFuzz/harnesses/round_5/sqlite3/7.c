// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:119215:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
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
#include <stdio.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for any potential file operations
    dummy_file_write(Data, Size);
    
    // Parse input for various parameters
    // We'll use the first part for pattern/string matching,
    // and the rest for other parameters.
    if (Size < 4) return 0;
    
    // Split input: first null-terminated string for pattern,
    // second for string, then bytes for escape char and column index
    size_t offset = 0;
    const char *pattern = (const char *)Data;
    while (offset < Size && Data[offset] != 0) offset++;
    if (offset >= Size) return 0;
    offset++; // skip null terminator
    
    const char *str = (const char *)(Data + offset);
    size_t str_start = offset;
    while (offset < Size && Data[offset] != 0) offset++;
    if (offset >= Size) return 0;
    offset++; // skip null terminator
    
    // Get escape character and column index
    if (offset + 2 > Size) return 0;
    unsigned int escape = Data[offset];
    int col_index = Data[offset + 1] & 0x7F; // Keep it positive
    
    // 1. sqlite3_strlike
    int like_result = sqlite3_strlike(pattern, str, escape);
    (void)like_result;
    
    // Create a minimal database and prepared statement for column functions
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and prepare a SELECT statement
    const char *create_sql = "CREATE TABLE t1(a, b, c);";
    sqlite3_exec(db, create_sql, 0, 0, 0);
    const char *insert_sql = "INSERT INTO t1 VALUES(1, 'test', 3.14);";
    sqlite3_exec(db, insert_sql, 0, 0, 0);
    const char *select_sql = "SELECT * FROM t1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, 0);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step once to get data
    sqlite3_step(stmt);
    
    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;
    
    // Adjust column index to be valid
    int adj_col_index = col_index;
    if (col_count > 0) {
        adj_col_index = col_index % col_count;
    } else {
        adj_col_index = 0;
    }
    
    // 3. sqlite3_column_name (first call)
    const char *col_name1 = sqlite3_column_name(stmt, adj_col_index);
    (void)col_name1;
    
    // 4. sqlite3_column_name (second call)
    const char *col_name2 = sqlite3_column_name(stmt, adj_col_index);
    (void)col_name2;
    
    // 5. sqlite3_stricmp
    int cmp_result = sqlite3_stricmp(pattern, str);
    (void)cmp_result;
    
    // 6. sqlite3_column_type
    int col_type = sqlite3_column_type(stmt, adj_col_index);
    (void)col_type;
    
    // Create a string accumulator for appendf functions
    sqlite3_str *str_acc = sqlite3_str_new(db);
    if (!str_acc) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // 7. sqlite3_str_appendf (first call)
    // Use a simple format string with data from input
    const char *fmt1 = "Test %.*s";
    int fmt1_len = (Size - str_start) % 64; // Limit length
    if (fmt1_len > 0) {
        sqlite3_str_appendf(str_acc, fmt1, fmt1_len, str);
    } else {
        sqlite3_str_appendf(str_acc, "Test string");
    }
    
    // 8. sqlite3_str_appendf (second call)
    sqlite3_str_appendf(str_acc, " Value: %d", like_result);
    
    // 9. sqlite3_finalize
    int finalize_result = sqlite3_finalize(stmt);
    (void)finalize_result;
    
    // 10. sqlite3_str_appendf (third call)
    sqlite3_str_appendf(str_acc, " Final: %d", col_count);
    
    // 11. sqlite3_str_finish
    char *finished_str = sqlite3_str_finish(str_acc);
    if (finished_str) {
        sqlite3_free(finished_str);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}