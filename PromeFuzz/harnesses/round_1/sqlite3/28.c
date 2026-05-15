// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_callback(void* not_used, int argc, char** argv, char** col_name) {
    return 0;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    
    // Initialize database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a table with a BLOB column for sqlite3_blob_open
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    if (sqlite3_exec(db, create_sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // Insert a row with BLOB data
    const char *insert_sql = "INSERT INTO test (data) VALUES (?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 100 ? 100 : Size), SQLITE_STATIC);
        } else {
            sqlite3_bind_zeroblob(stmt, 1, 100);
        }
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    
    // 1. sqlite3_finalize
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // 2. sqlite3_exec
    const char *select_sql = "SELECT * FROM test;";
    sqlite3_exec(db, select_sql, dummy_callback, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // 3. sqlite3_mprintf (first call)
    if (Size > 0) {
        str1 = sqlite3_mprintf("Test string with data: %.*s", 
                               (int)(Size > 50 ? 50 : Size), Data);
    } else {
        str1 = sqlite3_mprintf("Test string without data");
    }
    
    // 4. sqlite3_mprintf (second call)
    str2 = sqlite3_mprintf("Another string: %d", 42);
    
    // 5. sqlite3_free (first string)
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }
    
    // 6. sqlite3_free (second string)
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }
    
    // 7. sqlite3_blob_open
    sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    
cleanup:
    if (blob) {
        sqlite3_blob_close(blob);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    if (str1) {
        sqlite3_free(str1);
    }
    if (str2) {
        sqlite3_free(str2);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}