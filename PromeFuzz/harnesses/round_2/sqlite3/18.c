// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:110746:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_busy_handler(void *ptr, int count) {
    return 0;
}

static int dummy_authorize(void *userdata, int action, const char *arg1,
                          const char *arg2, const char *arg3, const char *arg4) {
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_text = NULL;
    sqlite3_str *str_obj = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set some callbacks to avoid crashes
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    sqlite3_set_authorizer(db, dummy_authorize, NULL);
    
    // Create a dummy table and prepare a statement
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t1(a, b, c);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    
    // Use fuzzer input to create a parameterized SQL statement
    char *sql = NULL;
    if (Size > 0 && Data) {
        // Copy up to 1024 bytes for SQL, ensuring null termination
        size_t sql_size = Size > 1024 ? 1024 : Size;
        sql = (char*)malloc(sql_size + 1);
        if (sql) {
            memcpy(sql, Data, sql_size);
            sql[sql_size] = '\0';
            
            // Try to prepare the statement
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
                // If preparation fails, use a default statement
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
        free(sql);
    }
    
    // If no statement was prepared, use a default one
    if (!stmt) {
        const char *default_sql = "SELECT ?, :param, @var FROM t1 WHERE a = ?1 AND b = ?2;";
        sqlite3_prepare_v2(db, default_sql, -1, &stmt, NULL);
    }
    
    if (stmt) {
        // 1. sqlite3_db_handle
        sqlite3 *db_handle = sqlite3_db_handle(stmt);
        
        // 2. sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        // 3. sqlite3_bind_parameter_count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 4. sqlite3_sql
        sql_text = sqlite3_sql(stmt);
        
        // 5. sqlite3_str_new - use the database handle from step 1
        str_obj = sqlite3_str_new(db_handle);
        if (str_obj) {
            // Do some operations on the string object
            sqlite3_str_append(str_obj, "test", 4);
            sqlite3_str_appendf(str_obj, " %d", param_count);
            
            // Free the string object
            char *result = sqlite3_str_finish(str_obj);
            if (result) {
                sqlite3_free(result);
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Close the database
    sqlite3_close(db);
    
    return 0;
}