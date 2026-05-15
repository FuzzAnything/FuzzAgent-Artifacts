// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:172022:26 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:172038:26 in sqlite3.h
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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Use first byte to decide which function to test */
    uint8_t selector = Data[0];
    const uint8_t *sql_data = Data + 1;
    size_t sql_size = Size - 1;
    
    /* Ensure SQL string is null-terminated */
    char *sql = malloc(sql_size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, sql_data, sql_size);
    sql[sql_size] = '\0';
    
    switch (selector % 6) {
        case 0: /* sqlite3_exec */
            rc = sqlite3_exec(db, sql, callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            break;
            
        case 1: /* sqlite3_changes64 */
            /* First execute some SQL that might cause changes */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(x);", NULL, NULL, NULL);
            sqlite3_exec(db, sql, NULL, NULL, NULL);
            (void)sqlite3_changes64(db);
            break;
            
        case 2: /* sqlite3_step */
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc == SQLITE_OK && stmt) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    /* Do nothing with row data */
                }
                sqlite3_finalize(stmt);
            }
            break;
            
        case 3: /* sqlite3_changes */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t2(y);", NULL, NULL, NULL);
            sqlite3_exec(db, sql, NULL, NULL, NULL);
            (void)sqlite3_changes(db);
            break;
            
        case 4: /* sqlite3_total_changes */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t3(z);", NULL, NULL, NULL);
            sqlite3_exec(db, sql, NULL, NULL, NULL);
            (void)sqlite3_total_changes(db);
            break;
            
        case 5: /* sqlite3_total_changes64 */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t4(w);", NULL, NULL, NULL);
            sqlite3_exec(db, sql, NULL, NULL, NULL);
            (void)sqlite3_total_changes64(db);
            break;
    }
    
    free(sql);
    sqlite3_close(db);
    
    return 0;
}