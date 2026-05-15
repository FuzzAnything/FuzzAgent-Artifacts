// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open an in-memory database for fuzzing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Ensure the input is null-terminated for safety
    char *sql = (char *)malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';
    
    // 1. sqlite3_prepare_v2
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    // 2. sqlite3_errmsg
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg; // Use the result to avoid unused variable warning
    
    if (stmt) {
        // 3. sqlite3_step (first call)
        rc = sqlite3_step(stmt);
        
        // 4. sqlite3_step (second call)
        rc = sqlite3_step(stmt);
        
        // 5. sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        
        if (col_count > 0) {
            // 6. sqlite3_column_type
            int col_type = sqlite3_column_type(stmt, 0);
            
            // 7. sqlite3_column_name
            const char *col_name = sqlite3_column_name(stmt, 0);
            (void)col_name;
            
            // 8. sqlite3_column_text (first call)
            const unsigned char *text1 = sqlite3_column_text(stmt, 0);
            (void)text1;
            
            // 9. sqlite3_column_text (second call)
            const unsigned char *text2 = sqlite3_column_text(stmt, 0);
            (void)text2;
            
            // 10. sqlite3_column_text (third call)
            const unsigned char *text3 = sqlite3_column_text(stmt, 0);
            (void)text3;
            
            // 11. sqlite3_column_bytes
            int bytes = sqlite3_column_bytes(stmt, 0);
            (void)bytes;
        }
        
        // Reset the statement to allow finalization
        sqlite3_reset(stmt);
    }
    
    // 12. sqlite3_finalize
    rc = sqlite3_finalize(stmt);
    
    // Cleanup
    free(sql);
    sqlite3_close(db);
    
    return 0;
}