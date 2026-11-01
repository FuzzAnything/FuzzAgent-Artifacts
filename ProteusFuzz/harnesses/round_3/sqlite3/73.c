// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:93828:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_sqlite3_column_info(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int col_count = 0;
    int i;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a dummy table to ensure we have a schema to query */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1 (c1 VARIANT, c2 INTEGER, c3 TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Use the fuzzing input as the SQL statement */
    /* Ensure null termination for safety if sqlite3_prepare_v2 expects it, 
       though nByte handles length. We pass Size as nByte. */
    rc = sqlite3_prepare_v2(db, (const char *)Data, Size, &stmt, NULL);

    if (rc == SQLITE_OK && stmt != NULL) {
        col_count = sqlite3_column_count(stmt);

        /* Fuzz column info retrieval */
        for (i = 0; i < col_count; i++) {
            /* Test sqlite3_column_name */
            const char *name = sqlite3_column_name(stmt, i);
            (void)name; /* Use to avoid unused warning, logic is side-effect free */

            /* Test sqlite3_column_decltype */
            const char *decltype = sqlite3_column_decltype(stmt, i);
            (void)decltype;

            /* Test sqlite3_column_int (requires stepping first) */
            /* We step to try to get data, but even if no rows, 
               the function is safe to call on a prepared statement.
               However, it returns 0 if not at a row. */
            int val = sqlite3_column_int(stmt, i);
            (void)val;
        }

        /* Test sqlite3_sql */
        const char *sql_text = sqlite3_sql(stmt);
        (void)sql_text;

        /* Execute step to potentially populate column values for int retrieval */
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (i = 0; i < col_count; i++) {
                int val = sqlite3_column_int(stmt, i);
                (void)val;
            }
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return 0;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input is treated as a string if possible, 
       though prepare_v2 handles binary data up to nByte.
       We just pass it directly. */
    
    fuzz_sqlite3_column_info(Data, Size);

    return 0;
}