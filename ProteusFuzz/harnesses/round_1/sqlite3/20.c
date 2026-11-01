// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence:
 * sqlite3_step -> sqlite3_finalize -> sqlite3_finalize -> 
 * sqlite3_prepare_v2 -> sqlite3_expanded_sql -> sqlite3_free -> 
 * sqlite3_bind_pointer -> sqlite3_step -> sqlite3_finalize -> sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *zTail = NULL;
    char *expanded = NULL;
    void *testPointer = (void *)Data;
    const char *typeName = "FUZZ_TYPE";

    /* 
     * Step 1: Prepare the environment.
     * Open an in-memory database.
     */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    /* 
     * Initial sequence: sqlite3_step -> sqlite3_finalize -> sqlite3_finalize
     * We need a statement to step. Let's prepare a simple one first.
     * Ensure we only step/finalize if the statement was successfully created.
     */
    const char *initSql = "SELECT 1";
    if (sqlite3_prepare_v2(db, initSql, -1, &stmt1, NULL) == SQLITE_OK) {
        /* Invoke sqlite3_step on a valid statement */
        sqlite3_step(stmt1);
        /* Invoke sqlite3_finalize on stmt1 */
        sqlite3_finalize(stmt1);
        stmt1 = NULL; /* Ensure pointer is NULL for next call */
        /* Invoke sqlite3_finalize again (idempotent on NULL) */
        sqlite3_finalize(stmt1);
    } else {
        /* Even if prepare fails, we can try to finalize NULLs to test robustness */
        sqlite3_finalize(NULL);
        sqlite3_finalize(NULL);
    }

    /* 
     * Main fuzzing sequence:
     * sqlite3_prepare_v2 -> sqlite3_expanded_sql -> sqlite3_free -> 
     * sqlite3_bind_pointer -> sqlite3_step -> sqlite3_finalize -> sqlite3_finalize
     */

    /* 
     * Use the fuzz input as SQL. 
     * Ensure we don't pass a NULL pointer if Size is 0, though prepare handles it.
     * We treat the input as the SQL string.
     */
    const char *sqlInput = (Size > 0) ? (const char *)Data : "";
    
    /* Invoke sqlite3_prepare_v2 */
    if (sqlite3_prepare_v2(db, sqlInput, (int)Size, &stmt2, &zTail) == SQLITE_OK) {
        
        /* Invoke sqlite3_expanded_sql */
        expanded = sqlite3_expanded_sql(stmt2);
        
        /* Invoke sqlite3_free if expanded is not NULL */
        if (expanded != NULL) {
            sqlite3_free(expanded);
        }

        /* Invoke sqlite3_bind_pointer */
        /* Bind at index 1, assuming the SQL might have a placeholder, or just test the API */
        /* If the SQL has no placeholders, this might return SQLITE_ERROR, which is fine for fuzzing */
        sqlite3_bind_pointer(stmt2, 1, testPointer, typeName, NULL);

        /* Invoke sqlite3_step */
        sqlite3_step(stmt2);

        /* Invoke sqlite3_finalize */
        sqlite3_finalize(stmt2);
        stmt2 = NULL; /* Ensure pointer is NULL for next call */

        /* Invoke sqlite3_finalize again */
        sqlite3_finalize(stmt2);
    } else {
        /* 
         * If prepare failed, we still want to exercise the finalization logic 
         * on NULL pointers to ensure no crashes occur.
         */
        sqlite3_finalize(NULL);
        sqlite3_finalize(NULL);
    }

    /* Cleanup: Close the database connection */
    sqlite3_close(db);

    return 0;
}