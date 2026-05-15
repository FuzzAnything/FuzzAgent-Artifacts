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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    const char *err_msg;
    int step_rc1, step_rc2;
    int col_count;
    int col_type;
    const char *col_name;
    const unsigned char *col_text1;
    const unsigned char *col_text2;
    const unsigned char *col_text3;
    int col_bytes;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }

    /* Prepare SQL statement from fuzzer input */
    /* Ensure the SQL string is null-terminated */
    char *sql = malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    /* Get error message */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Use variable to avoid unused warning */

    /* First step */
    step_rc1 = sqlite3_step(stmt);
    
    /* Second step */
    step_rc2 = sqlite3_step(stmt);
    
    /* Get column count */
    col_count = sqlite3_column_count(stmt);
    
    /* Get column type for first column if available */
    if (col_count > 0) {
        col_type = sqlite3_column_type(stmt, 0);
        
        /* Get column name for first column */
        col_name = sqlite3_column_name(stmt, 0);
        (void)col_name; /* Use variable to avoid unused warning */
        
        /* Get column text three times */
        col_text1 = sqlite3_column_text(stmt, 0);
        col_text2 = sqlite3_column_text(stmt, 0);
        col_text3 = sqlite3_column_text(stmt, 0);
        (void)col_text1; /* Use variables to avoid unused warnings */
        (void)col_text2;
        (void)col_text3;
        
        /* Get column bytes */
        col_bytes = sqlite3_column_bytes(stmt, 0);
        (void)col_bytes; /* Use variable to avoid unused warning */
    }

    /* Finalize the statement */
    sqlite3_finalize(stmt);
    
    /* Cleanup */
    free(sql);
    sqlite3_close(db);
    
    return 0;
}