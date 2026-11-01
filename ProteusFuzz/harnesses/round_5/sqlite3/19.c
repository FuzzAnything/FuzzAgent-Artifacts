// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_destructor(void *p) {
    (void)p;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    char *expanded_sql = NULL;
    int rc;

    /* Initialize database connection in memory */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    /* Ensure we have some data to work with */
    if (Size == 0) {
        sqlite3_close(db);
        return 0;
    }

    /* Step 1: sqlite3_step (on NULL to test error handling) */
    rc = sqlite3_step(NULL);
    (void)rc;

    /* Step 2: sqlite3_finalize (cleanup potential previous state) */
    rc = sqlite3_finalize(NULL);
    (void)rc;

    /* Step 3: sqlite3_finalize (again) */
    rc = sqlite3_finalize(NULL);
    (void)rc;

    /* Step 4: sqlite3_prepare_v2 */
    int sql_len = (Size > 1024) ? 1024 : (int)Size;
    const char *sql_tail = NULL;
    
    /* Create a safe SQL string by copying input and ensuring null termination */
    char *safe_sql = (char *)sqlite3_malloc(sql_len + 1);
    if (safe_sql) {
        memcpy(safe_sql, Data, sql_len);
        safe_sql[sql_len] = '\0';
        
        rc = sqlite3_prepare_v2(db, safe_sql, sql_len, &stmt1, &sql_tail);
        sqlite3_free(safe_sql);
        
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
    } else {
        sqlite3_close(db);
        return 0;
    }

    /* Step 5: sqlite3_expanded_sql */
    if (stmt1) {
        expanded_sql = sqlite3_expanded_sql(stmt1);
    }

    /* Step 6: sqlite3_free */
    if (expanded_sql) {
        sqlite3_free(expanded_sql);
        expanded_sql = NULL;
    }

    /* Step 7: sqlite3_bind_pointer */
    if (stmt1) {
        void *ptr_val = (void *)Data;
        rc = sqlite3_bind_pointer(stmt1, 1, ptr_val, "fuzz_type", dummy_destructor);
        (void)rc;
    }

    /* Step 8: sqlite3_step */
    if (stmt1) {
        rc = sqlite3_step(stmt1);
        (void)rc;
    }

    /* Step 9: sqlite3_finalize */
    if (stmt1) {
        rc = sqlite3_finalize(stmt1);
        stmt1 = NULL;
        (void)rc;
    }

    /* Step 10: sqlite3_finalize (cleanup second statement if we had one, or just NULL) */
    rc = sqlite3_finalize(stmt2);
    (void)rc;

    /* Cleanup database */
    sqlite3_close(db);

    return 0;
}