// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_expanded_sql
 * - sqlite3_column_count
 * - sqlite3_sql
 * - sqlite3_next_stmt
 * - sqlite3_db_handle
 * - sqlite3_prepare_v2
 *
 * This driver uses an in-memory database to avoid file system dependencies,
 * though it also attempts to use a dummy file if needed.
 */

static void cleanup_stmt(sqlite3_stmt *pStmt) {
    if (pStmt) {
        sqlite3_finalize(pStmt);
    }
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    const char *sql = NULL;
    const char *tail = NULL;
    int rc;

    /* Ensure minimum size to avoid empty input issues */
    if (Size < 1) {
        return 0;
    }

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Optionally create a dummy file for testing file-based operations if needed */
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Use the input data as SQL, ensuring it's null-terminated */
    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) {
        cleanup_db(db);
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';
    sql = sql_buf;

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, (int)Size, &pStmt, &tail);
    
    /* Test sqlite3_sql on the statement (even if preparation failed, it might return NULL safely) */
    if (pStmt) {
        const char *orig_sql = sqlite3_sql(pStmt);
        (void)orig_sql; /* Use to avoid unused variable warning */
        
        /* Test sqlite3_column_count */
        int col_count = sqlite3_column_count(pStmt);
        (void)col_count; /* Use to avoid unused variable warning */

        /* Test sqlite3_expanded_sql */
        char *expanded = sqlite3_expanded_sql(pStmt);
        if (expanded) {
            sqlite3_free(expanded);
        }

        /* Test sqlite3_db_handle */
        sqlite3 *handle = sqlite3_db_handle(pStmt);
        (void)handle; /* Use to avoid unused variable warning */
    }

    /* Test sqlite3_next_stmt */
    sqlite3_stmt *next_stmt = sqlite3_next_stmt(db, NULL);
    while (next_stmt) {
        /* Perform checks on the next statement */
        const char *ns_sql = sqlite3_sql(next_stmt);
        (void)ns_sql;
        int ns_col_count = sqlite3_column_count(next_stmt);
        (void)ns_col_count;
        char *ns_expanded = sqlite3_expanded_sql(next_stmt);
        if (ns_expanded) {
            sqlite3_free(ns_expanded);
        }
        sqlite3 *ns_handle = sqlite3_db_handle(next_stmt);
        (void)ns_handle;

        next_stmt = sqlite3_next_stmt(db, next_stmt);
    }

    /* Cleanup */
    cleanup_stmt(pStmt);
    free(sql_buf);
    cleanup_db(db);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}