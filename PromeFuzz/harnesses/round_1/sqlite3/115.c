// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *ptr) {
    /* Do nothing - used for sqlite3_bind_text16 */
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dummy_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, val INTEGER, txt TEXT, blob BLOB); INSERT INTO test VALUES (?, ?, ?, ?); SELECT id, val FROM test;";
    int rc;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Prepare statement */
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Use fuzzer data to generate parameters */
    if (Size >= sizeof(sqlite3_int64) + sizeof(int) + sizeof(int)) {
        sqlite3_int64 int64_val = 0;
        int int_val = 0;
        int zeroblob_size = 0;
        const void *text_data = NULL;
        int text_len = 0;
        
        /* Extract values from fuzzer input */
        memcpy(&int64_val, Data, sizeof(sqlite3_int64));
        memcpy(&int_val, Data + sizeof(sqlite3_int64), sizeof(int));
        memcpy(&zeroblob_size, Data + sizeof(sqlite3_int64) + sizeof(int), sizeof(int));
        
        /* Ensure zeroblob size is non-negative and reasonable */
        if (zeroblob_size < 0) zeroblob_size = 0;
        if (zeroblob_size > 1000000) zeroblob_size = 1000000;
        
        /* Prepare text data if available */
        if (Size > sizeof(sqlite3_int64) + sizeof(int) + sizeof(int)) {
            text_data = Data + sizeof(sqlite3_int64) + sizeof(int) + sizeof(int);
            text_len = (int)(Size - (sizeof(sqlite3_int64) + sizeof(int) + sizeof(int)));
            if (text_len > 1000000) text_len = 1000000;
        }
        
        /* Bind parameters to the prepared statement */
        sqlite3_bind_int64(stmt, 1, int64_val);
        sqlite3_bind_int(stmt, 2, int_val);
        
        if (text_data && text_len > 0) {
            sqlite3_bind_text16(stmt, 3, text_data, text_len, dummy_destructor);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        
        sqlite3_bind_zeroblob(stmt, 4, zeroblob_size);
        
        /* Execute the INSERT statement */
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
            /* Reset statement for next execution */
            sqlite3_reset(stmt);
            
            /* Test clear_bindings */
            sqlite3_clear_bindings(stmt);
            
            /* Re-bind with different values to explore more states */
            sqlite3_bind_int64(stmt, 1, int64_val ^ 0xFFFFFFFF);
            sqlite3_bind_int(stmt, 2, int_val ^ 0xFFFF);
            
            /* Execute again */
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            
            /* Now execute the SELECT part (second statement in the SQL string) */
            /* sqlite3_prepare_v2 prepared the entire multi-statement string */
            /* We need to get to the SELECT statement */
            sqlite3_finalize(stmt);
            
            /* Prepare just the SELECT statement */
            const char *select_sql = "SELECT id, val FROM test;";
            rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK && stmt) {
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    /* Test column_int functions */
                    int col0 = sqlite3_column_int(stmt, 0);
                    int col1 = sqlite3_column_int(stmt, 1);
                    (void)col0; /* Suppress unused variable warning */
                    (void)col1; /* Suppress unused variable warning */
                }
            }
        }
    }
    
    /* Cleanup */
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}