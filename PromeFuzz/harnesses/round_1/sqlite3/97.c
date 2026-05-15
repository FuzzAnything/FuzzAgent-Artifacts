// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
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
#include <stdio.h>

static void dummy_busy_handler(void *data, int count) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set a busy handler to avoid potential hangs
    sqlite3_busy_handler(db, (int(*)(void*,int))dummy_busy_handler, NULL);
    
    // Create a dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    // Prepare a statement using the fuzzer input as SQL
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    
    // Use the fuzzer data as SQL string (ensure null termination)
    char *sql = NULL;
    if (Size > 0) {
        sql = (char*)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            // Try to prepare the statement
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
            
            free(sql);
        }
    } else {
        // Use a default SQL statement if no input
        rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, &tail);
    }
    
    if (rc == SQLITE_OK && stmt != NULL) {
        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        // Test sqlite3_bind_parameter_count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Test sqlite3_stmt_isexplain
        int explain_type = sqlite3_stmt_isexplain(stmt);
        
        // Test sqlite3_stmt_scanstatus_v2 with various parameters
        // We need to step the statement first to get scan status
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Try different scan status operations
            for (int i = 0; i < 3; i++) {
                sqlite3_int64 out_val = 0;
                sqlite3_stmt_scanstatus_v2(stmt, 0, i, 0, &out_val);
            }
        }
        
        // Test sqlite3_reset
        rc = sqlite3_reset(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    remove("./dummy_file");
    
    return 0;
}