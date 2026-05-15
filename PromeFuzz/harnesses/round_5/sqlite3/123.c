// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *errmsg = NULL;
    int rc;
    
    // Use first byte to determine flags for sqlite3_open_v2
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        if (Data[0] & 0x01) flags |= SQLITE_OPEN_MEMORY;
        if (Data[0] & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
        if (Data[0] & 0x04) flags |= SQLITE_OPEN_FULLMUTEX;
        if (Data[0] & 0x08) flags |= SQLITE_OPEN_PRIVATECACHE;
        if (Data[0] & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
    }
    
    // Open database
    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    if (rc != SQLITE_OK) {
        // If opening fails, try with :memory: as fallback
        rc = sqlite3_open_v2(":memory:", &db, flags, NULL);
        if (rc != SQLITE_OK) {
            // Test sqlite3_errmsg with NULL/invalid db
            const char *err = sqlite3_errmsg(NULL);
            (void)err;
            
            // Test sqlite3_db_handle with NULL
            sqlite3 *db_from_null = sqlite3_db_handle(NULL);
            (void)db_from_null;
            
            // Test sqlite3_interrupt with NULL
            sqlite3_interrupt(NULL);
            return 0;
        }
    }
    
    // Create a table if database opened successfully
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER, name TEXT);", 
                      dummy_callback, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Prepare a statement using fuzzer data (ensure null termination)
    char *sql = NULL;
    if (Size > 0) {
        sql = (char*)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            // Try to prepare statement
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
            
            // Test sqlite3_db_handle
            if (stmt) {
                sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
                (void)db_from_stmt;
                
                // Finalize the statement
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            
            // Test sqlite3_exec with fuzzer data
            rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            
            free(sql);
        }
    } else {
        // Test with empty SQL
        rc = sqlite3_prepare_v2(db, "", -1, &stmt, &tail);
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        rc = sqlite3_exec(db, "", dummy_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    // Test sqlite3_errmsg
    const char *error_msg = sqlite3_errmsg(db);
    (void)error_msg;
    
    // Test sqlite3_interrupt
    sqlite3_interrupt(db);
    
    // Try to prepare and execute some fixed SQL statements
    const char *fixed_sqls[] = {
        "INSERT INTO t1 VALUES(1, 'test');",
        "SELECT * FROM t1;",
        "BEGIN;",
        "COMMIT;",
        "PRAGMA journal_mode=WAL;"
    };
    
    for (size_t i = 0; i < sizeof(fixed_sqls)/sizeof(fixed_sqls[0]); i++) {
        rc = sqlite3_prepare_v2(db, fixed_sqls[i], -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            // Test sqlite3_db_handle again
            sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
            (void)db_from_stmt;
            
            // Step through the statement
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                // Do nothing with row data
            }
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        
        rc = sqlite3_exec(db, fixed_sqls[i], dummy_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        
        // Call interrupt occasionally
        if (i % 2 == 0) {
            sqlite3_interrupt(db);
        }
    }
    
    // Test with NULL callback
    rc = sqlite3_exec(db, "SELECT 1;", NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}