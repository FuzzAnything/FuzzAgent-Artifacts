// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Initialize database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy SQL statement using fuzzer input
    char *sql = NULL;
    if (Size > 0) {
        sql = (char*)malloc(Size + 1);
        if (!sql) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql, Data, Size);
        sql[Size] = '\0';
    } else {
        sql = strdup("SELECT ?1, ?2, ?3");
    }
    
    // Prepare a statement
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    // Call target functions in specified order
    if (rc == SQLITE_OK && stmt != NULL) {
        // 1. sqlite3_db_handle
        sqlite3 *db_handle = sqlite3_db_handle(stmt);
        
        // 2. sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        // 3. sqlite3_bind_parameter_count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 4. sqlite3_sql
        const char *original_sql = sqlite3_sql(stmt);
        
        // 5. sqlite3_str_new
        sqlite3_str *str_obj = sqlite3_str_new(db_handle);
        if (str_obj != NULL) {
            // Clean up the string object
            char *str_result = sqlite3_str_finish(str_obj);
            sqlite3_free(str_result);
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(sql);
    sqlite3_close(db);
    
    return 0;
}