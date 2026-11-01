// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_libversion at sqlite3.c:185731:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
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

/*
 * Fuzz driver for sqlite3 library.
 * Invokes API functions in the specified order:
 * sqlite3_blob_close -> sqlite3_exec -> sqlite3_close -> sqlite3_close ->
 * sqlite3_libversion -> sqlite3_open -> sqlite3_prepare_v2 -> sqlite3_step ->
 * sqlite3_column_text -> sqlite3_finalize -> sqlite3_close
 */

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *version;
    char *errmsg = NULL;
    int rc;

    /* 1. sqlite3_blob_close: Safe to call with NULL (no-op) */
    sqlite3_blob_close(blob);

    /* 2. sqlite3_exec: Requires an open database. We will open it later, 
       but the order is strict. We can't execute without a DB.
       However, the task asks to invoke in order. 
       To strictly follow the order while maintaining validity, we must open the DB first?
       No, the prompt says "invoke the functions in the following order".
       If we call sqlite3_exec with NULL db, it might crash or return SQLITE_MISUSE.
       Let's try to interpret "invoke" as "call the function". 
       Calling sqlite3_exec(NULL, ...) is undefined behavior but often returns SQLITE_MISUSE.
       However, to be safe and explore states, let's open a DB first? 
       Wait, the order is explicit: blob_close -> exec -> close -> close -> libversion -> open ...
       This order implies we must call exec before open. 
       If we pass NULL to exec, it's a valid test case for robustness (checking for NULL checks).
       
       Let's attempt to call sqlite3_exec with NULL db and NULL sql to test error handling.
    */
    if (Data && Size > 0) {
        /* Use input data as SQL if possible, but ensure null termination if needed */
        /* We cannot easily pass Data as SQL to sqlite3_exec without a valid DB.
           Let's pass NULL for DB and a safe SQL string or NULL. */
        sqlite3_exec(NULL, NULL, NULL, NULL, NULL);
    }

    /* 3. sqlite3_close: Safe with NULL */
    sqlite3_close(db);

    /* 4. sqlite3_close: Safe with NULL */
    sqlite3_close(db);

    /* 5. sqlite3_libversion: Always safe */
    version = sqlite3_libversion();
    (void)version; /* Suppress unused warning */

    /* 6. sqlite3_open: Open a database (in-memory to avoid file system issues) */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If open fails, we can't proceed with prepare/step. 
           But we should still try to clean up if db was partially allocated.
           sqlite3_close handles NULL gracefully. */
        sqlite3_close(db);
        db = NULL;
        return 0;
    }

    /* 7. sqlite3_prepare_v2: Prepare a statement */
    /* Use input data as SQL if it looks like SQL, otherwise use a safe default */
    const char *sql = NULL;
    int sql_len = 0;
    
    if (Size > 0) {
        /* Ensure the input is treated as a C string. 
           If the input contains null bytes, sqlite3_prepare_v2 might stop early, which is fine.
           We pass the raw data. */
        sql = (const char *)Data;
        sql_len = (int)Size;
    } else {
        /* Fallback SQL if input is empty */
        sql = "SELECT 1;";
        sql_len = 8;
    }

    rc = sqlite3_prepare_v2(db, sql, sql_len, &stmt, NULL);
    if (rc != SQLITE_OK) {
        /* Prepare failed. We can still try to close the DB and exit. */
        sqlite3_close(db);
        db = NULL;
        return 0;
    }

    /* 8. sqlite3_step: Execute the statement */
    /* Step until done or error */
    rc = sqlite3_step(stmt);
    /* We only step once to keep the flow simple and match the sequence */

    /* 9. sqlite3_column_text: Retrieve data from the first column */
    /* Safe to call even if step didn't return SQLITE_ROW (returns NULL) */
    const unsigned char *col_text = sqlite3_column_text(stmt, 0);
    (void)col_text;

    /* 10. sqlite3_finalize: Clean up the statement */
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* 11. sqlite3_close: Clean up the database */
    sqlite3_close(db);
    db = NULL;

    return 0;
}