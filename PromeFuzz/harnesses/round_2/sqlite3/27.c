// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

static int dummy_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    char *mprintf_str1 = NULL;
    char *mprintf_str2 = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a table with a BLOB column for sqlite3_blob_open
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test (id, data) VALUES (1, x'01020304');"
        "INSERT INTO test (id, data) VALUES (2, x'05060708');",
        NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        // Continue anyway - table creation might fail due to memory constraints
    }
    
    // Prepare a dummy statement for sqlite3_finalize
    rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        stmt = NULL; // Ensure stmt is NULL if prepare failed
    }
    
    // 1. sqlite3_finalize
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    } else {
        // Call with NULL pointer (should be harmless)
        sqlite3_finalize(NULL);
    }
    
    // 2. sqlite3_exec
    // Use a portion of fuzz data as SQL string (limit to 100 bytes for safety)
    size_t sql_len = Size > 100 ? 100 : Size;
    char *sql = (char*)malloc(sql_len + 1);
    if (sql) {
        memcpy(sql, Data, sql_len);
        sql[sql_len] = '\0';
        
        rc = sqlite3_exec(db, sql, dummy_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        free(sql);
    }
    
    // 3. First sqlite3_mprintf
    // Use a portion of fuzz data as format string
    size_t fmt_len1 = Size > 50 ? 50 : Size;
    char *fmt1 = (char*)malloc(fmt_len1 + 1);
    if (fmt1) {
        memcpy(fmt1, Data, fmt_len1);
        fmt1[fmt_len1] = '\0';
        
        // Use different portions of data for arguments
        int int_arg = 0;
        const char *str_arg = "";
        
        if (Size >= sizeof(int)) {
            memcpy(&int_arg, Data, sizeof(int));
        }
        
        if (Size > sizeof(int)) {
            size_t str_len = Size - sizeof(int);
            str_len = str_len > 50 ? 50 : str_len;
            char *temp_str = (char*)malloc(str_len + 1);
            if (temp_str) {
                memcpy(temp_str, Data + sizeof(int), str_len);
                temp_str[str_len] = '\0';
                str_arg = temp_str;
            }
        }
        
        mprintf_str1 = sqlite3_mprintf(fmt1, int_arg, str_arg);
        
        if (Size > sizeof(int)) {
            free((void*)str_arg);
        }
        free(fmt1);
    }
    
    // 4. Second sqlite3_mprintf
    // Use another portion of fuzz data
    size_t offset = Size > 100 ? 100 : 0;
    size_t fmt_len2 = (Size - offset) > 50 ? 50 : (Size - offset);
    if (fmt_len2 > 0) {
        char *fmt2 = (char*)malloc(fmt_len2 + 1);
        if (fmt2) {
            memcpy(fmt2, Data + offset, fmt_len2);
            fmt2[fmt_len2] = '\0';
            
            // Try with different format specifiers
            mprintf_str2 = sqlite3_mprintf("%s %d %f", fmt2, (int)Size, (double)Size);
            free(fmt2);
        }
    } else {
        mprintf_str2 = sqlite3_mprintf("fuzz test %d", (int)Size);
    }
    
    // 5. First sqlite3_free
    sqlite3_free(mprintf_str1);
    
    // 6. Second sqlite3_free
    sqlite3_free(mprintf_str2);
    
    // 7. sqlite3_blob_open
    // Try to open a BLOB handle with various parameters from fuzz data
    int flags = 0;
    sqlite3_int64 rowid = 1;
    
    if (Size > 0) {
        // Use first byte to determine flags
        flags = (Data[0] & 1) ? 1 : 0;
        
        // Use portion of data for rowid
        if (Size >= sizeof(sqlite3_int64)) {
            memcpy(&rowid, Data, sizeof(sqlite3_int64));
            // Ensure rowid is positive
            if (rowid < 1) rowid = 1;
            if (rowid > 2) rowid = 2; // We only have rows 1 and 2
        }
    }
    
    rc = sqlite3_blob_open(db, "main", "test", "data", rowid, flags, &blob);
    if (rc == SQLITE_OK && blob) {
        sqlite3_blob_close(blob);
    }
    
cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}