// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql = (const char *)Data;
    int rc;

    /* Ensure the input is null-terminated for SQL parsing */
    if (Size == 0) {
        return 0;
    }
    /* Create a safe copy with null termination to avoid buffer overflows in SQL parser */
    char *safe_sql = (char *)malloc(Size + 1);
    if (!safe_sql) return 0;
    memcpy(safe_sql, Data, Size);
    safe_sql[Size] = '\0';

    /* Open database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(safe_sql);
        return 0;
    }

    /* Create a table to ensure valid SQL execution context */
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, value INTEGER);";
    sqlite3_exec(db, init_sql, NULL, NULL, NULL);

    /* Prepare first statement */
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_MISUSE) {
        /* If prepare fails, we might still want to test the sequence on NULL or invalid stmts later,
           but for now, if it's a hard error, we clean up and return to avoid undefined behavior.
           However, fuzzing often wants to see behavior on error states. 
           Let's try to proceed if possible, but be careful. */
    }

    /* 1. sqlite3_sql */
    if (stmt1) {
        const char *s1 = sqlite3_sql(stmt1);
        (void)s1; /* Use return value to suppress warnings, logic is just to call it */
    }

    /* 2. sqlite3_step */
    if (stmt1) {
        rc = sqlite3_step(stmt1);
        (void)rc;
    }

    /* 3. sqlite3_sql */
    if (stmt1) {
        const char *s2 = sqlite3_sql(stmt1);
        (void)s2;
    }

    /* 4. sqlite3_column_int */
    if (stmt1) {
        int col1 = sqlite3_column_int(stmt1, 0);
        (void)col1;
    }

    /* 5. sqlite3_column_int */
    if (stmt1) {
        int col2 = sqlite3_column_int(stmt1, 1);
        (void)col2;
    }

    /* 6. sqlite3_finalize */
    if (stmt1) {
        rc = sqlite3_finalize(stmt1);
        (void)rc;
        stmt1 = NULL;
    }

    /* 7. sqlite3_db_release_memory */
    if (db) {
        rc = sqlite3_db_release_memory(db);
        (void)rc;
    }

    /* Prepare second statement for the second half of the sequence */
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt2, NULL);
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_MISUSE) {
        /* Ignore prepare errors, proceed to test sequence on potentially NULL or invalid stmt */
    }

    /* 8. sqlite3_sql */
    if (stmt2) {
        const char *s3 = sqlite3_sql(stmt2);
        (void)s3;
    }

    /* 9. sqlite3_step */
    if (stmt2) {
        rc = sqlite3_step(stmt2);
        (void)rc;
    }

    /* 10. sqlite3_sql */
    if (stmt2) {
        const char *s4 = sqlite3_sql(stmt2);
        (void)s4;
    }

    /* 11. sqlite3_finalize */
    if (stmt2) {
        rc = sqlite3_finalize(stmt2);
        (void)rc;
        stmt2 = NULL;
    }

    /* Cleanup */
    if (db) {
        sqlite3_close(db);
    }
    free(safe_sql);

    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    return fuzz_sqlite3(Data, Size);
}