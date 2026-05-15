// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
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
#include <stdio.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *zErrMsg = NULL;
    int rc;
    int col_count;
    int i;
    const char *col_name;
    const unsigned char *text_val;
    int bytes_val;
    int col_type;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    /* Prepare a statement using the fuzzer input as SQL */
    /* Ensure null termination for SQL string */
    char *sql = (char *)malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    const char *tail = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    /* Get error message (even if prepare succeeded) */
    zErrMsg = sqlite3_errmsg(db);
    (void)zErrMsg; /* Use the result to avoid unused variable warning */

    if (rc == SQLITE_OK && stmt) {
        /* First step */
        rc = sqlite3_step(stmt);
        
        /* Second step (might be SQLITE_ROW or SQLITE_DONE) */
        rc = sqlite3_step(stmt);
        
        /* Get column count */
        col_count = sqlite3_column_count(stmt);
        
        if (col_count > 0) {
            /* Use first column (index 0) for column operations */
            i = 0;
            
            /* Get column type */
            col_type = sqlite3_column_type(stmt, i);
            
            /* Get column name */
            col_name = sqlite3_column_name(stmt, i);
            (void)col_name;
            
            /* Get column text value three times */
            text_val = sqlite3_column_text(stmt, i);
            (void)text_val;
            text_val = sqlite3_column_text(stmt, i);
            (void)text_val;
            text_val = sqlite3_column_text(stmt, i);
            (void)text_val;
            
            /* Get column bytes */
            bytes_val = sqlite3_column_bytes(stmt, i);
            (void)bytes_val;
        }
        
        /* Finalize the statement */
        rc = sqlite3_finalize(stmt);
    }

    free(sql);
    sqlite3_close(db);
    
    return 0;
}