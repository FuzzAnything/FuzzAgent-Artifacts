// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
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
#include <stdio.h>

/*
 * Helper function to safely open a database connection.
 * Returns a valid sqlite3* or NULL.
 */
static sqlite3 *safe_open_db(void) {
    sqlite3 *db = NULL;
    /* Open an in-memory database to avoid file system dependencies */
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Helper function to create a dummy module for testing sqlite3_create_module
 * and sqlite3_drop_modules.
 */
static int dummy_vtab_create(sqlite3 *db, void *pAux,
                             int argc, const char *const *argv,
                             sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux,
                              int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

/* A minimal module structure for fuzzing */
static sqlite3_module g_dummy_module = {
    0, /* iVersion */
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
    NULL  /* xIntegrity */
};

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    sqlite3 *db = safe_open_db();
    if (!db) {
        return 0;
    }

    /* Ensure we have a valid module to test with */
    const sqlite3_module *pModule = &g_dummy_module;
    const char *zModuleName = "fuzz_dummy_module";
    void *pClientData = (void *)(uintptr_t)(Size % 1000); /* Fuzz client data */

    /* 2. Execute diverse API calls based on input data */
    
    /* Use input data to drive logic branches */
    unsigned int op = Size % 10;

    /* Test sqlite3_create_module */
    if (op < 3) {
        /* Register the module */
        int rc = sqlite3_create_module(db, zModuleName, pModule, pClientData);
        (void)rc; /* Suppress unused warning */
    } else if (op < 5) {
        /* Drop the module by passing NULL as the module pointer */
        int rc = sqlite3_create_module(db, zModuleName, NULL, NULL);
        (void)rc;
    }

    /* Test sqlite3_set_errmsg */
    if (Size > 2) {
        int errCode = (Data[0] % 100) + 100; /* Generate an error code */
        /* Create a safe error message from input, ensuring null termination */
        char errMsg[64];
        size_t msgLen = (Size > 64) ? 63 : Size - 1;
        memcpy(errMsg, Data + 1, msgLen);
        errMsg[msgLen] = '\0';

        int rc = sqlite3_set_errmsg(db, errCode, errMsg);
        (void)rc;

        /* Also test with NULL message */
        rc = sqlite3_set_errmsg(db, errCode, NULL);
        (void)rc;
    }

    /* Test sqlite3_errcode and sqlite3_errmsg */
    int errCode = sqlite3_errcode(db);
    const char *errMsg = sqlite3_errmsg(db);
    (void)errCode;
    (void)errMsg;

    /* Test sqlite3_drop_modules */
    if (Size > 4) {
        /* Create a keep list from input data if possible, or NULL */
        const char **azKeep = NULL;
        if (Data[3] % 2 == 0) {
            /* Keep the dummy module */
            static const char *keepList[] = { "fuzz_dummy_module", NULL };
            azKeep = keepList;
        }
        int rc = sqlite3_drop_modules(db, azKeep);
        (void)rc;
    }

    /* Test sqlite3_load_extension (Expected to fail safely as file doesn't exist) */
    /* We use a dummy file path as per instructions */
    const char *zFile = "./dummy_file";
    const char *zProc = NULL;
    char *pzErrMsg = NULL;
    
    /* Enable load extension to prevent immediate SQLITE_ERROR due to disabled feature */
    sqlite3_enable_load_extension(db, 1);
    
    int rc = sqlite3_load_extension(db, zFile, zProc, &pzErrMsg);
    if (pzErrMsg) {
        sqlite3_free(pzErrMsg);
        pzErrMsg = NULL;
    }
    (void)rc;

    /* 3. Cleanup */
    sqlite3_close(db);

    return 0;
}