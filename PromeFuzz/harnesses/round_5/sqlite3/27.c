// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    char *errmsg = NULL;
    sqlite3_blob *pBlob = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a table with a BLOB column for sqlite3_blob_open
    const char *create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test_table (id, data) VALUES (1, x'01020304');";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // 1. sqlite3_finalize
    // Prepare a statement first, then finalize it
    const char *select_sql = "SELECT * FROM test_table;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &pStmt, NULL) == SQLITE_OK) {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    // Also test with NULL
    sqlite3_finalize(NULL);
    
    // 2. sqlite3_exec
    // Use fuzz data as SQL string (with null termination)
    char *sql = NULL;
    if (Size > 0) {
        sql = (char*)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
            free(sql);
        }
    } else {
        sqlite3_exec(db, "", dummy_callback, NULL, &errmsg);
    }
    
    // 3. sqlite3_mprintf (first call)
    char *mprintf_str1 = NULL;
    if (Size > 0) {
        // Use first byte as format specifier
        char format[32];
        snprintf(format, sizeof(format), "%%%us", (unsigned)Size);
        mprintf_str1 = sqlite3_mprintf(format, Data);
    } else {
        mprintf_str1 = sqlite3_mprintf("empty input");
    }
    
    // 4. sqlite3_mprintf (second call)
    char *mprintf_str2 = NULL;
    if (Size > 4) {
        // Use different format
        mprintf_str2 = sqlite3_mprintf("Data: %.*s", (int)(Size > 100 ? 100 : Size), Data);
    } else {
        mprintf_str2 = sqlite3_mprintf("small input");
    }
    
    // 5. sqlite3_free (first call)
    if (mprintf_str1) {
        sqlite3_free(mprintf_str1);
    }
    
    // 6. sqlite3_free (second call)
    if (mprintf_str2) {
        sqlite3_free(mprintf_str2);
    }
    
    // Also free error message from sqlite3_exec if any
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }
    
    // 7. sqlite3_blob_open
    // Try to open a blob handle to the inserted row
    sqlite3_blob_open(db, "main", "test_table", "data", 1, 0, &pBlob);
    
    // Cleanup blob if opened
    if (pBlob) {
        sqlite3_blob_close(pBlob);
    }
    
    // Final cleanup
    sqlite3_close(db);
    
    return 0;
}