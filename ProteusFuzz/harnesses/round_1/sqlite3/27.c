// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
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
 * Fuzz driver for sqlite3 targeting the sequence:
 * sqlite3_finalize -> sqlite3_exec -> sqlite3_mprintf -> sqlite3_mprintf -> sqlite3_free -> sqlite3_free -> sqlite3_blob_open
 */

static int dummy_callback(void *arg, int argc, char **argv, char **col_names) {
    (void)arg;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *sql = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    sqlite3_blob *blob = NULL;
    int rc;

    /* Ensure minimum size to avoid trivial inputs */
    if (Size < 1) {
        return 0;
    }

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Step 1: sqlite3_finalize
     * We can safely call finalize on NULL or an uninitialized pointer.
     * To explore more states, we might try to prepare a statement first if data allows.
     */
    if (Size > 5) {
        /* Create a safe SQL string from input, ensuring null termination */
        size_t sql_len = (Size > 100) ? 100 : Size;
        char safe_sql[101];
        memcpy(safe_sql, Data, sql_len);
        safe_sql[sql_len] = '\0';

        /* Try to prepare a statement to have something to finalize */
        rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt != NULL) {
            /* Execute step to potentially trigger errors before finalize */
            sqlite3_step(stmt);
        }
    }
    /* Call finalize as required by the sequence. Safe on NULL. */
    sqlite3_finalize(stmt);

    /* Step 2: sqlite3_exec
     * Execute SQL statements. We reuse input data as SQL if possible.
     */
    if (Size > 5) {
        size_t sql_len = (Size > 500) ? 500 : Size;
        char safe_sql[501];
        memcpy(safe_sql, Data, sql_len);
        safe_sql[sql_len] = '\0';

        /* Ensure SQL is not empty or just whitespace for better coverage */
        if (safe_sql[0] != '\0') {
            rc = sqlite3_exec(db, safe_sql, dummy_callback, NULL, &errmsg);
            if (errmsg != NULL) {
                sqlite3_free(errmsg);
                errmsg = NULL;
            }
        }
    }

    /* Step 3 & 4: sqlite3_mprintf (twice)
     * Use input data as format string or arguments.
     * Be careful with format specifiers to avoid crashes, but fuzzing should explore them.
     */
    if (Size > 0) {
        /* Create a safe format string buffer */
        char fmt[256];
        size_t fmt_len = (Size > 255) ? 255 : Size;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';

        /* First mprintf */
        str1 = sqlite3_mprintf(fmt, 12345);
        
        /* Second mprintf */
        str2 = sqlite3_mprintf(fmt, 67890);
    }

    /* Step 5 & 6: sqlite3_free (twice)
     * Free the strings allocated by mprintf.
     */
    if (str1 != NULL) {
        sqlite3_free(str1);
        str1 = NULL;
    }
    if (str2 != NULL) {
        sqlite3_free(str2);
        str2 = NULL;
    }

    /* Step 7: sqlite3_blob_open
     * This requires a valid table and row. We need to ensure a table exists.
     * We'll create a simple table if it doesn't exist, then try to open a blob.
     */
    
    /* Create a table for blob operations if it doesn't exist */
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS fuzz_blob (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, &errmsg);
    if (errmsg != NULL) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* Insert a row to ensure we have a rowid to open a blob on */
    const char *insert_sql = "INSERT INTO fuzz_blob (data) VALUES (zeroblob(100));";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, &errmsg);
    if (errmsg != NULL) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    /* Attempt to open a blob. 
     * We use "main" as db name, "fuzz_blob" as table, "data" as column.
     * Rowid 1 should exist from the insert.
     * Flags: 0 for read-only, 1 for read-write. We'll vary based on input if possible, but 0 is safe.
     */
    sqlite3_int64 rowid = 1;
    int flags = (Size > 0 && Data[0] % 2 == 1) ? 1 : 0;

    rc = sqlite3_blob_open(db, "main", "fuzz_blob", "data", rowid, flags, &blob);
    
    /* If blob was opened successfully, close it to prevent leaks */
    if (blob != NULL) {
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    /* Cleanup database connection */
    sqlite3_close(db);

    return 0;
}