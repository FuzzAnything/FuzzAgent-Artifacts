// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:161654:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "sqlite3.h"

/*
 * Simple dummy virtual table implementation to exercise the target APIs.
 * This implementation is intentionally minimal and focuses on calling
 * the fuzzed APIs within the appropriate callbacks.
 */

/* 
 * We define a wrapper struct because sqlite3_vtab is opaque in the public API.
 * We will cast the pointer to this struct to store our custom data.
 */
struct dummy_vtab {
    sqlite3_vtab base;
    sqlite3 *db;
};

static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Exercise sqlite3_vtab_config */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);

    /* Exercise sqlite3_declare_vtab */
    int rc = sqlite3_declare_vtab(db, "CREATE TABLE dummy(col1 TEXT, col2 INT)");
    if (rc != SQLITE_OK) {
        return rc;
    }

    /* Allocate a wrapper structure */
    struct dummy_vtab *pVTab = malloc(sizeof(struct dummy_vtab));
    if (!pVTab) {
        return SQLITE_NOMEM;
    }
    memset(pVTab, 0, sizeof(struct dummy_vtab));
    pVTab->db = db;

    *ppVTab = &pVTab->base;
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    /* Reuse create logic for connect */
    return dummy_xCreate(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    if (pVTab) {
        struct dummy_vtab *pDummy = (struct dummy_vtab *)pVTab;
        free(pDummy);
    }
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    if (pVTab) {
        struct dummy_vtab *pDummy = (struct dummy_vtab *)pVTab;
        free(pDummy);
    }
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) {
        return SQLITE_NOMEM;
    }
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    if (pCursor) {
        free(pCursor);
    }
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    (void)pCursor;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    /* Simple cursor state */
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    /* No data to iterate */
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1; /* Always EOF */
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor;
    (void)i;

    /* Exercise sqlite3_vtab_nochange */
    if (sqlite3_vtab_nochange(ctx)) {
        /* Optimization: return without setting result */
        return SQLITE_OK;
    }

    /* Fallback: return a dummy value */
    sqlite3_result_text(ctx, "dummy", -1, SQLITE_STATIC);
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 1;
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int nArg, sqlite3_value **argv,
                         sqlite3_int64 *pRowid) {
    (void)nArg;
    (void)argv;
    (void)pRowid;

    /* Exercise sqlite3_vtab_on_conflict */
    /* We stored the db pointer in the wrapper struct during xCreate/xConnect */
    if (pVTab) {
        struct dummy_vtab *pDummy = (struct dummy_vtab *)pVTab;
        if (pDummy->db) {
            int conflict = sqlite3_vtab_on_conflict(pDummy->db);
            (void)conflict; /* Use the value to avoid unused warning */
        }
    }

    return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

/* Module destructor for v2 */
static void dummy_module_destroy(void *pClientData) {
    (void)pClientData;
    /* Cleanup if needed */
}

/* Static module definition */
static const sqlite3_module dummy_module = {
    0, /* iVersion */
    dummy_xCreate,
    dummy_xConnect,
    NULL, /* xBestIndex */
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
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

/*
 * Helper to safely open a database in memory.
 */
static sqlite3 *open_db(void) {
    sqlite3 *db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/*
 * Helper to safely close a database.
 */
static void close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = open_db();
    if (!db) {
        return 0;
    }

    /*
     * Use input data to drive fuzzing:
     * - Select module name length and content
     * - Select client data
     * - Select SQL for declare_vtab (if needed)
     * - Select config options
     */

    size_t idx = 0;
    size_t len = Size;

    /* Extract module name */
    size_t name_len = 0;
    if (idx < len) {
        name_len = Data[idx++] % 32;
        if (name_len == 0) name_len = 1;
    }
    if (idx + name_len > len) {
        name_len = len - idx;
    }
    char *module_name = malloc(name_len + 1);
    if (!module_name) {
        close_db(db);
        return 0;
    }
    memcpy(module_name, Data + idx, name_len);
    module_name[name_len] = '\0';
    idx += name_len;

    /* Extract client data length and content */
    size_t client_data_len = 0;
    if (idx < len) {
        client_data_len = Data[idx++] % 64;
    }
    if (idx + client_data_len > len) {
        client_data_len = len - idx;
    }
    void *client_data = NULL;
    if (client_data_len > 0) {
        client_data = malloc(client_data_len);
        if (client_data) {
            memcpy(client_data, Data + idx, client_data_len);
            idx += client_data_len;
        }
    }

    /* Extract SQL for declare_vtab (optional) */
    size_t sql_len = 0;
    if (idx < len) {
        sql_len = Data[idx++] % 128;
    }
    if (idx + sql_len > len) {
        sql_len = len - idx;
    }
    char *sql = NULL;
    if (sql_len > 0) {
        sql = malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, Data + idx, sql_len);
            sql[sql_len] = '\0';
            idx += sql_len;
        }
    }

    /* Extract config option */
    int config_op = 0;
    if (idx < len) {
        config_op = Data[idx++] % 10;
    }

    /*
     * Test sqlite3_create_module
     */
    sqlite3_create_module(db, module_name, &dummy_module, client_data);

    /*
     * Test sqlite3_create_module_v2
     */
    sqlite3_create_module_v2(db, module_name, &dummy_module, client_data, dummy_module_destroy);

    /*
     * Drop module by passing NULL
     */
    sqlite3_create_module(db, module_name, NULL, NULL);

    /*
     * Attempt to create a virtual table to trigger xCreate/xConnect
     */
    char create_stmt[256];
    snprintf(create_stmt, sizeof(create_stmt),
             "CREATE VIRTUAL TABLE IF NOT EXISTS test_vtab USING %s", module_name);
    sqlite3_exec(db, create_stmt, NULL, NULL, NULL);

    /*
     * Attempt to use the virtual table to trigger xColumn (and thus sqlite3_vtab_nochange)
     */
    char select_stmt[256];
    snprintf(select_stmt, sizeof(select_stmt),
             "SELECT col1 FROM test_vtab WHERE 1=0");
    sqlite3_exec(db, select_stmt, NULL, NULL, NULL);

    /*
     * Attempt UPDATE to trigger xUpdate (and thus sqlite3_vtab_on_conflict)
     */
    char update_stmt[256];
    snprintf(update_stmt, sizeof(update_stmt),
             "UPDATE test_vtab SET col1='test' WHERE 1=0");
    sqlite3_exec(db, update_stmt, NULL, NULL, NULL);

    /*
     * Exercise sqlite3_declare_vtab directly if SQL was provided
     * Note: This is normally called from xCreate/xConnect, but we can try it here
     * to test error paths.
     */
    if (sql && strlen(sql) > 0) {
        sqlite3_declare_vtab(db, sql);
    }

    /*
     * Exercise sqlite3_vtab_config directly (undefined behavior outside xCreate/xConnect,
     * but useful for fuzzing error handling)
     */
    sqlite3_vtab_config(db, config_op, 1);

    /* Cleanup */
    free(module_name);
    if (client_data) {
        free(client_data);
    }
    if (sql) {
        free(sql);
    }

    close_db(db);

    return 0;
}