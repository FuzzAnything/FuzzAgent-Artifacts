// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:161654:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Custom vtab structure to store the database connection */
typedef struct {
    sqlite3_vtab base;
    sqlite3 *db;
} dummy_vtab;

/* Custom cursor structure */
typedef struct {
    sqlite3_vtab_cursor base;
    int eof;
} dummy_vtab_cursor;

/* Forward declaration */
static const sqlite3_module dummy_module;

static int dummy_vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    dummy_vtab *pNew = (dummy_vtab *)sqlite3_malloc(sizeof(dummy_vtab));
    if (pNew == 0) return SQLITE_NOMEM;
    
    pNew->base.pModule = &dummy_module;
    pNew->db = db;
    
    *ppVTab = (sqlite3_vtab *)pNew;
    
    /* Test sqlite3_vtab_config */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    
    const char *zSQL = "CREATE TABLE x1(x1)";
    if (argc > 0 && argv[0] != NULL) {
        if (strlen(argv[0]) > 0 && strlen(argv[0]) < 1000) {
             if (strncmp(argv[0], "CREATE", 6) == 0) {
                 zSQL = argv[0];
             }
        }
    }
    
    return sqlite3_declare_vtab(db, zSQL);
}

static int dummy_vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                               sqlite3_vtab **ppVTab, char **pzErr) {
    dummy_vtab *pNew = (dummy_vtab *)sqlite3_malloc(sizeof(dummy_vtab));
    if (pNew == 0) return SQLITE_NOMEM;
    
    pNew->base.pModule = &dummy_module;
    pNew->db = db;
    
    *ppVTab = (sqlite3_vtab *)pNew;
    
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 0);
    return sqlite3_declare_vtab(db, "CREATE TABLE x1(x1)");
}

static int dummy_vtab_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_vtab_xDisconnect(sqlite3_vtab *pVTab) {
    dummy_vtab *p = (dummy_vtab *)pVTab;
    sqlite3_free(p);
    return SQLITE_OK;
}

static int dummy_vtab_xDestroy(sqlite3_vtab *pVTab) {
    dummy_vtab *p = (dummy_vtab *)pVTab;
    sqlite3_free(p);
    return SQLITE_OK;
}

static int dummy_vtab_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    dummy_vtab_cursor *pNew = (dummy_vtab_cursor *)sqlite3_malloc(sizeof(dummy_vtab_cursor));
    if (pNew == 0) return SQLITE_NOMEM;
    pNew->eof = 1;
    *ppCursor = (sqlite3_vtab_cursor *)pNew;
    return SQLITE_OK;
}

static int dummy_vtab_xClose(sqlite3_vtab_cursor *pCursor) {
    dummy_vtab_cursor *p = (dummy_vtab_cursor *)pCursor;
    sqlite3_free(p);
    return SQLITE_OK;
}

static int dummy_vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                              int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_vtab_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1; /* Always EOF */
}

static int dummy_vtab_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int iCol) {
    /* Test sqlite3_vtab_nochange */
    sqlite3_vtab_nochange(pCtx);
    sqlite3_result_text(pCtx, "dummy", -1, SQLITE_STATIC);
    return SQLITE_OK;
}

static int dummy_vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 1;
    return SQLITE_OK;
}

static int dummy_vtab_xUpdate(sqlite3_vtab *pVTab, int nArg, sqlite3_value **argv,
                              sqlite3_int64 *pRowid) {
    dummy_vtab *p = (dummy_vtab *)pVTab;
    /* Test sqlite3_vtab_on_conflict with the stored db pointer */
    sqlite3_vtab_on_conflict(p->db);
    return SQLITE_OK;
}

static int dummy_vtab_xBegin(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xSync(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xCommit(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xRollback(sqlite3_vtab *pVTab) { return SQLITE_OK; }
static int dummy_vtab_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                                    void (**pxFunc)(sqlite3_context *, int, sqlite3_value **),
                                    void **ppArg) { return SQLITE_OK; }
static int dummy_vtab_xRename(sqlite3_vtab *pVtab, const char *zNew) { return SQLITE_OK; }
static int dummy_vtab_xSavepoint(sqlite3_vtab *pVTab, int) { return SQLITE_OK; }
static int dummy_vtab_xRelease(sqlite3_vtab *pVTab, int) { return SQLITE_OK; }
static int dummy_vtab_xRollbackTo(sqlite3_vtab *pVTab, int) { return SQLITE_OK; }
static int dummy_vtab_xShadowName(const char *) { return SQLITE_OK; }
static int dummy_vtab_xIntegrity(sqlite3_vtab *pVTab, const char *zSchema,
                                 const char *zTabName, int mFlags, char **pzErr) { return SQLITE_OK; }

/* Define the module after all function pointers are defined */
static const sqlite3_module dummy_module = {
    0, /* iVersion */
    dummy_vtab_xCreate,
    dummy_vtab_xConnect,
    dummy_vtab_xBestIndex,
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
    dummy_vtab_xFindFunction,
    dummy_vtab_xRename,
    dummy_vtab_xSavepoint,
    dummy_vtab_xRelease,
    dummy_vtab_xRollbackTo,
    dummy_vtab_xShadowName,
    dummy_vtab_xIntegrity
};

static void dummy_destroy(void *p) {
    /* Optional destructor */
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* 1. Fuzz sqlite3_create_module and sqlite3_create_module_v2 */
    char module_name[256];
    size_t name_len = (Size > 255) ? 255 : Size;
    memcpy(module_name, Data, name_len);
    module_name[name_len] = '\0';
    
    if (name_len == 0) {
        module_name[0] = 't';
        module_name[1] = 'e';
        module_name[2] = 's';
        module_name[3] = 't';
        module_name[4] = '\0';
    }

    /* Register module */
    sqlite3_create_module(db, module_name, &dummy_module, NULL);
    sqlite3_create_module_v2(db, module_name, &dummy_module, (void*)0x1234, dummy_destroy);

    /* 2. Create a virtual table to trigger xConnect/xCreate internally */
    char sql[512];
    int sql_len = snprintf(sql, sizeof(sql), "CREATE VIRTUAL TABLE test_vtab USING %s", module_name);
    if (sql_len < 0 || sql_len >= (int)sizeof(sql)) {
        sqlite3_close(db);
        return 0;
    }

    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }

    /* 3. Trigger xUpdate to test sqlite3_vtab_on_conflict */
    const char *update_sql = "UPDATE test_vtab SET x1 = 1";
    rc = sqlite3_exec(db, update_sql, NULL, NULL, NULL);

    /* 4. Trigger xColumn to test sqlite3_vtab_nochange */
    const char *select_sql = "SELECT x1 FROM test_vtab";
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_column_text(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    /* 5. Drop the module */
    sqlite3_create_module(db, module_name, NULL, NULL);

    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}