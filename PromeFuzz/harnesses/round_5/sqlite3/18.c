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
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
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
    /* Empty destructor for sqlite3_bind_pointer */
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *tail = NULL;
    char *expanded_sql = NULL;
    int rc;
    static int dummy_ptr = 42;

    /* Open an in-memory database */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to have something to query */
    sqlite3_exec(db, "CREATE TABLE t1(a, b); INSERT INTO t1 VALUES(1, 2);", NULL, NULL, NULL);

    /* Prepare first statement */
    const char *sql1 = "SELECT * FROM t1";
    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt1, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 1. sqlite3_step */
    sqlite3_step(stmt1);

    /* 2. sqlite3_finalize */
    sqlite3_finalize(stmt1);

    /* 3. sqlite3_finalize (on NULL is safe no-op) */
    sqlite3_finalize(NULL);

    /* 4. sqlite3_prepare_v2 */
    /* Use fuzzer input as SQL if it's valid UTF-8 and null-terminated */
    char *sql2 = NULL;
    if (Size > 0) {
        sql2 = (char *)malloc(Size + 1);
        if (sql2) {
            memcpy(sql2, Data, Size);
            sql2[Size] = '\0';
            rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, &tail);
            free(sql2);
        } else {
            rc = SQLITE_NOMEM;
        }
    } else {
        /* Fallback SQL if no input */
        rc = sqlite3_prepare_v2(db, "SELECT ?", -1, &stmt2, &tail);
    }

    if (rc != SQLITE_OK) {
        /* If prepare failed, create a valid statement for the remaining calls */
        sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt2, &tail);
    }

    /* 5. sqlite3_expanded_sql */
    expanded_sql = sqlite3_expanded_sql(stmt2);

    /* 6. sqlite3_free */
    sqlite3_free(expanded_sql);

    /* 7. sqlite3_bind_pointer */
    /* Bind the dummy pointer with a type string */
    sqlite3_bind_pointer(stmt2, 1, &dummy_ptr, "dummy_type", dummy_destructor);

    /* 8. sqlite3_step */
    sqlite3_step(stmt2);

    /* 9. sqlite3_finalize */
    sqlite3_finalize(stmt2);

    /* 10. sqlite3_finalize */
    sqlite3_finalize(NULL);

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}