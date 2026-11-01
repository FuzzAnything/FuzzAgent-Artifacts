// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:185731:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence:
 * sqlite3_blob_close -> sqlite3_exec -> sqlite3_close -> sqlite3_close ->
 * sqlite3_libversion -> sqlite3_open -> sqlite3_prepare_v2 -> sqlite3_step ->
 * sqlite3_column_text -> sqlite3_finalize -> sqlite3_close
 */

static int dummy_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    int rc;
    const char *version;
    const unsigned char *text;

    /* 1. sqlite3_blob_close */
    /* Passing NULL is a harmless no-op as per documentation */
    rc = sqlite3_blob_close(blob);
    (void)rc;

    /* 2. sqlite3_exec */
    /* We need a valid database connection for this. We create a temp one. */
    sqlite3 *temp_db = NULL;
    rc = sqlite3_open(":memory:", &temp_db);
    if (rc == SQLITE_OK) {
        /* Use the fuzz input as SQL if possible, ensuring null-termination */
        const char *sql = (const char *)Data;
        if (Size > 0) {
            /* Fuzz input might not be null-terminated. Allocate and copy. */
            char *safe_sql = (char *)malloc(Size + 1);
            if (safe_sql) {
                memcpy(safe_sql, Data, Size);
                safe_sql[Size] = '\0';
                rc = sqlite3_exec(temp_db, safe_sql, dummy_callback, NULL, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
                free(safe_sql);
            } else {
                /* Fallback if malloc fails */
                rc = sqlite3_exec(temp_db, "SELECT 1;", dummy_callback, NULL, &errmsg);
                if (errmsg) {
                    sqlite3_free(errmsg);
                    errmsg = NULL;
                }
            }
        } else {
            /* Empty input, run a safe query */
            rc = sqlite3_exec(temp_db, "SELECT 1;", dummy_callback, NULL, &errmsg);
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }

        /* 3. sqlite3_close */
        rc = sqlite3_close(temp_db);
        (void)rc;
        temp_db = NULL;
    }

    /* 4. sqlite3_close (second time) */
    /* Passing NULL is safe */
    rc = sqlite3_close(temp_db);
    (void)rc;

    /* 5. sqlite3_libversion */
    version = sqlite3_libversion();
    (void)version;

    /* 6. sqlite3_open */
    /* Open a real file or memory DB for the rest of the tests */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we can't open, we can't proceed with prepare/step. */
        goto cleanup;
    }

    /* 7. sqlite3_prepare_v2 */
    /* Prepare a statement using the fuzz input as SQL */
    const char *zSql = (const char *)Data;
    int nByte = Size;
    const char *pzTail = NULL;
    
    /* If Size is 0, pass an empty string to avoid parsing issues */
    if (nByte == 0) {
        zSql = "";
        nByte = 1;
    }

    rc = sqlite3_prepare_v2(db, zSql, nByte, &stmt, &pzTail);
    
    if (rc != SQLITE_OK) {
        /* Prepare failed. stmt is NULL. */
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    } else {
        /* 8. sqlite3_step */
        rc = sqlite3_step(stmt);
        
        /* 9. sqlite3_column_text */
        if (rc == SQLITE_ROW) {
            text = sqlite3_column_text(stmt, 0);
            (void)text;
        } else {
            /* Even if not a row, we can try to get text (returns NULL) */
            text = sqlite3_column_text(stmt, 0);
            (void)text;
        }
        
        /* 10. sqlite3_finalize */
        rc = sqlite3_finalize(stmt);
        (void)rc;
        stmt = NULL;
    }

    /* 11. sqlite3_close */
    rc = sqlite3_close(db);
    (void)rc;
    db = NULL;

cleanup:
    /* Cleanup any remaining resources */
    if (db) {
        sqlite3_close(db);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (blob) {
        sqlite3_blob_close(blob);
    }
    /* errmsg should have been freed immediately after use in the block above */
    if (errmsg) {
        sqlite3_free(errmsg);
    }

    return 0;
}