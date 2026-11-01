// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper functions to avoid name conflicts.
 */

/* Dummy collation compare function */
static int dummy_collation_compare(void *pArg, int nLen1, const void *pStr1, int nLen2, const void *pStr2) {
    (void)pArg;
    if (nLen1 == 0 && nLen2 == 0) return 0;
    if (nLen1 == 0) return -1;
    if (nLen2 == 0) return 1;
    return memcmp(pStr1, pStr2, (nLen1 < nLen2 ? nLen1 : nLen2));
}

/* Dummy scalar function */
static void dummy_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 0);
}

/* Dummy aggregate step function */
static void dummy_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* No-op step */
}

/* Dummy aggregate final function */
static void dummy_aggregate_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_int(ctx, 0);
}

/* Dummy window value function */
static void dummy_window_value(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_int(ctx, 0);
}

/* Dummy window inverse function */
static void dummy_window_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* No-op inverse */
}

/*
 * Destructor for application data.
 * IMPORTANT: We use a flag to prevent double-free if SQLite calls this
 * and we also try to free it manually, or if SQLite calls it multiple times.
 * However, the standard pattern is to let SQLite manage the lifetime if xDestroy is passed.
 * If we pass xDestroy, we MUST NOT free pApp manually after the call.
 * If we do NOT pass xDestroy, we MUST free pApp manually.
 *
 * To fix the double-free:
 * 1. If we pass a non-NULL xDestroy to sqlite3_create_function_v2 or sqlite3_create_window_function,
 *    SQLite will call it when the function is deleted or the DB is closed.
 *    We should NOT free pApp ourselves in that case.
 * 2. If we pass NULL for xDestroy, we are responsible for freeing pApp.
 *
 * In the previous code, we passed dummy_destroy (which calls free) to sqlite3_create_function_v2
 * and sqlite3_create_window_function, but we also called free(pApp) at the end of the function
 * (implicitly or explicitly in logic not shown in the snippet but implied by the error trace
 * showing free called from line 57 which is inside dummy_destroy, and the error says double free).
 *
 * Wait, the error trace shows:
 * "freed by thread T0 here: ... dummy_destroy ... free(pApp)"
 * "attempting double-free ... dummy_destroy ... free(pApp)"
 *
 * This means dummy_destroy was called twice.
 * Why?
 * We called sqlite3_create_function_v2 with dummy_destroy.
 * Then we called sqlite3_create_window_function with dummy_destroy.
 * If the function name is the same ("fuzz_func"), the second call might override the first.
 * When the first function is overridden, SQLite calls the xDestroy of the old function.
 * Then when the DB is closed, SQLite calls the xDestroy of the current function.
 * If pApp is the same pointer for both, and we pass the same pApp, we get a double free.
 *
 * Fix:
 * 1. Do not pass xDestroy if we want to manage memory ourselves, OR
 * 2. Ensure we don't register the same function twice with the same pApp and xDestroy, OR
 * 3. Make the destructor idempotent (check if already freed), OR
 * 4. Allocate separate pApp for each registration if we use xDestroy.
 *
 * The safest approach for fuzzing to avoid complexity:
 * - Do not pass xDestroy to the create functions.
 * - Manage memory manually (free pApp at the end).
 * - This avoids SQLite calling free on our behalf.
 *
 * Let's modify the code to NOT pass xDestroy to the create functions.
 * We will free pApp manually at the end.
 */

static void dummy_destroy(void *pApp) {
    /* This function is no longer passed to SQLite to avoid double-free issues.
     * If we need to use it, we would need to ensure pApp is unique per registration
     * or use a flag. But simpler is to not use it. */
    if (pApp) {
        free(pApp);
    }
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *zFuncName = "fuzz_func";
    const char *zCollName = "fuzz_coll";
    const void *zFuncName16 = L"fuzz_func16";
    const void *zCollName16 = L"fuzz_coll16";
    int nArg = 0;
    int eTextRep = SQLITE_UTF8;
    void *pApp = NULL;

    /*
     * 1. Prepare the environment.
     * Open an in-memory database.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Use input data to vary parameters if Size is sufficient.
     * Ensure bounds checking before accessing Data array.
     */
    if (Size > 0) {
        nArg = Data[0] % 10; /* 0 to 9 arguments */
        if (Data[0] & 0x80) nArg = -1; /* Variable arguments */
        
        if (Size > 1) {
            eTextRep = (Data[1] % 5); /* 0 to 4: SQLITE_UTF8, UTF16LE, UTF16BE, UTF16, UTF16_ALIGNED */
            if (eTextRep == 0) eTextRep = SQLITE_UTF8;
            else if (eTextRep == 1) eTextRep = SQLITE_UTF16LE;
            else if (eTextRep == 2) eTextRep = SQLITE_UTF16BE;
            else if (eTextRep == 3) eTextRep = SQLITE_UTF16;
            else if (eTextRep == 4) eTextRep = SQLITE_UTF16_ALIGNED;
        }

        /* Allocate some dummy app data if requested */
        if (Size > 2 && Data[2] > 0) {
            size_t allocSize = (Size > 3 ? Data[3] : 10) % 1024;
            if (allocSize == 0) allocSize = 1;
            pApp = malloc(allocSize);
            if (pApp && Size > 4) {
                size_t copySize = (Size - 4);
                if (copySize > allocSize) copySize = allocSize;
                memcpy(pApp, Data + 4, copySize);
            }
        }
    }

    /*
     * 2. Invoke target functions diversely to explore program states.
     * IMPORTANT: We pass NULL for xDestroy to avoid double-free.
     * We will free pApp manually at the end of this function.
     */

    /* Test sqlite3_create_function */
    rc = sqlite3_create_function(
        db,
        zFuncName,
        nArg,
        eTextRep,
        pApp,
        dummy_scalar_func,
        NULL,
        NULL
    );
    (void)rc;

    /* Test sqlite3_create_function_v2 */
    /* Pass NULL for xDestroy */
    rc = sqlite3_create_function_v2(
        db,
        zFuncName,
        nArg,
        eTextRep,
        pApp,
        dummy_scalar_func,
        NULL,
        NULL,
        NULL /* xDestroy is NULL */
    );
    (void)rc;

    /* Test sqlite3_create_function16 */
    rc = sqlite3_create_function16(
        db,
        zFuncName16,
        nArg,
        eTextRep,
        pApp,
        dummy_scalar_func,
        NULL,
        NULL
    );
    (void)rc;

    /* Test sqlite3_create_collation */
    rc = sqlite3_create_collation(
        db,
        zCollName,
        eTextRep,
        pApp,
        dummy_collation_compare
    );
    (void)rc;

    /* Test sqlite3_create_collation16 */
    rc = sqlite3_create_collation16(
        db,
        zCollName16,
        eTextRep,
        pApp,
        dummy_collation_compare
    );
    (void)rc;

    /* Test sqlite3_create_window_function */
    /* Pass NULL for xDestroy */
    rc = sqlite3_create_window_function(
        db,
        zFuncName,
        nArg,
        eTextRep,
        pApp,
        dummy_aggregate_step,
        dummy_aggregate_final,
        dummy_window_value,
        dummy_window_inverse,
        NULL /* xDestroy is NULL */
    );
    (void)rc;

    /*
     * Test aggregate function registration specifically
     */
    rc = sqlite3_create_function(
        db,
        "fuzz_agg",
        nArg,
        eTextRep,
        NULL, /* No pApp for this one */
        NULL,
        dummy_aggregate_step,
        dummy_aggregate_final
    );
    (void)rc;

    /*
     * 3. Cleanup.
     * Free pApp manually since we did not pass xDestroy to SQLite.
     * Close the database connection.
     */
    if (pApp) {
        free(pApp);
        pApp = NULL;
    }

    sqlite3_close(db);

    return 0;
}