// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:170987:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    sqlite3 *db1 = NULL;
    sqlite3 *db2 = NULL;
    sqlite3 *db3 = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *tail = NULL;
    sqlite3_blob *blob = NULL;
    
    // Use the fuzzer input to create SQL strings
    char *sql = NULL;
    if (Size > 0) {
        sql = (char *)malloc(Size + 1);
        if (!sql) return 0;
        memcpy(sql, Data, Size);
        sql[Size] = '\0';
    } else {
        sql = (char *)malloc(1);
        if (!sql) return 0;
        sql[0] = '\0';
    }
    
    // 1. sqlite3_blob_close (with NULL is safe no-op)
    sqlite3_blob_close(blob);
    
    // 2. sqlite3_exec (needs a valid database connection)
    sqlite3_open(":memory:", &db1);
    if (db1) {
        sqlite3_exec(db1, sql, dummy_callback, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    // 3. sqlite3_close (first close)
    sqlite3_close(db1);
    
    // 4. sqlite3_close (second close, NULL is safe)
    sqlite3_close(NULL);
    
    // 5. sqlite3_libversion
    (void)sqlite3_libversion();
    
    // 6. sqlite3_open
    sqlite3_open(":memory:", &db2);
    
    // 7. sqlite3_prepare_v2
    if (db2) {
        sqlite3_prepare_v2(db2, sql, -1, &stmt, &tail);
    }
    
    // 8. sqlite3_step
    if (stmt) {
        sqlite3_step(stmt);
    }
    
    // 9. sqlite3_column_text
    if (stmt) {
        (void)sqlite3_column_text(stmt, 0);
    }
    
    // 10. sqlite3_finalize
    sqlite3_finalize(stmt);
    
    // 11. sqlite3_close
    sqlite3_close(db2);
    
    // Clean up
    free(sql);
    
    // Try to open and close another database to explore more states
    sqlite3_open_v2(":memory:", &db3, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    sqlite3_close(db3);
    
    return 0;
}