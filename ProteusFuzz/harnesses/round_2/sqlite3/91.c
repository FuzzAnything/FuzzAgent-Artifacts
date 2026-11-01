// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:187786:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper functions to avoid name conflicts and keep the fuzzer clean.
 */

/* Dummy scalar function implementation */
static void dummyFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 0);
}

/* Dummy aggregate step implementation */
static void dummyStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

/* Dummy aggregate final implementation */
static void dummyFinal(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_int(ctx, 0);
}

/* Dummy window value implementation */
static void dummyValue(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_int(ctx, 0);
}

/* Dummy window inverse implementation */
static void dummyInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

/* Dummy destructor for pApp */
static void dummyDestroy(void *pApp) {
    if (pApp) {
        free(pApp);
    }
}

/* Dummy collation compare implementation */
static int dummyCompare(void *pArg, int nLenA, const void *zA, int nLenB, const void *zB) {
    (void)pArg;
    if (nLenA == nLenB && memcmp(zA, zB, nLenA) == 0) {
        return 0;
    }
    return (nLenA < nLenB) ? -1 : 1;
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file I/O issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have some data to work with */
    if (Size == 0) {
        sqlite3_close(db);
        return 0;
    }

    /* Use input data to drive fuzzing parameters */
    int nArg = (Data[0] % 10) - 1; /* Range: -1 to 8 */
    int eTextRep = SQLITE_UTF8;
    if (Size > 1) {
        int encChoice = Data[1] % 5;
        if (encChoice == 0) eTextRep = SQLITE_UTF8;
        else if (encChoice == 1) eTextRep = SQLITE_UTF16LE;
        else if (encChoice == 2) eTextRep = SQLITE_UTF16BE;
        else if (encChoice == 3) eTextRep = SQLITE_UTF16;
        else eTextRep = SQLITE_UTF16_ALIGNED;
    }

    /* Allocate some dummy application data for pApp */
    void *pApp = NULL;
    if (Size > 2 && (Data[2] % 2) == 1) {
        size_t appSize = (Size > 10) ? (Data[3] % (Size - 10) + 1) : 1;
        pApp = malloc(appSize);
        if (pApp) {
            /* Ensure we don't read past Data end */
            size_t copyLen = (Size > 4) ? appSize : 0;
            if (copyLen > Size - 4) copyLen = Size - 4;
            memcpy(pApp, Data + 4, copyLen);
        }
    }

    /* Create a dummy function name from input data */
    char funcName[256];
    size_t nameLen = (Size > 5) ? (Data[5] % 100) : 10;
    if (nameLen > sizeof(funcName) - 1) nameLen = sizeof(funcName) - 1;
    
    /* Ensure we don't read past Data end */
    size_t availableData = (Size > 6) ? (Size - 6) : 0;
    if (nameLen > availableData) nameLen = availableData;
    
    if (nameLen > 0) {
        memcpy(funcName, Data + 6, nameLen);
    }
    funcName[nameLen] = '\0';

    /* Create a dummy UTF-16 function name for create_function16 */
    wchar_t *funcName16 = NULL;
    if (Size > 10) {
        size_t wLen = nameLen + 1;
        funcName16 = (wchar_t *)malloc(wLen * sizeof(wchar_t));
        if (funcName16) {
            for (size_t j = 0; j < nameLen; j++) {
                funcName16[j] = (wchar_t)(Data[6 + j] & 0x7F); /* Simple ASCII to UTF-16 */
            }
            funcName16[nameLen] = L'\0';
        }
    }

    /*
     * 2. & 3. Invoke target functions diversely
     */

    /* Test sqlite3_create_function */
    if (Size > 0) {
        rc = sqlite3_create_function(
            db,
            funcName,
            nArg,
            eTextRep,
            pApp,
            dummyFunc,
            NULL,
            NULL
        );
        (void)rc; /* Ignore return for fuzzing stability */
    }

    /* Test sqlite3_create_function_v2 */
    if (Size > 1) {
        void *v2App = NULL;
        if (Data[1] % 2) {
            v2App = malloc(1);
        }
        rc = sqlite3_create_function_v2(
            db,
            funcName,
            nArg,
            eTextRep,
            v2App,
            dummyFunc,
            NULL,
            NULL,
            dummyDestroy
        );
        (void)rc;
    }

    /* Test sqlite3_create_function16 */
    if (funcName16 && Size > 2) {
        rc = sqlite3_create_function16(
            db,
            funcName16,
            nArg,
            eTextRep,
            pApp,
            dummyFunc,
            NULL,
            NULL
        );
        (void)rc;
    }

    /* Test sqlite3_create_window_function */
    if (Size > 3) {
        void *winApp = NULL;
        if (Data[3] % 2) {
            winApp = malloc(1);
        }
        rc = sqlite3_create_window_function(
            db,
            funcName,
            nArg,
            eTextRep,
            winApp,
            dummyStep,
            dummyFinal,
            dummyValue,
            dummyInverse,
            dummyDestroy
        );
        (void)rc;
    }

    /* Test sqlite3_create_collation */
    if (Size > 4) {
        char collName[64];
        size_t collLen = (Data[4] % 30) + 1;
        if (collLen > sizeof(collName) - 1) collLen = sizeof(collName) - 1;
        
        size_t availableCollData = (Size > 5) ? (Size - 5) : 0;
        if (collLen > availableCollData) collLen = availableCollData;

        if (collLen > 0) {
            memcpy(collName, Data + 5, collLen);
        }
        collName[collLen] = '\0';

        rc = sqlite3_create_collation(
            db,
            collName,
            eTextRep,
            pApp,
            dummyCompare
        );
        (void)rc;
    }

    /* Test sqlite3_create_collation16 */
    if (funcName16 && Size > 5) {
        rc = sqlite3_create_collation16(
            db,
            funcName16,
            eTextRep,
            pApp,
            dummyCompare
        );
        (void)rc;
    }

    /*
     * 4. Cleanup
     */
    if (funcName16) {
        free(funcName16);
    }
    /* Note: pApp might be freed by destructors if registered with v2. 
       In this fuzzer, we pass separate pointers or NULL to avoid double-free.
       We do NOT free pApp here to be safe.
    */

    sqlite3_close(db);

    return 0;
}