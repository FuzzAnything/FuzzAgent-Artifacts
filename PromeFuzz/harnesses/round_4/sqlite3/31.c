// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
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

static void dummy_free(void *ptr) {
    /* Do nothing - used as callback for sqlite3_bind_text */
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *err_msg = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Create a simple table and prepare a statement */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(id INTEGER, txt TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    rc = sqlite3_prepare_v2(db, "INSERT INTO t1 VALUES(?, ?)", -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    /* First sqlite3_reset call */
    rc = sqlite3_reset(stmt);
    
    /* Prepare text data from fuzzer input */
    char *text_data = NULL;
    if (Size > 0) {
        text_data = (char *)malloc(Size + 1);
        if (text_data) {
            memcpy(text_data, Data, Size);
            text_data[Size] = '\0';
        }
    } else {
        text_data = (char *)malloc(1);
        if (text_data) {
            text_data[0] = '\0';
        }
    }
    
    if (!text_data) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    /* sqlite3_bind_text call */
    rc = sqlite3_bind_text(stmt, 2, text_data, -1, dummy_free);
    
    /* Also bind an integer parameter */
    int int_param = (Size > 0) ? (int)(Data[0]) : 0;
    rc = sqlite3_bind_int(stmt, 1, int_param);
    
    /* sqlite3_step call */
    rc = sqlite3_step(stmt);
    
    /* sqlite3_errmsg call */
    err_msg = sqlite3_errmsg(db);
    
    /* Second sqlite3_reset call */
    rc = sqlite3_reset(stmt);
    
    /* sqlite3_changes call */
    int changes = sqlite3_changes(db);
    
    /* Cleanup */
    free(text_data);
    sqlite3_finalize(stmt);
    
    /* Try to exercise different states by creating another statement */
    sqlite3_stmt *stmt2 = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT * FROM t1", -1, &stmt2, NULL);
    if (rc == SQLITE_OK && stmt2) {
        rc = sqlite3_reset(stmt2);
        rc = sqlite3_step(stmt2);
        sqlite3_finalize(stmt2);
    }
    
    sqlite3_close(db);
    
    return 0;
}