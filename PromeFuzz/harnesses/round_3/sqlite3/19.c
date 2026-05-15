// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
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

static void dummy_destructor(void *ptr) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *tail = NULL;
    char *expanded_sql = NULL;
    int rc;
    
    /* Open an in-memory database */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Create a dummy table to have something to query */
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t(x);", NULL, NULL, NULL);
    
    /* Prepare first statement */
    const char *sql = "SELECT * FROM t;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt1, &tail) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* First sqlite3_step */
    sqlite3_step(stmt1);
    
    /* First sqlite3_finalize */
    sqlite3_finalize(stmt1);
    
    /* Second sqlite3_finalize (on NULL is safe) */
    sqlite3_finalize(NULL);
    
    /* Prepare second statement using fuzzer input as SQL */
    char *sql_buffer = NULL;
    if (Size > 0) {
        sql_buffer = (char *)malloc(Size + 1);
        if (sql_buffer) {
            memcpy(sql_buffer, Data, Size);
            sql_buffer[Size] = '\0';
            sqlite3_prepare_v2(db, sql_buffer, -1, &stmt2, &tail);
            free(sql_buffer);
        }
    } else {
        /* Use a simple SQL if no input */
        sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt2, &tail);
    }
    
    if (stmt2) {
        /* sqlite3_expanded_sql */
        expanded_sql = sqlite3_expanded_sql(stmt2);
        
        /* sqlite3_free */
        sqlite3_free(expanded_sql);
        
        /* sqlite3_bind_pointer - bind NULL pointer with dummy destructor */
        sqlite3_bind_pointer(stmt2, 1, NULL, "dummy_type", dummy_destructor);
        
        /* Second sqlite3_step */
        rc = sqlite3_step(stmt2);
        while (rc == SQLITE_ROW) {
            rc = sqlite3_step(stmt2);
        }
        
        /* Third sqlite3_finalize */
        sqlite3_finalize(stmt2);
    }
    
    /* Fourth sqlite3_finalize (on NULL is safe) */
    sqlite3_finalize(NULL);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}