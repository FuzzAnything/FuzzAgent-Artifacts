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
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:119215:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize database and objects
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str_obj = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table and prepare a statement
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t1 (a, b, c);";
    sqlite3_exec(db, create_sql, dummy_callback, 0, NULL);
    
    // Prepare a simple SELECT statement
    const char *select_sql = "SELECT * FROM t1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step once to initialize result set
    sqlite3_step(stmt);
    
    // Use Data to create strings for various functions
    // Ensure we don't read beyond Size
    size_t half_size = Size / 2;
    const uint8_t *data1 = Data;
    size_t size1 = half_size;
    const uint8_t *data2 = Data + half_size;
    size_t size2 = Size - half_size;
    
    // Create null-terminated strings from input data
    char *str1 = (char *)malloc(size1 + 1);
    char *str2 = (char *)malloc(size2 + 1);
    
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    memcpy(str1, data1, size1);
    str1[size1] = '\0';
    memcpy(str2, data2, size2);
    str2[size2] = '\0';
    
    // 1. sqlite3_strlike
    unsigned int escape_char = (Size > 0) ? Data[0] : 0;
    sqlite3_strlike(str1, str2, escape_char);
    
    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    
    // 3. sqlite3_column_name
    const char *col_name1 = NULL;
    if (col_count > 0) {
        col_name1 = sqlite3_column_name(stmt, 0);
    }
    
    // 4. sqlite3_column_name (second call)
    const char *col_name2 = NULL;
    if (col_count > 1) {
        col_name2 = sqlite3_column_name(stmt, 1);
    } else if (col_count > 0) {
        col_name2 = sqlite3_column_name(stmt, 0);
    }
    
    // 5. sqlite3_stricmp
    sqlite3_stricmp(col_name1, col_name2);
    
    // 6. sqlite3_column_type
    int col_type = SQLITE_NULL;
    if (col_count > 0) {
        col_type = sqlite3_column_type(stmt, 0);
    }
    
    // 7. sqlite3_str_appendf
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        sqlite3_str_appendf(str_obj, "Test: %d, %s", col_type, str1);
    }
    
    // 8. sqlite3_str_appendf (second call)
    if (str_obj) {
        sqlite3_str_appendf(str_obj, " - More: %s", str2);
    }
    
    // 9. sqlite3_finalize
    sqlite3_finalize(stmt);
    
    // 10. sqlite3_str_appendf (third call)
    if (str_obj) {
        sqlite3_str_appendf(str_obj, " - Final");
    }
    
    // 11. sqlite3_str_finish
    char *finished_str = NULL;
    if (str_obj) {
        finished_str = sqlite3_str_finish(str_obj);
        if (finished_str) {
            sqlite3_free(finished_str);
        }
    }
    
    // Cleanup
    free(str1);
    free(str2);
    sqlite3_close(db);
    
    return 0;
}