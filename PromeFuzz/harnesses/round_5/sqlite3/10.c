// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;
    char buffer1[256];
    char buffer2[256];
    const unsigned char *col_text;
    const char *sql_text;
    
    if (Size < 1) return 0;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER, name TEXT);", callback, 0, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_exec(db, "INSERT INTO test VALUES (1, 'test');", callback, 0, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_prepare_v2(db, "SELECT * FROM test;", -1, &stmt1, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_step(stmt1);
    if (rc == SQLITE_ROW) {
        col_text = sqlite3_column_text(stmt1, 1);
        
        sqlite3_snprintf(sizeof(buffer1), buffer1, "Column: %s", col_text ? (const char*)col_text : "NULL");
        
        sqlite3_snprintf(sizeof(buffer2), buffer2, "Size: %zu", Size);
    }
    
    rc = sqlite3_reset(stmt1);
    
    rc = sqlite3_prepare_v2(db, "INSERT INTO test VALUES (?, ?);", -1, &stmt2, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    if (Size > 0) {
        char *bind_text = (char*)malloc(Size + 1);
        if (bind_text) {
            memcpy(bind_text, Data, Size);
            bind_text[Size] = '\0';
            sqlite3_bind_text(stmt2, 2, bind_text, -1, SQLITE_TRANSIENT);
            free(bind_text);
        }
    }
    
    sqlite3_step(stmt2);
    sqlite3_reset(stmt2);
    
    sql_text = sqlite3_sql(stmt2);
    
    sqlite3_finalize(stmt2);
    stmt2 = NULL;
    
    sqlite3_finalize(stmt1);
    stmt1 = NULL;
    
    sqlite3_finalize(NULL);

cleanup:
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt1) sqlite3_finalize(stmt1);
    if (db) sqlite3_close(db);
    
    return 0;
}