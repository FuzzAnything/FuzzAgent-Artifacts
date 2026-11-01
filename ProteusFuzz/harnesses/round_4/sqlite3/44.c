// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:187769:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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

/* Static helper to avoid name conflicts */
static void dummy_destroy(void *p) {
    if (p) {
        free(p);
    }
}

static int dummy_collate(void *p, int n1, const void *s1, int n2, const void *s2) {
    (void)p;
    if (n1 == n2 && n1 > 0 && n2 > 0) {
        return memcmp(s1, s2, n1);
    }
    return n1 - n2;
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
}

static void dummy_value(sqlite3_context *ctx) {
    (void)ctx;
}

static void dummy_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Prepare environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have enough data to extract parameters */
    if (Size < 4) {
        sqlite3_close(db);
        return 0;
    }

    /* Extract parameters from fuzz input */
    int nArg = (int)(Data[0] & 0x7F) - 1; /* Range -1 to 126 */
    int eTextRep = (int)(Data[1] & 0x03); /* 0 to 3 */
    int funcType = (int)(Data[2] & 0x07); /* Select which API to call */
    int useDestroy = (Data[3] & 0x01);    /* Use destructor or not */

    /* Map eTextRep to valid constants */
    if (eTextRep == 0) eTextRep = SQLITE_UTF8;
    else if (eTextRep == 1) eTextRep = SQLITE_UTF16LE;
    else if (eTextRep == 2) eTextRep = SQLITE_UTF16BE;
    else if (eTextRep == 3) eTextRep = SQLITE_UTF16;

    /* Create a safe name buffer */
    char name[256];
    size_t nameLen = (Size > 255) ? 255 : Size - 3;
    if (nameLen > 0) {
        memcpy(name, Data + 3, nameLen);
        name[nameLen] = '\0';
    } else {
        strcpy(name, "fuzz_func");
    }

    /* Create a safe UTF-16 name buffer for sqlite3_create_collation16 */
    wchar_t *wName = NULL;
    if (nameLen > 0) {
        wName = (wchar_t *)malloc((nameLen + 1) * sizeof(wchar_t));
        if (wName) {
            for (size_t i = 0; i <= nameLen; i++) {
                wName[i] = (wchar_t)(name[i] & 0xFF);
            }
        }
    }

    /* Allocate dummy application data */
    void *pApp = NULL;
    if (Size > 10) {
        pApp = malloc(Size - 10);
        if (pApp) {
            memcpy(pApp, Data + 10, Size - 10);
        }
    }

    /* Invoke target functions based on funcType */
    switch (funcType) {
        case 0: {
            /* sqlite3_create_collation */
            int (*xCompare)(void *, int, const void *, int, const void *) = 
                (useDestroy && pApp) ? dummy_collate : NULL;
            rc = sqlite3_create_collation(db, name, eTextRep, pApp, xCompare);
            break;
        }
        case 1: {
            /* sqlite3_create_collation16 */
            int (*xCompare)(void *, int, const void *, int, const void *) = 
                (wName && pApp) ? dummy_collate : NULL;
            rc = sqlite3_create_collation16(db, wName, eTextRep, pApp, xCompare);
            break;
        }
        case 2: {
            /* sqlite3_create_collation_v2 */
            int (*xCompare)(void *, int, const void *, int, const void *) = 
                (pApp) ? dummy_collate : NULL;
            void (*xDestroy)(void *) = (useDestroy && pApp) ? dummy_destroy : NULL;
            rc = sqlite3_create_collation_v2(db, name, eTextRep, pApp, xCompare, xDestroy);
            break;
        }
        case 3: {
            /* sqlite3_create_function */
            void (*xFunc)(sqlite3_context *, int, sqlite3_value **) = (pApp) ? dummy_func : NULL;
            void (*xStep)(sqlite3_context *, int, sqlite3_value **) = (pApp) ? dummy_step : NULL;
            void (*xFinal)(sqlite3_context *) = (pApp) ? dummy_final : NULL;
            rc = sqlite3_create_function(db, name, nArg, eTextRep, pApp, xFunc, xStep, xFinal);
            break;
        }
        case 4: {
            /* sqlite3_create_function_v2 */
            void (*xFunc)(sqlite3_context *, int, sqlite3_value **) = (pApp) ? dummy_func : NULL;
            void (*xStep)(sqlite3_context *, int, sqlite3_value **) = (pApp) ? dummy_step : NULL;
            void (*xFinal)(sqlite3_context *) = (pApp) ? dummy_final : NULL;
            void (*xDestroy)(void *) = (useDestroy && pApp) ? dummy_destroy : NULL;
            rc = sqlite3_create_function_v2(db, name, nArg, eTextRep, pApp, xFunc, xStep, xFinal, xDestroy);
            break;
        }
        case 5: {
            /* sqlite3_create_window_function */
            void (*xStep)(sqlite3_context *, int, sqlite3_value **) = (pApp) ? dummy_step : NULL;
            void (*xFinal)(sqlite3_context *) = (pApp) ? dummy_final : NULL;
            void (*xValue)(sqlite3_context *) = (pApp) ? dummy_value : NULL;
            void (*xInverse)(sqlite3_context *, int, sqlite3_value **) = (pApp) ? dummy_inverse : NULL;
            void (*xDestroy)(void *) = (useDestroy && pApp) ? dummy_destroy : NULL;
            rc = sqlite3_create_window_function(db, name, nArg, eTextRep, pApp, xStep, xFinal, xValue, xInverse, xDestroy);
            break;
        }
        case 6: {
            /* Test with NULL name (should return SQLITE_MISUSE) */
            rc = sqlite3_create_function(db, NULL, nArg, eTextRep, pApp, dummy_func, NULL, NULL);
            break;
        }
        case 7: {
            /* Test with NULL callback to remove function */
            rc = sqlite3_create_function(db, name, nArg, eTextRep, pApp, NULL, NULL, NULL);
            break;
        }
        default:
            break;
    }

    /* Cleanup */
    if (wName) {
        free(wName);
    }
    if (pApp && !useDestroy) {
        /* If we didn't pass a destructor, we must free manually */
        free(pApp);
    }
    sqlite3_close(db);

    return 0;
}