// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to create a dummy virtual table module for testing */
static int dummy_vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    /* Test sqlite3_vtab_config and sqlite3_declare_vtab inside xCreate */
    if (argc > 0 && argv[0] && argv[0][0] == 'c') {
        sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    }
    return sqlite3_declare_vtab(db, "CREATE TABLE t1(a TEXT, b INT)");
}

static int dummy_vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                               sqlite3_vtab **ppVTab, char **pzErr) {
    /* Test sqlite3_vtab_config inside xConnect */
    if (argc > 0 && argv[0] && argv[0][0] == 'c') {
        sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    }
    return sqlite3_declare_vtab(db, "CREATE TABLE t1(a TEXT, b INT)");
}

static int dummy_vtab_xDisconnect(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xDestroy(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = NULL;
    return SQLITE_OK;
}
static int dummy_vtab_xClose(sqlite3_vtab_cursor *pCursor) { return SQLITE_OK; }
static int dummy_vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                              int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}
static int dummy_vtab_xNext(sqlite3_vtab_cursor *pCursor) { return SQLITE_DONE; }
static int dummy_vtab_xEof(sqlite3_vtab_cursor *pCursor) { return 1; }

static int dummy_vtab_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int iCol) {
    /* Test sqlite3_vtab_nochange inside xColumn */
    int nochange = sqlite3_vtab_nochange(pCtx);
    if (nochange) {
        /* Optimization: do not set a value, rely on xUpdate to detect nochange */
        return SQLITE_OK;
    }
    sqlite3_result_text(pCtx, "dummy", -1, SQLITE_STATIC);
    return SQLITE_OK;
}

static int dummy_vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 1;
    return SQLITE_OK;
}

static int dummy_vtab_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv,
                              sqlite3_int64 *pRowid) {
    /* Test sqlite3_vtab_on_conflict inside xUpdate */
    /* Note: We cannot easily get the sqlite3* handle here without storing it in pVTab.
       However, the function requires a sqlite3* argument.
       Since we cannot safely extract it from the opaque pVTab without internal knowledge,
       we will skip calling sqlite3_vtab_on_conflict in this specific dummy implementation
       to avoid undefined behavior or crashes, as the context is not fully available.
       The fuzzer will still exercise the code path by invoking the module registration.
       To properly test sqlite3_vtab_on_conflict, a real UPDATE statement must be executed
       against the virtual table, which triggers this callback.
    */
    return SQLITE_OK;
}

static int dummy_vtab_xBegin(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xSync(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xCommit(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xRollback(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xSavepoint(sqlite3_vtab *pVTab, int iSavepoint) { return SQLITE_OK; }
static int dummy_vtab_xRelease(sqlite3_vtab *pVTab, int iSavepoint) { return SQLITE_OK; }
static int dummy_vtab_xRollbackTo(sqlite3_vtab *pVTab, int iSavepoint) { return SQLITE_OK; }

static const sqlite3_module dummy_module = {
    0,
    dummy_vtab_xCreate,
    dummy_vtab_xConnect,
    NULL, /* xBestIndex */
    dummy_vtab_xDisconnect,
    dummy_vtab_xDestroy,
    dummy_vtab_xOpen,
    dummy_vtab_xClose,
    dummy_vtab_xFilter,
    dummy_vtab_xNext,
    dummy_vtab_xEof,
    dummy_vtab_xColumn,
    dummy_vtab_xRowid,
    dummy_vtab_xUpdate,
    dummy_vtab_xBegin,
    dummy_vtab_xSync,
    dummy_vtab_xCommit,
    dummy_vtab_xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    dummy_vtab_xSavepoint,
    dummy_vtab_xRelease,
    dummy_vtab_xRollbackTo,
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

/* Destructor for client data */
static void dummy_client_data_destroy(void *p) {
    if (p) {
        free(p);
    }
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *zName = "dummy_module";
    
    /* 1. Prepare the environment */
    if (Size < 1) return 0;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 2. Fuzzing logic */
    
    /* Use input data to decide which API to call and with what parameters */
    uint8_t op = Data[0] % 5;
    
    /* Prepare client data for v2 */
    void *pClientData = NULL;
    /* Ensure we have enough data to copy before allocating */
    if (Size > 1 && Data[1] % 2 == 0) {
        size_t len = Size - 1; /* Data starts at index 1, so available bytes is Size-1 */
        if (len > 10) len = 10;
        
        if (len > 0) {
            pClientData = malloc(len);
            if (pClientData) {
                memcpy(pClientData, Data + 1, len);
            }
        }
    }

    switch (op) {
        case 0:
            /* Test sqlite3_create_module */
            rc = sqlite3_create_module(db, zName, &dummy_module, pClientData);
            break;
        case 1:
            /* Test sqlite3_create_module_v2 */
            rc = sqlite3_create_module_v2(db, zName, &dummy_module, pClientData, dummy_client_data_destroy);
            break;
        case 2:
            /* Test dropping a module (NULL pointer for module) */
            rc = sqlite3_create_module(db, zName, NULL, NULL);
            break;
        case 3:
            /* Test with a potentially malicious name from input (if safe) */
            /* Ensure null termination */
            char zNameBuf[256];
            size_t nameLen = Size > 255 ? 255 : Size;
            if (nameLen > 0) {
                memcpy(zNameBuf, Data, nameLen);
                zNameBuf[nameLen] = '\0';
                /* Remove null bytes in the middle to ensure valid string */
                for (size_t i = 0; i < nameLen; i++) {
                    if (zNameBuf[i] == 0) zNameBuf[i] = 'x';
                }
                rc = sqlite3_create_module(db, zNameBuf, &dummy_module, pClientData);
            }
            break;
        case 4:
            /* Attempt to create a virtual table to trigger xCreate/xConnect */
            /* This indirectly tests sqlite3_declare_vtab and sqlite3_vtab_config */
            /* and potentially sqlite3_vtab_nochange and sqlite3_vtab_on_conflict */
            /* if we execute an UPDATE. */
            const char *createSQL = "CREATE VIRTUAL TABLE test_vtab USING dummy_module;";
            const char *updateSQL = "UPDATE test_vtab SET a='new_val' WHERE 1=1;";
            
            rc = sqlite3_exec(db, createSQL, NULL, NULL, NULL);
            if (rc == SQLITE_OK) {
                /* Execute an update to trigger xUpdate and potentially xColumn with nochange hint */
                rc = sqlite3_exec(db, updateSQL, NULL, NULL, NULL);
            }
            break;
    }

    /* 3. Cleanup */
    /* Note: If op==1, sqlite3_create_module_v2 takes ownership of pClientData and will free it via destructor.
       If op!=1 and we allocated pClientData, we must free it here. */
    if (pClientData && op != 1) {
        free(pClientData);
    }
    
    sqlite3_close(db);
    
    return 0;
}