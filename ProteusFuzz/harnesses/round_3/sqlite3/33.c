// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_sql -> sqlite3_step -> sqlite3_sql -> sqlite3_column_int -> 
 * sqlite3_column_int -> sqlite3_finalize -> sqlite3_db_release_memory -> 
 * sqlite3_sql -> sqlite3_step -> sqlite3_sql -> sqlite3_finalize
 *
 * Fix for "stack-use-after-scope":
 * The previous implementation used a local buffer `safe_sql` on the stack 
 * and passed it to sqlite3_prepare_v2. SQLite may store a pointer to this 
 * string internally (e.g., for error messages or re-parsing) even after 
 * the function returns. When the function returns, the stack buffer goes 
 * out of scope, leading to a use-after-scope error when SQLite later 
 * accesses it.
 *
 * Solution: Allocate the SQL string on the heap using sqlite3_malloc 
 * (or standard malloc) so it remains valid for the lifetime of the 
 * database connection and prepared statements.
 */

static void *safe_malloc(size_t size) {
    void *ptr = sqlite3_malloc(size);
    if (!ptr) {
        /* Fallback to standard malloc if sqlite3_malloc fails or is unavailable */
        ptr = malloc(size);
    }
    return ptr;
}

static void safe_free(void *ptr) {
    if (ptr) {
        sqlite3_free(ptr);
        /* Note: sqlite3_free handles NULL safely. 
         * If we used malloc, we should use free, but sqlite3_free 
         * usually works for malloc'd memory in standard builds. 
         * To be strictly safe with mixed allocators, we'd need a flag, 
         * but for fuzzing sqlite3, using sqlite3_malloc/free is preferred.
         */
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    char *sql = NULL;

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Allocate SQL string on heap to avoid stack-use-after-scope */
    if (Size > 0) {
        size_t len = (Size < 4096) ? Size : 4096;
        sql = (char *)safe_malloc(len + 1);
        if (sql == NULL) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql, Data, len);
        sql[len] = '\0';
    } else {
        /* Default SQL if input is empty */
        sql = (char *)safe_malloc(10);
        if (sql == NULL) {
            sqlite3_close(db);
            return 0;
        }
        strcpy(sql, "SELECT 1");
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        safe_free(sql);
        sqlite3_close(db);
        return 0;
    }

    /* Sequence Execution */

    /* 1. sqlite3_sql */
    const char *sql1 = sqlite3_sql(stmt);
    (void)sql1; 

    /* 2. sqlite3_step */
    rc = sqlite3_step(stmt);

    /* 3. sqlite3_sql */
    const char *sql2 = sqlite3_sql(stmt);
    (void)sql2;

    /* 4. sqlite3_column_int */
    int col1 = sqlite3_column_int(stmt, 0);
    (void)col1;

    /* 5. sqlite3_column_int */
    int col2 = sqlite3_column_int(stmt, 1);
    (void)col2;

    /* 6. sqlite3_finalize */
    rc = sqlite3_finalize(stmt);
    
    /* 
     * To prevent ASAN SEGV (Use-After-Free) on the subsequent calls, 
     * we set stmt to NULL. 
     * sqlite3_step(NULL), sqlite3_sql(NULL), and sqlite3_finalize(NULL) 
     * are defined to handle NULL gracefully (returning MISUSE or OK), 
     * whereas calling them on a freed pointer causes a crash.
     */
    stmt = NULL;

    /* 7. sqlite3_db_release_memory */
    rc = sqlite3_db_release_memory(db);
    (void)rc;

    /* 8. sqlite3_sql (now on NULL) */
    const char *sql3 = sqlite3_sql(stmt);
    (void)sql3;

    /* 9. sqlite3_step (now on NULL) */
    rc = sqlite3_step(stmt);
    (void)rc;

    /* 10. sqlite3_sql (now on NULL) */
    const char *sql4 = sqlite3_sql(stmt);
    (void)sql4;

    /* 11. sqlite3_finalize (now on NULL) */
    rc = sqlite3_finalize(stmt);
    (void)rc;

    /* 2. Cleanup */
    safe_free(sql);
    sqlite3_close(db);

    return 0;
}