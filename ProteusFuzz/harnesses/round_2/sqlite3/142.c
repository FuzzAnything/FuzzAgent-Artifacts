// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:161654:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Dummy VTable implementation for fuzzing */
typedef struct {
    sqlite3_vtab base;
    sqlite3 *db; /* Store db pointer here to access it in methods */
    int dummy;
} MyVtab;

typedef struct {
    sqlite3_vtab_cursor base;
    int dummy;
} MyVtabCursor;

/* Static helper to safely read a null-terminated string from fuzz input */
static const char *get_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    /* Find the end of the string */
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    
    /* If we didn't find a null terminator within bounds, return NULL */
    if (*offset == size && start == *offset) return NULL;
    if (*offset == size) {
        /* If we hit the end without a null terminator, treat it as invalid */
        return NULL;
    }
    
    size_t len = *offset - start;
    if (len == 0) return NULL;
    
    /* Create a temporary buffer for the string to ensure null-termination */
    static char buf[256];
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;
    
    memcpy(buf, data + start, len);
    buf[len] = '\0';
    
    return buf;
}

/* Dummy xCreate implementation */
static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                   sqlite3_vtab **ppVtab, char **pzErr) {
    MyVtab *p = (MyVtab *)sqlite3_malloc(sizeof(MyVtab));
    if (!p) return SQLITE_NOMEM;
    memset(p, 0, sizeof(MyVtab));
    p->db = db; /* Store the db pointer */
    *ppVtab = &p->base;
    
    /* Fuzz sqlite3_vtab_config */
    if (argc > 0) {
        int op = atoi(argv[0]);
        int arg = (argc > 1) ? atoi(argv[1]) : 0;
        sqlite3_vtab_config(db, op, arg);
    }
    
    /* Fuzz sqlite3_declare_vtab */
    if (argc > 2) {
        sqlite3_declare_vtab(db, argv[2]);
    }
    
    return SQLITE_OK;
}

/* Dummy xConnect implementation */
static int xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                    sqlite3_vtab **ppVtab, char **pzErr) {
    return xCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

/* Dummy xDisconnect implementation */
static int xDisconnect(sqlite3_vtab *pVtab) {
    MyVtab *p = (MyVtab *)pVtab;
    sqlite3_free(p);
    return SQLITE_OK;
}

/* Dummy xDestroy implementation */
static int xDestroy(sqlite3_vtab *pVtab) {
    MyVtab *p = (MyVtab *)pVtab;
    sqlite3_free(p);
    return SQLITE_OK;
}

/* Dummy xOpen implementation */
static int xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    MyVtabCursor *p = (MyVtabCursor *)sqlite3_malloc(sizeof(MyVtabCursor));
    if (!p) return SQLITE_NOMEM;
    memset(p, 0, sizeof(MyVtabCursor));
    *ppCursor = &p->base;
    return SQLITE_OK;
}

/* Dummy xClose implementation */
static int xClose(sqlite3_vtab_cursor *cur) {
    sqlite3_free(cur);
    return SQLITE_OK;
}

/* Dummy xFilter implementation */
static int xFilter(sqlite3_vtab_cursor *pCur, int idxNum, const char *idxStr,
                   int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

/* Dummy xNext implementation */
static int xNext(sqlite3_vtab_cursor *pCur) {
    return SQLITE_DONE;
}

/* Dummy xEof implementation */
static int xEof(sqlite3_vtab_cursor *pCur) {
    return 1;
}

/* Dummy xColumn implementation - fuzzes sqlite3_vtab_nochange */
static int xColumn(sqlite3_vtab_cursor *pCur, sqlite3_context *ctx, int i) {
    (void)pCur;
    (void)i;
    
    /* Fuzz sqlite3_vtab_nochange */
    int nochange = sqlite3_vtab_nochange(ctx);
    (void)nochange;
    
    sqlite3_result_int(ctx, 0);
    return SQLITE_OK;
}

/* Dummy xRowid implementation */
static int xRowid(sqlite3_vtab_cursor *pCur, sqlite3_int64 *pRowid) {
    *pRowid = 1;
    return SQLITE_OK;
}

/* Dummy xUpdate implementation - fuzzes sqlite3_vtab_on_conflict */
static int xUpdate(sqlite3_vtab *pVtab, int nArg, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)nArg;
    (void)argv;
    (void)pRowid;
    
    MyVtab *p = (MyVtab *)pVtab;
    
    /* Fuzz sqlite3_vtab_on_conflict */
    int conflict = sqlite3_vtab_on_conflict(p->db);
    (void)conflict;
    
    return SQLITE_OK;
}

/* Dummy xBegin implementation */
static int xBegin(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

/* Dummy xSync implementation */
static int xSync(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

/* Dummy xCommit implementation */
static int xCommit(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

/* Dummy xRollback implementation */
static int xRollback(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

/* Dummy xSavepoint implementation */
static int xSavepoint(sqlite3_vtab *pVtab, int) {
    return SQLITE_OK;
}

/* Dummy xRelease implementation */
static int xRelease(sqlite3_vtab *pVtab, int) {
    return SQLITE_OK;
}

/* Dummy xRollbackTo implementation */
static int xRollbackTo(sqlite3_vtab *pVtab, int) {
    return SQLITE_OK;
}

/* Module definition */
static const sqlite3_module my_module = {
    3, /* iVersion */
    xCreate,
    xConnect,
    NULL, /* xBestIndex */
    xDisconnect,
    xDestroy,
    xOpen,
    xClose,
    xFilter,
    xNext,
    xEof,
    xColumn,
    xRowid,
    xUpdate,
    xBegin,
    xSync,
    xCommit,
    xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    xSavepoint,
    xRelease,
    xRollbackTo,
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    sqlite3 *db;
    int rc;
    size_t offset = 0;
    
    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Register the module using sqlite3_create_module */
    rc = sqlite3_create_module(db, "fuzz_vtab", &my_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Register the module using sqlite3_create_module_v2 */
    rc = sqlite3_create_module_v2(db, "fuzz_vtab_v2", &my_module, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Try to create a virtual table to trigger xCreate/xConnect */
    if (Size > 10) {
        /* Extract a potential SQL string for CREATE VIRTUAL TABLE */
        const char *tbl_name = get_string(Data, Size, &offset);
        if (tbl_name && strlen(tbl_name) > 0) {
            char create_stmt[256];
            snprintf(create_stmt, sizeof(create_stmt), "CREATE VIRTUAL TABLE %s USING fuzz_vtab", tbl_name);
            rc = sqlite3_exec(db, create_stmt, NULL, NULL, NULL);
            (void)rc;
        }
        
        /* Try with the v2 module */
        if (tbl_name && strlen(tbl_name) > 0) {
            char create_stmt[256];
            snprintf(create_stmt, sizeof(create_stmt), "CREATE VIRTUAL TABLE %s_v2 USING fuzz_vtab_v2", tbl_name);
            rc = sqlite3_exec(db, create_stmt, NULL, NULL, NULL);
            (void)rc;
        }
    }
    
    /* Try to execute an UPDATE to trigger xUpdate and sqlite3_vtab_on_conflict */
    if (Size > 20) {
        const char *tbl_name = get_string(Data, Size, &offset);
        if (tbl_name && strlen(tbl_name) > 0) {
            char update_stmt[256];
            snprintf(update_stmt, sizeof(update_stmt), "UPDATE %s SET col1 = 1 WHERE rowid = 1", tbl_name);
            rc = sqlite3_exec(db, update_stmt, NULL, NULL, NULL);
            (void)rc;
        }
        
        /* Try with conflict modes */
        if (tbl_name && strlen(tbl_name) > 0) {
            char update_stmt[256];
            snprintf(update_stmt, sizeof(update_stmt), "UPDATE OR REPLACE %s SET col1 = 1 WHERE rowid = 1", tbl_name);
            rc = sqlite3_exec(db, update_stmt, NULL, NULL, NULL);
            (void)rc;
            
            snprintf(update_stmt, sizeof(update_stmt), "UPDATE OR IGNORE %s SET col1 = 1 WHERE rowid = 1", tbl_name);
            rc = sqlite3_exec(db, update_stmt, NULL, NULL, NULL);
            (void)rc;
        }
    }
    
    /* Drop the modules */
    sqlite3_create_module(db, "fuzz_vtab", NULL, NULL);
    sqlite3_create_module(db, "fuzz_vtab_v2", NULL, NULL);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}