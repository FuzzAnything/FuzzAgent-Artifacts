// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

/*
 * Static helper to create a minimal dummy virtual table module.
 */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 1);
    return sqlite3_declare_vtab(db, "CREATE TABLE x(id INTEGER PRIMARY KEY, val TEXT)");
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 0);
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);
    return sqlite3_declare_vtab(db, "CREATE TABLE x(id INTEGER PRIMARY KEY, val TEXT)");
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xDestroy(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = NULL;
    return SQLITE_OK;
}
static int dummy_xClose(sqlite3_vtab_cursor *pCursor) { return SQLITE_OK; }
static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}
static int dummy_xNext(sqlite3_vtab_cursor *pCursor) { return SQLITE_DONE; }
static int dummy_xEof(sqlite3_vtab_cursor *pCursor) { return 1; }
static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int iCol) {
    sqlite3_result_null(pCtx);
    return SQLITE_OK;
}
static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}
static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}
static int dummy_xBegin(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xSync(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xCommit(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xRollback(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_xSavepoint(sqlite3_vtab *pVTab, int iSavepoint) { return SQLITE_OK; }
static int dummy_xRelease(sqlite3_vtab *pVTab, int iSavepoint) { return SQLITE_OK; }
static int dummy_xRollbackTo(sqlite3_vtab *pVTab, int iSavepoint) { return SQLITE_OK; }

static const sqlite3_module dummy_module = {
    3,
    dummy_xCreate,
    dummy_xConnect,
    NULL,
    dummy_xDisconnect,
    dummy_xDestroy,
    dummy_xOpen,
    dummy_xClose,
    dummy_xFilter,
    dummy_xNext,
    dummy_xEof,
    dummy_xColumn,
    dummy_xRowid,
    dummy_xUpdate,
    dummy_xBegin,
    dummy_xSync,
    dummy_xCommit,
    dummy_xRollback,
    NULL,
    NULL,
    dummy_xSavepoint,
    dummy_xRelease,
    dummy_xRollbackTo,
    NULL,
    NULL
};

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    if (Size == 0) return 0;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    unsigned char op = Data[0] % 10;

    switch (op) {
        case 0: {
            const char *name = (Size > 1) ? (const char *)&Data[1] : "dummy_mod";
            char safe_name[64];
            size_t len = (Size > 1) ? (Size - 1) : 0;
            if (len >= sizeof(safe_name)) len = sizeof(safe_name) - 1;
            memcpy(safe_name, name, len);
            safe_name[len] = '\0';

            rc = sqlite3_create_module(db, safe_name, &dummy_module, NULL);
            if (rc != SQLITE_OK) {
                sqlite3_create_module(db, safe_name, NULL, NULL);
                sqlite3_create_module(db, safe_name, &dummy_module, NULL);
            }
            break;
        }
        case 1: {
            const char *name = (Size > 1) ? (const char *)&Data[1] : "dummy_mod_v2";
            char safe_name[64];
            size_t len = (Size > 1) ? (Size - 1) : 0;
            if (len >= sizeof(safe_name)) len = sizeof(safe_name) - 1;
            memcpy(safe_name, name, len);
            safe_name[len] = '\0';

            rc = sqlite3_create_module_v2(db, safe_name, &dummy_module, NULL, NULL);
            if (rc != SQLITE_OK) {
                sqlite3_create_module_v2(db, safe_name, NULL, NULL, NULL);
                sqlite3_create_module_v2(db, safe_name, &dummy_module, NULL, NULL);
            }
            break;
        }
        case 2: {
            rc = sqlite3_drop_modules(db, NULL);
            break;
        }
        case 3: {
            const char *keep_list[] = { "dummy_mod", NULL };
            rc = sqlite3_drop_modules(db, keep_list);
            break;
        }
        case 4: {
            const char *sql = "CREATE TABLE test(a INTEGER)";
            rc = sqlite3_declare_vtab(db, sql);
            break;
        }
        case 5: {
            rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
            break;
        }
        case 6: {
            sqlite3_index_info info;
            memset(&info, 0, sizeof(info));
            const char *coll = sqlite3_vtab_collation(&info, 0);
            (void)coll;
            break;
        }
        case 7: {
            const char *name = "temp_mod";
            sqlite3_create_module(db, name, &dummy_module, NULL);
            sqlite3_drop_modules(db, NULL);
            break;
        }
        case 8: {
            char name1[16], name2[16];
            unsigned int val1 = 0;
            unsigned int val2 = 0;
            
            if (Size > 1) val1 = Data[1] & 0xFF;
            if (Size > 2) val2 = Data[2] & 0xFF;

            snprintf(name1, sizeof(name1), "mod_%u", val1);
            snprintf(name2, sizeof(name2), "mod_%u", val2);
            
            sqlite3_create_module(db, name1, &dummy_module, NULL);
            sqlite3_create_module(db, name2, &dummy_module, NULL);
            const char *keep[] = { name1, NULL };
            sqlite3_drop_modules(db, keep);
            break;
        }
        case 9: {
            const char *name = "drop_test";
            sqlite3_create_module(db, name, NULL, NULL);
            break;
        }
        default:
            break;
    }

    sqlite3_close(db);
    return 0;
}