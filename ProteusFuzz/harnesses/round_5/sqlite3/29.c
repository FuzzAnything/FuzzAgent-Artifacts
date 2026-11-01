// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    if (Size < 1) {
        return 0;
    }

    /* Create a temporary database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to ensure valid statements can be prepared */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, value INTEGER);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement from the fuzz input if possible, otherwise use a safe default */
    const char *input_sql = (const char *)Data;
    size_t sql_len = Size;
    
    /* Ensure null termination for safety */
    char *safe_sql = (char *)malloc(sql_len + 1);
    if (!safe_sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(safe_sql, input_sql, sql_len);
    safe_sql[sql_len] = '\0';

    /* Try to prepare the statement */
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
    free(safe_sql);

    /* If preparation failed, try a safe default statement */
    if (rc != SQLITE_OK || stmt == NULL) {
        rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL);
    }

    if (stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Invoke target functions in the specified order */

    /* sqlite3_sql */
    (void)sqlite3_sql(stmt);

    /* sqlite3_step */
    (void)sqlite3_step(stmt);

    /* sqlite3_sql */
    (void)sqlite3_sql(stmt);

    /* sqlite3_column_int (column 0) */
    (void)sqlite3_column_int(stmt, 0);

    /* sqlite3_column_int (column 1) */
    (void)sqlite3_column_int(stmt, 1);

    /* sqlite3_finalize */
    sqlite3_finalize(stmt);
    stmt = NULL; /* Ensure we don't use the finalized statement */

    /* sqlite3_db_release_memory */
    (void)sqlite3_db_release_memory(db);

    /* Prepare a new statement for the next sequence */
    /* We reuse the input data or a safe default again */
    safe_sql = (char *)malloc(Size + 1);
    if (safe_sql) {
        memcpy(safe_sql, Data, Size);
        safe_sql[Size] = '\0';
        rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
        free(safe_sql);
    }
    
    if (rc != SQLITE_OK || stmt == NULL) {
        /* Fallback to safe statement if input was invalid */
        rc = sqlite3_prepare_v2(db, "SELECT 2;", -1, &stmt, NULL);
    }

    if (stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* sqlite3_sql */
    (void)sqlite3_sql(stmt);

    /* sqlite3_step */
    (void)sqlite3_step(stmt);

    /* sqlite3_sql */
    (void)sqlite3_sql(stmt);

    /* sqlite3_finalize */
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* 3. Cleanup */
    sqlite3_close(db);

    return 0;
}