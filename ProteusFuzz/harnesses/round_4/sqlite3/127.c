// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper functions to avoid name conflicts */

/* Simple collation compare function */
static int collate_func(void *pArg, int n1, const void *s1, int n2, const void *s2) {
    (void)pArg;
    if (n1 == 0 && n2 == 0) return 0;
    if (n1 == 0) return -1;
    if (n2 == 0) return 1;
    return memcmp(s1, s2, (size_t)(n1 < n2 ? n1 : n2));
}

/* Simple collation compare function for UTF-16 */
static int collate_func16(void *pArg, int n1, const void *s1, int n2, const void *s2) {
    (void)pArg;
    if (n1 == 0 && n2 == 0) return 0;
    if (n1 == 0) return -1;
    if (n2 == 0) return 1;
    return memcmp(s1, s2, (size_t)(n1 < n2 ? n1 : n2));
}

/* Simple scalar function */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 42);
}

/* Simple step function for aggregate */
static void step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Accumulate logic would go here */
}

/* Simple final function for aggregate */
static void final_func(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 0);
}

/* Simple value function for window */
static void value_func(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 0);
}

/* Simple inverse function for window */
static void inverse_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
}

/* Destructor for app data - sets a flag to indicate it was called */
static void destroy_func(void *pApp) {
    if (pApp) {
        /* We do not free here because the caller will manage the lifecycle
           to avoid double-free if SQLite calls this.
           Instead, we just mark it or do nothing if we plan to free manually.
           However, the API contract says xDestroy is called when the function
           is deleted. If we pass xDestroy, SQLite takes ownership.
           To fuzz safely without double-free, we should NOT free in the destructor
           if we also free manually at the end, OR we should let SQLite free it.
           
           Strategy: Use a dummy destructor that does NOT free, and we free manually.
           OR: Use a destructor that frees, and we do NOT free manually.
           
           The error log shows SQLite called xDestroy (line 187729 in sqlite3.c)
           and then we called free(pApp) in the driver.
           
           Fix: Do not free pApp manually if we passed a non-NULL xDestroy.
           We will pass NULL for xDestroy in the fuzzing calls to avoid ownership transfer,
           OR we will ensure we don't free manually if xDestroy was used.
           
           Let's change the approach: 
           1. Pass NULL for xDestroy in sqlite3_create_function_v2 and sqlite3_create_window_function.
           2. Free pApp manually at the end.
           This avoids SQLite trying to free it.
        */
        (void)pApp;
    }
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have enough data to work with */
    if (Size < 2) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy file if needed */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Extract parameters from fuzz input */
    int nArg = (int)Data[0];
    int eTextRep = (int)Data[1];
    void *pApp = NULL;

    /* Allocate app data if needed */
    if (Size > 2) {
        size_t appSize = Size - 2;
        if (appSize > 1024) appSize = 1024; /* Limit size */
        pApp = malloc(appSize);
        if (pApp) {
            memcpy(pApp, Data + 2, appSize);
        }
    }

    /* Create function names from input data */
    char funcName[256];
    const char *zName = (const char *)Data;
    size_t nameLen = 0;
    while (nameLen < Size && Data[nameLen] != '\0' && nameLen < sizeof(funcName) - 1) {
        nameLen++;
    }
    if (nameLen == 0) nameLen = 1;
    memcpy(funcName, Data, nameLen);
    funcName[nameLen] = '\0';

    /* UTF-16 name buffer */
    wchar_t wName[256];
    size_t wNameLen = 0;
    for (size_t i = 0; i < nameLen && i < sizeof(wName) / sizeof(wName[0]) - 1; i++) {
        wName[i] = (wchar_t)Data[i];
        wNameLen++;
    }
    wName[wNameLen] = L'\0';

    /* Fuzz sqlite3_create_collation */
    rc = sqlite3_create_collation(db, funcName, eTextRep, pApp, collate_func);
    (void)rc;

    /* Fuzz sqlite3_create_collation16 */
    rc = sqlite3_create_collation16(db, wName, eTextRep, pApp, collate_func16);
    (void)rc;

    /* Fuzz sqlite3_create_function */
    rc = sqlite3_create_function(db, funcName, nArg, eTextRep, pApp, scalar_func, NULL, NULL);
    (void)rc;

    /* Fuzz sqlite3_create_function16 */
    rc = sqlite3_create_function16(db, wName, nArg, eTextRep, pApp, scalar_func, NULL, NULL);
    (void)rc;

    /* Fuzz sqlite3_create_function_v2 
       IMPORTANT: Pass NULL for xDestroy to prevent SQLite from freeing pApp.
       We will free pApp manually below.
    */
    rc = sqlite3_create_function_v2(db, funcName, nArg, eTextRep, pApp, scalar_func, step_func, final_func, NULL);
    (void)rc;

    /* Fuzz sqlite3_create_window_function
       IMPORTANT: Pass NULL for xDestroy to prevent SQLite from freeing pApp.
    */
    rc = sqlite3_create_window_function(db, funcName, nArg, eTextRep, pApp, step_func, final_func, value_func, inverse_func, NULL);
    (void)rc;

    /* Cleanup */
    if (pApp) {
        free(pApp);
    }
    sqlite3_close(db);

    return 0;
}