// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a table with a BLOB column for sqlite3_blob_open
    const char *create_sql = 
        "CREATE TABLE test(id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test(data) VALUES(x'01020304');";
    if (sqlite3_exec(db, create_sql, NULL, NULL, &errmsg) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare a statement for sqlite3_finalize
    const char *select_sql = "SELECT * FROM test";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // 1. sqlite3_finalize
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // 2. sqlite3_exec
    // Use fuzz data as SQL (null-terminated)
    char *sql = (char*)malloc(Size + 1);
    if (!sql) goto cleanup;
    memcpy(sql, Data, Size);
    sql[Size] = '\0';
    
    int rc = sqlite3_exec(db, sql, callback, NULL, &errmsg);
    (void)rc;
    
    // 3. First sqlite3_mprintf
    // Use fuzz data as format string (truncated if needed)
    size_t format_len = Size > 100 ? 100 : Size;
    char format[101];
    memcpy(format, Data, format_len);
    format[format_len] = '\0';
    
    str1 = sqlite3_mprintf("%s", format);
    
    // 4. Second sqlite3_mprintf
    // Use different part of fuzz data
    size_t offset = Size / 2;
    size_t remaining = Size - offset;
    size_t arg_len = remaining > 50 ? 50 : remaining;
    char arg[51];
    if (arg_len > 0) {
        memcpy(arg, Data + offset, arg_len);
        arg[arg_len] = '\0';
        str2 = sqlite3_mprintf("%s %d", arg, (int)Size);
    } else {
        str2 = sqlite3_mprintf("default %d", (int)Size);
    }
    
    // 5. First sqlite3_free
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }
    
    // 6. Second sqlite3_free
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }
    
    // 7. sqlite3_blob_open
    // Use fuzz data to determine parameters
    int flags = (Size > 0 && (Data[0] & 1)) ? 1 : 0; // 0 for read-only, 1 for read/write
    sqlite3_int64 rowid = 1; // We inserted row with id=1
    
    if (sqlite3_blob_open(db, "main", "test", "data", rowid, flags, &blob) == SQLITE_OK) {
        sqlite3_blob_close(blob);
    }
    
cleanup:
    if (errmsg) sqlite3_free(errmsg);
    if (str1) sqlite3_free(str1);
    if (str2) sqlite3_free(str2);
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    free(sql);
    
    return 0;
}