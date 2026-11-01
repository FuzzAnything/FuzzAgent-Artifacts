// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *db_name;
    int op1, op2;
    int db_idx;
    const char *zDbName;
    int dummy_arg = 0;

    /* Ensure minimum size to extract parameters safely */
    if (Size < 4) {
        return 0;
    }

    /* Extract fuzzing parameters from input data */
    op1 = Data[0];
    op2 = Data[1];
    db_idx = Data[2] % 5; /* Limit to 0-4 to test main, temp, and attached */
    
    /* Determine database name for file_control based on index */
    if (db_idx == 0) {
        zDbName = "main";
    } else if (db_idx == 1) {
        zDbName = "temp";
    } else {
        /* For N > 1, use NULL or a non-existent name to trigger error paths */
        zDbName = (Size > 3 && Data[3] == 0) ? NULL : "nonexistent_db";
    }

    /* Step 1: sqlite3_open */
    /* Use a dummy file as per instructions */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        /* If open fails but db is not NULL, close it immediately */
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    /* Step 2: sqlite3_db_name */
    /* Retrieve schema name for the N-th database */
    db_name = sqlite3_db_name(db, db_idx);
    /* We don't need to do anything with db_name, just invoking it to exercise the code path.
       Note: The returned string is managed by SQLite, so we don't free it. */
    (void)db_name;

    /* Step 3: sqlite3_file_control (First invocation) */
    /* 
       The crash occurred because passing NULL as the 4th argument (pArg) 
       caused sqlite3_file_control to dereference it for certain opcodes.
       We provide a valid pointer to a dummy integer instead.
    */
    rc = sqlite3_file_control(db, zDbName, op1, &dummy_arg);
    (void)rc;

    /* Step 4: sqlite3_file_control (Second invocation) */
    rc = sqlite3_file_control(db, zDbName, op2, &dummy_arg);
    (void)rc;

    /* Step 5: sqlite3_close */
    /* Close the first connection */
    rc = sqlite3_close(db);
    /* If SQLITE_BUSY, it means resources were left open, but we didn't create any statements.
       This is just a state exploration. */
    db = NULL; /* Reset pointer */

    /* Step 6: sqlite3_open (Second invocation as per sequence) */
    /* Re-open the database to test state after close */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    /* Cleanup: Close the final connection */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}