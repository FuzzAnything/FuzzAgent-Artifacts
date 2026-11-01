// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/*
 * Fuzz driver for sqlite3.
 * Invokes the target API functions in the specified order:
 * sqlite3_finalize -> sqlite3_exec -> sqlite3_mprintf -> sqlite3_mprintf -> sqlite3_free -> sqlite3_free -> sqlite3_blob_open
 */

static int callback(void *data, int argc, char **argv, char **col) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col;
    return 0;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    sqlite3_blob *blob = NULL;
    const char *dummy_file = "./dummy_file";
    
    /* Initialize the database connection */
    if (sqlite3_open_v2(dummy_file, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 1. sqlite3_finalize: Can be called on NULL or uninitialized stmt safely */
    sqlite3_finalize(stmt);

    /* 2. sqlite3_exec: Execute SQL using fuzzed input */
    /* Ensure the input is null-terminated for safety */
    if (Size > 0) {
        /* Create a safe buffer for SQL */
        char *sql_buf = (char *)malloc(Size + 1);
        if (sql_buf) {
            memcpy(sql_buf, Data, Size);
            sql_buf[Size] = '\0';
            
            /* Execute the SQL */
            sqlite3_exec(db, sql_buf, callback, NULL, &errmsg);
            
            /* Clean up error message if present */
            if (errmsg) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
            free(sql_buf);
        }
    }

    /* 3. sqlite3_mprintf: First allocation */
    if (Size > 0) {
        /* Use part of the input as format string, ensuring safety */
        char *format_buf = (char *)malloc(Size + 1);
        if (format_buf) {
            memcpy(format_buf, Data, Size);
            format_buf[Size] = '\0';
            
            /* Simple format to avoid complex parsing issues */
            str1 = sqlite3_mprintf("%s", format_buf);
            free(format_buf);
        }
    }

    /* 4. sqlite3_mprintf: Second allocation */
    if (Size > 0) {
        char *format_buf = (char *)malloc(Size + 1);
        if (format_buf) {
            memcpy(format_buf, Data, Size);
            format_buf[Size] = '\0';
            
            str2 = sqlite3_mprintf("%s", format_buf);
            free(format_buf);
        }
    }

    /* 5. sqlite3_free: Free first string */
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }

    /* 6. sqlite3_free: Free second string */
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }

    /* 7. sqlite3_blob_open: Attempt to open a BLOB */
    /* We need a table to open a blob from. Create one first if it doesn't exist. */
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS fuzz_blob_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* Insert a row to ensure we have data to open a blob on */
    const char *insert_sql = "INSERT INTO fuzz_blob_table (data) VALUES (zeroblob(100));";
    sqlite3_exec(db, insert_sql, NULL, NULL, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* Try to open a blob on rowid 1 */
    sqlite3_blob_open(db, "main", "fuzz_blob_table", "data", 1, 0, &blob);
    
    /* Close the blob if opened successfully */
    if (blob) {
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    /* Cleanup database connection */
    sqlite3_close(db);
    db = NULL;

    return 0;
}