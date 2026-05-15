// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *dummy_file = "./dummy_file";
    
    // Use first byte to determine behavior variations
    uint8_t mode = Size > 0 ? Data[0] : 0;
    
    // Write data to dummy file for sqlite3_open_v2
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 1) {
            fwrite(Data + 1, 1, Size - 1, fp);
        }
        fclose(fp);
    }
    
    // Test sqlite3_open_v2 with various flags
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (mode & 0x01) flags |= SQLITE_OPEN_MEMORY;
    if (mode & 0x02) flags |= SQLITE_OPEN_NOMUTEX;
    
    int rc = sqlite3_open_v2(dummy_file, &db, flags, NULL);
    
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_errmsg
        const char *error_msg = sqlite3_errmsg(db);
        (void)error_msg; // Use result to avoid unused variable warning
        
        // Test sqlite3_exec with various SQL commands
        char *sql = NULL;
        if (Size > 10) {
            sql = (char*)malloc(Size + 1);
            if (sql) {
                memcpy(sql, Data, Size);
                sql[Size] = '\0';
                
                // Try different SQL patterns based on input
                if (mode & 0x04) {
                    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b);", 
                                     dummy_callback, NULL, &errmsg);
                } else if (mode & 0x08) {
                    rc = sqlite3_exec(db, "SELECT * FROM sqlite_master;", 
                                     dummy_callback, NULL, &errmsg);
                } else {
                    rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
                }
                
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
                free(sql);
            }
        }
        
        // Test prepared statement and sqlite3_db_handle
        rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            // Test sqlite3_db_handle
            sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
            (void)db_from_stmt; // Use result
            
            // Test sqlite3_interrupt
            if (mode & 0x10) {
                sqlite3_interrupt(db);
            }
            
            sqlite3_finalize(stmt);
        }
        
        // Test sqlite3_close
        if (mode & 0x20) {
            // Try to close with possible unfinalized statements
            rc = sqlite3_close(db);
            if (rc == SQLITE_BUSY) {
                // Database wasn't closed, but that's OK for fuzzing
                sqlite3_close_v2(db);
            }
        } else {
            sqlite3_close_v2(db);
        }
    } else {
        // Open failed, but we can still test some APIs
        if (db) {
            // Test sqlite3_errmsg on potentially invalid db
            const char *error_msg = sqlite3_errmsg(db);
            (void)error_msg;
            
            // Test sqlite3_close on potentially invalid db
            sqlite3_close(db);
        }
        
        // Test sqlite3_db_handle with NULL
        sqlite3 *null_db = sqlite3_db_handle(NULL);
        (void)null_db;
        
        // Test sqlite3_errmsg with NULL
        const char *null_err = sqlite3_errmsg(NULL);
        (void)null_err;
        
        // Test sqlite3_close with NULL (should be harmless)
        sqlite3_close(NULL);
        
        // Test sqlite3_interrupt with NULL
        sqlite3_interrupt(NULL);
    }
    
    // Clean up dummy file
    remove(dummy_file);
    
    return 0;
}