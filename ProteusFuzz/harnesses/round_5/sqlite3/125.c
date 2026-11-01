// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Static helper to safely open a database connection.
 * Returns NULL on failure.
 */
static sqlite3 *open_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Static helper to create a dummy virtual table module.
 * This is necessary because the target APIs (vtab_collation, vtab_distinct, etc.)
 * are only valid when called from within virtual table methods (xBestIndex, xConnect, etc.).
 * To fuzz them, we must register a module and trigger these callbacks.
 */
static int create_dummy_module(sqlite3 *db) {
    static int module_created = 0;
    if (module_created) return SQLITE_OK;

    sqlite3_module mod = {0};
    mod.iVersion = 3;
    mod.xCreate = NULL;
    mod.xConnect = NULL;
    mod.xBestIndex = NULL; /* We will not implement a full xBestIndex to avoid complex logic, 
                             but we can trigger the APIs if we call them manually in a controlled way.
                             However, the documentation says these APIs MUST be called from within xBestIndex.
                             Calling them outside is undefined behavior.
                             
                             To fuzz safely, we will create a simple module that does nothing,
                             and then try to call the APIs in a way that might trigger internal checks
                             or by simulating the context if possible. 
                             
                             Actually, the safest way to fuzz these specific APIs which are context-sensitive
                             is to register the module and then attempt to call them with invalid/null
                             pointers or malformed structures to test error handling, 
                             OR to implement a minimal xBestIndex that calls these functions.
                             
                             Let's implement a minimal xBestIndex that calls the target functions.
                             This ensures we are in the correct context.
                        */
    
    /* We need a structure to hold the module state if needed, but for now static is fine. */
    return SQLITE_OK;
}

/* 
 * A dummy xBestIndex implementation that calls the target vtab APIs.
 * This allows us to fuzz the APIs in their valid context.
 */
static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    /* 
     * We cannot easily construct a valid sqlite3_index_info from fuzz input 
     * because it contains pointers to arrays that SQLite manages.
     * However, we can try to call the APIs with the pIdxInfo passed by SQLite.
     * To fuzz effectively, we need to generate queries that trigger xBestIndex 
     * with various constraints.
     * 
     * Since we are writing a fuzzer, we will generate SQL queries that 
     * exercise the virtual table. The fuzz driver will create the DB, 
     * register the module, create the table, and then execute random SQL.
     * The xBestIndex will be called by SQLite internally.
     */
    
    /* Call sqlite3_vtab_distinct */
    if (pIdxInfo) {
        int distinct_val = sqlite3_vtab_distinct(pIdxInfo);
        (void)distinct_val; /* Suppress unused warning */
    }

    /* Call sqlite3_vtab_collation if constraints exist */
    if (pIdxInfo && pIdxInfo->aConstraint && pIdxInfo->nConstraint > 0) {
        /* Try to get collation for the first constraint */
        const char *coll = sqlite3_vtab_collation(pIdxInfo, 0);
        (void)coll;
    }

    /* Call sqlite3_vtab_rhs_value if constraints exist */
    if (pIdxInfo && pIdxInfo->aConstraint && pIdxInfo->nConstraint > 0) {
        sqlite3_value *val = NULL;
        int rc = sqlite3_vtab_rhs_value(pIdxInfo, 0, &val);
        (void)rc;
        (void)val;
    }

    /* Call sqlite3_vtab_in if constraints exist */
    if (pIdxInfo && pIdxInfo->aConstraint && pIdxInfo->nConstraint > 0) {
        /* Check if it's an IN operator */
        int is_in = sqlite3_vtab_in(pIdxInfo, 0, -1);
        (void)is_in;
        
        /* Request handling */
        int handled = sqlite3_vtab_in(pIdxInfo, 0, 1);
        (void)handled;
    }

    /* Set some dummy output to avoid errors */
    if (pIdxInfo) {
        pIdxInfo->idxNum = 0;
        pIdxInfo->estimatedCost = 100.0;
        pIdxInfo->estimatedRows = 10;
    }
    return SQLITE_OK;
}

/* Dummy xConnect */
static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                          sqlite3_vtab **ppVtab, char **pzErr) {
    /* Call sqlite3_vtab_config here as it's valid in xConnect */
    int rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    (void)rc;

    /* Allocate a dummy vtab structure */
    sqlite3_vtab *vtab = sqlite3_malloc(sizeof(sqlite3_vtab));
    if (!vtab) {
        *pzErr = "Out of memory";
        return SQLITE_NOMEM;
    }
    memset(vtab, 0, sizeof(sqlite3_vtab));
    *ppVtab = vtab;
    return SQLITE_OK;
}

/* Dummy xDisconnect */
static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

/* Dummy xDestroy */
static int dummy_xDestroy(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

/* Dummy xOpen */
static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    /* Allocate a dummy cursor */
    sqlite3_vtab_cursor *cur = sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (!cur) return SQLITE_NOMEM;
    memset(cur, 0, sizeof(sqlite3_vtab_cursor));
    *ppCursor = cur;
    return SQLITE_OK;
}

/* Dummy xClose */
static int dummy_xClose(sqlite3_vtab_cursor *cur) {
    sqlite3_free(cur);
    return SQLITE_OK;
}

/* Dummy xFilter */
static int dummy_xFilter(sqlite3_vtab_cursor *cur, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    (void)cur; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

/* Dummy xNext */
static int dummy_xNext(sqlite3_vtab_cursor *cur) {
    (void)cur;
    return SQLITE_DONE;
}

/* Dummy xEof */
static int dummy_xEof(sqlite3_vtab_cursor *cur) {
    (void)cur;
    return 1;
}

/* Dummy xColumn */
static int dummy_xColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i) {
    (void)cur; (void)ctx; (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

/* Dummy xRowid */
static int dummy_xRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid) {
    (void)cur;
    *pRowid = 1;
    return SQLITE_OK;
}

/* Dummy xUpdate */
static int dummy_xUpdate(sqlite3_vtab *p, int argc, sqlite3_value **argv, sqlite_int64 *pRowid) {
    (void)p; (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

/* Dummy xBegin */
static int dummy_xBegin(sqlite3_vtab *p) { (void)p; return SQLITE_OK; }
/* Dummy xSync */
static int dummy_xSync(sqlite3_vtab *p) { (void)p; return SQLITE_OK; }
/* Dummy xCommit */
static int dummy_xCommit(sqlite3_vtab *p) { (void)p; return SQLITE_OK; }
/* Dummy xRollback */
static int dummy_xRollback(sqlite3_vtab *p) { (void)p; return SQLITE_OK; }

/*
 * Static helper to register the dummy virtual table module.
 */
static int register_dummy_module(sqlite3 *db) {
    sqlite3_module mod = {0};
    mod.iVersion = 3;
    mod.xCreate = NULL;
    mod.xConnect = dummy_xConnect;
    mod.xBestIndex = dummy_xBestIndex;
    mod.xDisconnect = dummy_xDisconnect;
    mod.xDestroy = dummy_xDestroy;
    mod.xOpen = dummy_xOpen;
    mod.xClose = dummy_xClose;
    mod.xFilter = dummy_xFilter;
    mod.xNext = dummy_xNext;
    mod.xEof = dummy_xEof;
    mod.xColumn = dummy_xColumn;
    mod.xRowid = dummy_xRowid;
    mod.xUpdate = dummy_xUpdate;
    mod.xBegin = dummy_xBegin;
    mod.xSync = dummy_xSync;
    mod.xCommit = dummy_xCommit;
    mod.xRollback = dummy_xRollback;
    mod.xRename = NULL;
    mod.xSavepoint = NULL;
    mod.xRelease = NULL;
    mod.xRollbackTo = NULL;

    return sqlite3_create_module(db, "fuzz_vtab", &mod, NULL);
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = open_db();
    if (!db) return 0;

    /* Register the dummy module */
    if (register_dummy_module(db) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create the virtual table */
    const char *create_sql = "CREATE TABLE fuzz_table USING fuzz_vtab(col1 TEXT, col2 INT);";
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, create_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    /*
     * Use the fuzz input as SQL to exercise the virtual table.
     * We will try to execute the input as SQL.
     * We need to ensure the input is null-terminated or handle it safely.
     */
    
    /* Create a null-terminated copy of the input, ensuring safety */
    char *sql = (char *)sqlite3_malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    /* 
     * We also want to test sqlite3_overload_function directly.
     * We can extract a function name and arg count from the input if possible,
     * or just use fixed values to ensure the API is called.
     */
    
    /* Test sqlite3_overload_function with a name derived from input or fixed */
    const char *func_name = "fuzz_func";
    int n_arg = 0;
    
    if (Size >= 2) {
        /* Use first byte as arg count (0-10) */
        n_arg = (Data[0] % 11);
        /* Use a substring as function name if printable */
        if (Size > 10) {
            char fname[12] = {0};
            int len = 0;
            for (int i = 1; i < 10 && i < Size; i++) {
                if (Data[i] >= 32 && Data[i] < 127) {
                    fname[len++] = Data[i];
                }
            }
            if (len > 0) func_name = fname;
        }
    }
    
    int ov_rc = sqlite3_overload_function(db, func_name, n_arg);
    (void)ov_rc;

    /* Execute the SQL */
    char *exec_err = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &exec_err);
    (void)rc; /* We don't care if SQL fails, we just want to trigger the code paths */
    if (exec_err) sqlite3_free(exec_err);

    /* Cleanup */
    sqlite3_free(sql);
    sqlite3_close(db);

    return 0;
}