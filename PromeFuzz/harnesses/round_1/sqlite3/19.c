// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
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
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table and prepare a statement
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Prepare a statement using fuzzer data if possible
    if (Size > 0) {
        // Use the fuzzer data as SQL if it's null-terminated
        char *sql_copy = malloc(Size + 1);
        if (sql_copy) {
            memcpy(sql_copy, Data, Size);
            sql_copy[Size] = '\0';
            
            // Try to prepare the statement
            if (sqlite3_prepare_v2(db, sql_copy, -1, &stmt, NULL) != SQLITE_OK) {
                // If preparation fails, use a default statement
                sqlite3_finalize(stmt);
                stmt = NULL;
                sqlite3_prepare_v2(db, "SELECT * FROM test", -1, &stmt, NULL);
            }
            free(sql_copy);
        }
    }
    
    // If we don't have a statement yet, create a default one
    if (!stmt) {
        sqlite3_prepare_v2(db, "SELECT * FROM test", -1, &stmt, NULL);
    }
    
    // Now call the target functions in the specified order
    if (stmt) {
        // 1. sqlite3_db_handle
        sqlite3 *db_handle = sqlite3_db_handle(stmt);
        
        // 2. sqlite3_column_count
        int column_count = sqlite3_column_count(stmt);
        
        // 3. sqlite3_bind_parameter_count
        int bind_param_count = sqlite3_bind_parameter_count(stmt);
        
        // 4. sqlite3_sql
        const char *sql_text = sqlite3_sql(stmt);
        
        // 5. sqlite3_str_new
        sqlite3_str *str_obj = sqlite3_str_new(db_handle);
        
        // Clean up the string object
        if (str_obj) {
            char *str_result = sqlite3_str_finish(str_obj);
            if (str_result) {
                sqlite3_free(str_result);
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}