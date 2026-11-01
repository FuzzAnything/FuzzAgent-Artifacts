// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:94395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:94049:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 APIs.
 * Order of invocation:
 * sqlite3_step -> sqlite3_finalize -> sqlite3_finalize ->
 * sqlite3_prepare_v2 -> sqlite3_expanded_sql -> sqlite3_free ->
 * sqlite3_bind_pointer -> sqlite3_step -> sqlite3_finalize -> sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *tail = NULL;
    char *expanded = NULL;
    int rc;

    /*
     * 1. Prepare environment: Open an in-memory database.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Create a dummy table to ensure we have something to query if needed.
     * This helps in exploring more states during sqlite3_step.
     */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * We need a prepared statement to call sqlite3_step first as per the required order.
     * We will prepare a simple statement, step it, then finalize it twice (safe to call on NULL).
     */
    const char *sql1 = "SELECT 1;";
    rc = sqlite3_prepare_v2(db, sql1, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * 2. Invoke target functions in the specified order.
     */

    /* sqlite3_step */
    rc = sqlite3_step(stmt1);
    (void)rc; /* Ignore return for fuzzing, just exercise the path */

    /* sqlite3_finalize (first time) */
    sqlite3_finalize(stmt1);
    stmt1 = NULL;

    /* sqlite3_finalize (second time, on NULL or already finalized is safe) */
    sqlite3_finalize(stmt1);

    /* sqlite3_prepare_v2 */
    /* Use fuzz input as SQL if possible, but ensure it's null-terminated for safety if we treat it as string.
     * However, sqlite3_prepare_v2 takes length, so we can pass raw data.
     * We limit the length to avoid huge allocations or invalid memory access if Size is too large.
     */
    int nByte = (Size > 10000) ? 10000 : (int)Size;
    rc = sqlite3_prepare_v2(db, (const char *)Data, nByte, &stmt2, &tail);
    
    /* If prepare fails, stmt2 is NULL. We can still proceed with the next steps safely. */
    
    /* sqlite3_expanded_sql */
    if (stmt2 != NULL) {
        expanded = sqlite3_expanded_sql(stmt2);
    } else {
        expanded = NULL;
    }

    /* sqlite3_free */
    if (expanded != NULL) {
        sqlite3_free(expanded);
        expanded = NULL;
    }

    /* sqlite3_bind_pointer */
    /* Bind a pointer to the first parameter if the statement is valid and has parameters.
     * We use a dummy pointer and type name.
     */
    if (stmt2 != NULL) {
        void *dummy_ptr = (void *)0x12345678;
        rc = sqlite3_bind_pointer(stmt2, 1, dummy_ptr, "fuzz_type", NULL);
        (void)rc;
    }

    /* sqlite3_step */
    if (stmt2 != NULL) {
        rc = sqlite3_step(stmt2);
        (void)rc;
    }

    /* sqlite3_finalize (first time) */
    sqlite3_finalize(stmt2);
    stmt2 = NULL;

    /* sqlite3_finalize (second time) */
    sqlite3_finalize(stmt2);

    /*
     * 3. Cleanup.
     */
    sqlite3_close(db);

    return 0;
}