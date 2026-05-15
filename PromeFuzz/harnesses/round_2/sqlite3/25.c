// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:170987:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db1 = NULL, *db2 = NULL, *db3 = NULL;
    sqlite3_stmt *stmt = NULL;
    char *err_msg = NULL;
    const char *tail = NULL;
    
    /* 1. sqlite3_blob_close with NULL (safe no-op) */
    sqlite3_blob_close(NULL);
    
    /* 2. sqlite3_exec - need an open database first */
    if (sqlite3_open(":memory:", &db1) == SQLITE_OK) {
        /* Use first part of data as SQL string for exec */
        size_t sql_len = Size > 1024 ? 1024 : Size;
        char *sql = malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data, sql_len);
            sql[sql_len] = '\0';
            sqlite3_exec(db1, sql, dummy_callback, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            free(sql);
        }
    }
    
    /* 3. First sqlite3_close */
    sqlite3_close(db1);
    
    /* 4. Second sqlite3_close with NULL (safe no-op) */
    sqlite3_close(NULL);
    
    /* 5. sqlite3_libversion */
    const char *version = sqlite3_libversion();
    (void)version; /* Use result to avoid unused variable warning */
    
    /* 6. sqlite3_open */
    if (sqlite3_open(":memory:", &db2) != SQLITE_OK) {
        return 0;
    }
    
    /* 7. sqlite3_prepare_v2 */
    /* Use data as SQL string, but ensure null termination */
    size_t prepare_sql_len = Size > 512 ? 512 : Size;
    char *prepare_sql = malloc(prepare_sql_len + 1);
    if (!prepare_sql) {
        sqlite3_close(db2);
        return 0;
    }
    memcpy(prepare_sql, Data, prepare_sql_len);
    prepare_sql[prepare_sql_len] = '\0';
    
    int rc = sqlite3_prepare_v2(db2, prepare_sql, -1, &stmt, &tail);
    
    /* 8. sqlite3_step */
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* 9. sqlite3_column_text */
            if (sqlite3_column_count(stmt) > 0) {
                const unsigned char *text = sqlite3_column_text(stmt, 0);
                (void)text; /* Use result */
            }
        }
    }
    
    /* 10. sqlite3_finalize */
    sqlite3_finalize(stmt);
    
    /* 11. sqlite3_close */
    sqlite3_close(db2);
    
    free(prepare_sql);
    
    /* Try to open and close another database to explore more states */
    if (sqlite3_open_v2(":memory:", &db3, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK) {
        sqlite3_close(db3);
    }
    
    return 0;
}