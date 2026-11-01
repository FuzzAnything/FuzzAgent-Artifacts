// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:161654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to safely open an in-memory database */
static sqlite3 *open_db(void) {
    sqlite3 *db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/* Static helper to safely close database and finalize statements */
static void close_db(sqlite3 *db, sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

/* Dummy module implementation to trigger virtual table logic */
static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                             sqlite3_vtab **ppVTab, char **pzErr) {
    /* Test sqlite3_vtab_config */
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);

    /* Test sqlite3_declare_vtab with a simple schema */
    const char *sql = "CREATE TABLE dummy_table(id INTEGER PRIMARY KEY, name TEXT)";
    return sqlite3_declare_vtab(db, sql);
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_vtab_create(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    free(pVTab);
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    free(pVTab);
    return SQLITE_OK;
}

static int dummy_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_vtab_close(sqlite3_vtab_cursor *pCursor) {
    free(pCursor);
    return SQLITE_OK;
}

static int dummy_vtab_filter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                             int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_vtab_next(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_eof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_vtab_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    /* Test sqlite3_vtab_nochange */
    (void)sqlite3_vtab_nochange(ctx);
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    /* Test sqlite3_vtab_on_conflict. 
       Note: sqlite3_vtab_on_conflict expects a sqlite3* connection.
       The connection is typically stored inside the vtab implementation's private data.
       For this dummy implementation, we assume the pVTab pointer itself or a known offset
       doesn't hold it directly in the public struct, but we can't access private fields.
       However, the API requires a sqlite3* argument. 
       In a real module, pVTab->pMod->pAux or similar would hold the db.
       Since we can't access private members of sqlite3_vtab, we cannot call this safely
       inside the dummy update without a valid db pointer passed via pAux or stored in pVTab's private data.
       To satisfy the fuzzer logic without crashing, we skip the call if we don't have a valid db.
       But wait, the prompt says "Call it with a valid DB".
       We can store the db pointer in the pVTab structure if we allocate it with extra space,
       or we can rely on the fact that the fuzzer input might not trigger this path deeply.
       Let's try to pass a dummy or NULL to see if it handles it, but the API says it must be valid.
       Actually, looking at the signature: int sqlite3_vtab_on_conflict(sqlite3 *);
       We need a valid sqlite3*. 
       In a real scenario, the module stores the db in the pVTab struct (which is user-defined).
       Let's define a custom vtab struct that holds the db.
    */
    return SQLITE_OK;
}

/* Custom vtab structure to hold the database connection */
typedef struct {
    sqlite3_vtab base;
    sqlite3 *db;
} dummy_vtab;

/* Redefine methods to use the custom struct */
static int dummy_vtab_create_fixed(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                             sqlite3_vtab **ppVTab, char **pzErr) {
    dummy_vtab *v = malloc(sizeof(dummy_vtab));
    if (!v) return SQLITE_NOMEM;
    memset(v, 0, sizeof(dummy_vtab));
    v->db = db;
    
    /* Test sqlite3_vtab_config */
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    (void)sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);

    /* Test sqlite3_declare_vtab with a simple schema */
    const char *sql = "CREATE TABLE dummy_table(id INTEGER PRIMARY KEY, name TEXT)";
    int rc = sqlite3_declare_vtab(db, sql);
    if (rc != SQLITE_OK) {
        free(v);
        return rc;
    }
    *ppVTab = &v->base;
    return SQLITE_OK;
}

static int dummy_vtab_connect_fixed(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_vtab_create_fixed(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_vtab_disconnect_fixed(sqlite3_vtab *pVTab) {
    dummy_vtab *v = (dummy_vtab*)pVTab;
    free(v);
    return SQLITE_OK;
}

static int dummy_vtab_destroy_fixed(sqlite3_vtab *pVTab) {
    dummy_vtab *v = (dummy_vtab*)pVTab;
    free(v);
    return SQLITE_OK;
}

static int dummy_vtab_update_fixed(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    dummy_vtab *v = (dummy_vtab*)pVTab;
    if (v && v->db) {
        /* Test sqlite3_vtab_on_conflict */
        (void)sqlite3_vtab_on_conflict(v->db);
    }
    return SQLITE_OK;
}

static const sqlite3_module dummy_module = {
    3, /* iVersion */
    dummy_vtab_create_fixed,
    dummy_vtab_connect_fixed,
    NULL, /* xBestIndex */
    dummy_vtab_disconnect_fixed,
    dummy_vtab_destroy_fixed,
    dummy_vtab_open,
    dummy_vtab_close,
    dummy_vtab_filter,
    dummy_vtab_next,
    dummy_vtab_eof,
    dummy_vtab_column,
    dummy_vtab_rowid,
    dummy_vtab_update_fixed,
    NULL, NULL, NULL, NULL, /* xBegin, xSync, xCommit, xRollback */
    NULL, NULL, /* xFindFunction, xRename */
    NULL, NULL, NULL, /* xSavepoint, xRelease, xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = open_db();
    if (!db) return 0;

    /* Use input data to construct SQL strings safely */
    char *sql_buf = malloc(Size + 64);
    if (!sql_buf) {
        close_db(db, NULL);
        return 0;
    }

    /* Ensure null termination */
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    /* Fuzz sqlite3_create_module and sqlite3_create_module_v2 */
    /* We use a fixed module name "fuzz_module" to ensure valid registration */
    int rc_module = sqlite3_create_module(db, "fuzz_module", &dummy_module, NULL);
    if (rc_module == SQLITE_OK) {
        /* Try v2 version with a dummy destructor */
        rc_module = sqlite3_create_module_v2(db, "fuzz_module", &dummy_module, (void*)0x1234, NULL);
    }

    /* Fuzz virtual table creation which triggers sqlite3_declare_vtab */
    /* Construct CREATE VIRTUAL TABLE statement */
    char create_stmt[256];
    snprintf(create_stmt, sizeof(create_stmt), "CREATE VIRTUAL TABLE ft USING fuzz_module");

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, create_stmt, -1, &stmt, NULL);

    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Fuzz with malformed SQL from input if it looks like a statement */
    /* We try to execute the raw input as SQL to trigger parsing errors in vtab logic */
    if (Size > 10 && Size < 1000) {
        /* Check if it starts with CREATE VIRTUAL TABLE to avoid obvious syntax errors on other commands */
        if (strncmp(sql_buf, "CREATE VIRTUAL TABLE", 18) == 0) {
            rc = sqlite3_prepare_v2(db, sql_buf, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                rc = sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
    }

    /* Drop the module to clean up */
    sqlite3_create_module(db, "fuzz_module", NULL, NULL);

    close_db(db, stmt);
    free(sql_buf);

    return 0;
}