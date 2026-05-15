// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(void* data, const char* sql) {
    // Empty callback to satisfy API requirements
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_text = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table for testing
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, value INTEGER);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO test VALUES (1, 100), (2, 200), (3, 300);", NULL, NULL, NULL);
    
    // Prepare a statement using fuzzer data if possible, otherwise use default
    const char *default_sql = "SELECT id, value FROM test WHERE id > 0";
    char *fuzzer_sql = NULL;
    
    if (Size > 0) {
        fuzzer_sql = (char*)malloc(Size + 1);
        if (fuzzer_sql) {
            memcpy(fuzzer_sql, Data, Size);
            fuzzer_sql[Size] = '\0';
            rc = sqlite3_prepare_v2(db, fuzzer_sql, -1, &stmt, NULL);
            free(fuzzer_sql);
        }
    }
    
    // If preparation with fuzzer data failed, use default SQL
    if (!stmt) {
        sqlite3_prepare_v2(db, default_sql, -1, &stmt, NULL);
    }
    
    if (stmt) {
        // First sequence: sqlite3_sql -> sqlite3_step -> sqlite3_sql -> sqlite3_column_int -> sqlite3_column_int -> sqlite3_finalize
        sql_text = sqlite3_sql(stmt);
        (void)sql_text; // Avoid unused variable warning
        
        rc = sqlite3_step(stmt);
        
        sql_text = sqlite3_sql(stmt);
        (void)sql_text;
        
        // Only call column_int if we have a row
        if (rc == SQLITE_ROW) {
            int col1 = sqlite3_column_int(stmt, 0);
            int col2 = sqlite3_column_int(stmt, 1);
            (void)col1;
            (void)col2;
        }
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Call sqlite3_db_release_memory
    sqlite3_db_release_memory(db);
    
    // Prepare another statement for the second sequence
    if (Size > 0) {
        fuzzer_sql = (char*)malloc(Size + 1);
        if (fuzzer_sql) {
            memcpy(fuzzer_sql, Data, Size);
            fuzzer_sql[Size] = '\0';
            rc = sqlite3_prepare_v2(db, fuzzer_sql, -1, &stmt, NULL);
            free(fuzzer_sql);
        }
    }
    
    // If preparation with fuzzer data failed, use default SQL again
    if (!stmt) {
        sqlite3_prepare_v2(db, default_sql, -1, &stmt, NULL);
    }
    
    if (stmt) {
        // Second sequence: sqlite3_sql -> sqlite3_step -> sqlite3_sql -> sqlite3_finalize
        sql_text = sqlite3_sql(stmt);
        (void)sql_text;
        
        rc = sqlite3_step(stmt);
        
        sql_text = sqlite3_sql(stmt);
        (void)sql_text;
        
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}