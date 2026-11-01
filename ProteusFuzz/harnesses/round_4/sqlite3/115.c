// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Dummy virtual table module implementation for fuzzing */
static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                             sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
    *ppVTab = NULL;
    return SQLITE_OK;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
    *ppVTab = NULL;
    return SQLITE_OK;
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

/* Dummy module structure */
static sqlite3_module g_dummy_module = {
    3, /* iVersion */
    dummy_vtab_create,
    dummy_vtab_connect,
    NULL, /* xBestIndex */
    dummy_vtab_disconnect,
    dummy_vtab_destroy,
    NULL, /* xOpen */
    NULL, /* xClose */
    NULL, /* xFilter */
    NULL, /* xNext */
    NULL, /* xEof */
    NULL, /* xColumn */
    NULL, /* xRowid */
    NULL, /* xUpdate */
    NULL, /* xBegin */
    NULL, /* xSync */
    NULL, /* xCommit */
    NULL, /* xRollback */
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL, /* xIntegrity */
};

/* Dummy destructor for client data */
static void dummy_destructor(void *p) {
    (void)p;
}

/* Dummy collation compare function */
static int dummy_collation_compare(void *pArg, int nLenA, const void *pA, int nLenB, const void *pB) {
    (void)pArg;
    if (nLenA == nLenB && nLenA > 0 && pA != NULL && pB != NULL) {
        return memcmp(pA, pB, nLenA);
    }
    return (nLenA < nLenB) ? -1 : ((nLenA > nLenB) ? 1 : 0);
}

/* Dummy collation needed callback */
static void dummy_collation_needed(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    (void)eTextRep;
    (void)zName;
    if (db && zName) {
        sqlite3_create_collation(db, zName, SQLITE_UTF8, NULL, dummy_collation_compare);
    }
}

/* Dummy SQL function */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
    sqlite3_result_null(ctx);
}

/* Dummy aggregate step */
static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
}

/* Dummy aggregate final */
static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have enough data to work with */
    if (Size < 4) {
        sqlite3_close(db);
        return 0;
    }

    /* Use parts of the input to drive fuzzing logic */
    size_t offset = 0;
    
    /* Extract a name for the module/collation/function */
    /* We manually find the null terminator to avoid strlen on non-terminated buffer */
    const char *name = NULL;
    size_t name_len = 0;
    if (offset < Size) {
        size_t i = 0;
        while (offset + i < Size && Data[offset + i] != '\0') {
            i++;
        }
        if (offset + i < Size) {
            name = (const char *)(Data + offset);
            name_len = i;
            offset += i + 1; /* Skip null terminator */
        } else {
            /* No null terminator found, skip this part */
            offset = Size;
        }
    }

    /* Extract a name for client data key */
    const char *key = NULL;
    size_t key_len = 0;
    if (offset < Size) {
        size_t i = 0;
        while (offset + i < Size && Data[offset + i] != '\0') {
            i++;
        }
        if (offset + i < Size) {
            key = (const char *)(Data + offset);
            key_len = i;
            offset += i + 1; /* Skip null terminator */
        } else {
            offset = Size;
        }
    }

    /* Allocate some dummy client data */
    void *client_data = NULL;
    if (offset < Size) {
        size_t data_size = Data[offset];
        if (data_size > 0 && data_size < 1024) {
            if (offset + 1 + data_size <= Size) {
                client_data = malloc(data_size);
                if (client_data) {
                    memcpy(client_data, Data + offset + 1, data_size);
                    offset += data_size + 1;
                }
            }
        }
    }

    /* 2. Fuzz Target API Functions */

    /* Target 1: sqlite3_create_module_v2 */
    if (name != NULL && name_len > 0) {
        sqlite3_create_module_v2(db, name, &g_dummy_module, client_data, dummy_destructor);
    }

    /* Target 2: sqlite3_collation_needed */
    sqlite3_collation_needed(db, client_data, dummy_collation_needed);

    /* Target 3: sqlite3_create_collation */
    if (name != NULL && name_len > 0) {
        int enc = SQLITE_UTF8;
        if (offset < Size) {
            int val = Data[offset];
            if (val == 1) enc = SQLITE_UTF16LE;
            else if (val == 2) enc = SQLITE_UTF16BE;
        }
        sqlite3_create_collation(db, name, enc, client_data, dummy_collation_compare);
    }

    /* Target 4: sqlite3_set_clientdata */
    if (key != NULL && key_len > 0) {
        sqlite3_set_clientdata(db, key, client_data, dummy_destructor);
    }

    /* Target 5: sqlite3_create_collation_v2 */
    if (name != NULL && name_len > 0) {
        int enc = SQLITE_UTF8;
        if (offset < Size) {
            int val = Data[offset];
            if (val == 1) enc = SQLITE_UTF16LE;
            else if (val == 2) enc = SQLITE_UTF16BE;
        }
        sqlite3_create_collation_v2(db, name, enc, client_data, dummy_collation_compare, dummy_destructor);
    }

    /* Target 6: sqlite3_create_function_v2 */
    if (name != NULL && name_len > 0) {
        int nArg = 0;
        int enc = SQLITE_UTF8;
        if (offset < Size) nArg = Data[offset];
        if (offset + 1 < Size) {
            int val = Data[offset + 1];
            if (val == 1) enc = SQLITE_UTF16LE;
            else if (val == 2) enc = SQLITE_UTF16BE;
        }

        /* Try scalar function */
        sqlite3_create_function_v2(db, name, nArg, enc, client_data, dummy_func, NULL, NULL, dummy_destructor);
        
        /* Try aggregate function */
        sqlite3_create_function_v2(db, name, nArg, enc, client_data, NULL, dummy_step, dummy_final, dummy_destructor);
    }

    /* 3. Cleanup */
    sqlite3_close(db);
    
    /* Free allocated client data if it was malloc'd */
    if (client_data) {
        free(client_data);
    }

    return 0;
}