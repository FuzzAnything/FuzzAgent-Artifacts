// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *tail = NULL;

    /* Create an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to ensure we have something to query */
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, init_sql, NULL, NULL, NULL);

    /* Use the fuzz input as the SQL statement */
    const char *sql_input = (const char *)Data;
    
    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql_input, (int)Size, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Step through the statement to populate column data */
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Fuzz sqlite3_column_type */
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                (void)sqlite3_column_type(stmt, i);
                
                /* Fuzz sqlite3_column_int64 */
                (void)sqlite3_column_int64(stmt, i);
            }
        }

        /* Fuzz sqlite3_db_handle */
        sqlite3 *handle = sqlite3_db_handle(stmt);
        (void)handle; /* Suppress unused variable warning */

        /* Fuzz sqlite3_reset */
        (void)sqlite3_reset(stmt);

        /* Fuzz sqlite3_interrupt */
        sqlite3_interrupt(db);
    }

    /* Cleanup */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}