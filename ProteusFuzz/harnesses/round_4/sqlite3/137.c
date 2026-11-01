// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
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
 * Static helper to create a dummy virtual table module.
 * This is required to trigger the xBestIndex callback where the target APIs reside.
 */
static int dummy_vtab_best_index(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
  (void)pVtab;
  /* 
   * The fuzzing logic for the target APIs is implemented in LLVMFuzzerTestOneInput.
   * This function acts as the entry point for the virtual table implementation.
   * We simply return SQLITE_OK to indicate success.
   */
  return SQLITE_OK;
}

static int dummy_vtab_filter(sqlite3_vtab_cursor *pCur, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
  (void)pCur;
  (void)idxNum;
  (void)idxStr;
  (void)argc;
  (void)argv;
  return SQLITE_OK;
}

static int dummy_vtab_next(sqlite3_vtab_cursor *pCur) {
  (void)pCur;
  return SQLITE_DONE;
}

static int dummy_vtab_eof(sqlite3_vtab_cursor *pCur) {
  (void)pCur;
  return 1;
}

static int dummy_vtab_column(sqlite3_vtab_cursor *pCur, sqlite3_context *ctx, int i) {
  (void)pCur;
  (void)ctx;
  (void)i;
  sqlite3_result_null(ctx);
  return SQLITE_OK;
}

static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCur, sqlite3_int64 *pRowid) {
  (void)pCur;
  (void)pRowid;
  return SQLITE_OK;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
  (void)db;
  (void)pAux;
  (void)argc;
  (void)argv;
  (void)pzErr;
  
  /* Allocate memory for the virtual table structure */
  *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
  if (*ppVtab == NULL) {
    return SQLITE_NOMEM;
  }
  
  /* Initialize the virtual table structure */
  memset(*ppVtab, 0, sizeof(sqlite3_vtab));
  
  /* Set the module pointer */
  (*ppVtab)->pModule = (sqlite3_module *)pAux;
  
  return SQLITE_OK;
}

static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
  return dummy_vtab_connect(db, pAux, argc, argv, ppVtab, pzErr);
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVtab) {
  if (pVtab) {
    sqlite3_free(pVtab);
  }
  return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVtab) {
  return dummy_vtab_disconnect(pVtab);
}

static int dummy_vtab_open(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
  (void)pVtab;
  *ppCursor = (sqlite3_vtab_cursor *)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
  if (*ppCursor == NULL) {
    return SQLITE_NOMEM;
  }
  memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
  return SQLITE_OK;
}

static int dummy_vtab_close(sqlite3_vtab_cursor *pCur) {
  if (pCur) {
    sqlite3_free(pCur);
  }
  return SQLITE_OK;
}

static int dummy_vtab_begin(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_vtab_sync(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_vtab_commit(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_vtab_rollback(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_vtab_update(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
  (void)pVtab;
  (void)argc;
  (void)argv;
  (void)pRowid;
  return SQLITE_OK;
}

static int dummy_vtab_bestindex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
  return dummy_vtab_best_index(pVtab, pIdxInfo);
}

/*
 * Fuzz driver entry point
 */
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  int rc;
  sqlite3_module *module;
  
  /* Initialize the database connection */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    return 0;
  }
  
  /* Create a dummy virtual table module */
  module = (sqlite3_module *)sqlite3_malloc(sizeof(sqlite3_module));
  if (module == NULL) {
    sqlite3_close(db);
    return 0;
  }
  memset(module, 0, sizeof(sqlite3_module));
  module->iVersion = 3;
  module->xCreate = dummy_vtab_create;
  module->xConnect = dummy_vtab_connect;
  module->xBestIndex = dummy_vtab_bestindex;
  module->xDisconnect = dummy_vtab_disconnect;
  module->xDestroy = dummy_vtab_destroy;
  module->xOpen = dummy_vtab_open;
  module->xClose = dummy_vtab_close;
  module->xFilter = dummy_vtab_filter;
  module->xNext = dummy_vtab_next;
  module->xEof = dummy_vtab_eof;
  module->xColumn = dummy_vtab_column;
  module->xRowid = dummy_vtab_rowid;
  module->xBegin = dummy_vtab_begin;
  module->xSync = dummy_vtab_sync;
  module->xCommit = dummy_vtab_commit;
  module->xRollback = dummy_vtab_rollback;
  module->xUpdate = dummy_vtab_update;
  
  /* Register the module */
  rc = sqlite3_create_module(db, "dummy_vtab", module, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_free(module);
    sqlite3_close(db);
    return 0;
  }
  
  /* Create a dummy virtual table */
  rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE dummy USING dummy_vtab", NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }
  
  /* 
   * Prepare a query that will trigger xBestIndex.
   * We use a simple SELECT with a WHERE clause to ensure constraints are present.
   */
  sqlite3_stmt *stmt;
  const char *sql = "SELECT * FROM dummy WHERE col1 = ?";
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }
  
  /* Bind a parameter to trigger the query execution */
  if (Size > 0) {
    sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
  }
  
  /* Execute the statement to trigger xBestIndex */
  rc = sqlite3_step(stmt);
  (void)rc;
  
  /* Clean up the statement */
  sqlite3_finalize(stmt);
  
  /* 
   * Test sqlite3_overload_function with fuzzed input
   */
  if (Size > 10) {
    char func_name[64];
    int nArg;
    
    /* Extract function name and argument count from fuzzed data */
    size_t name_len = (Size > 64) ? 63 : Size - 1;
    memcpy(func_name, Data, name_len);
    func_name[name_len] = '\0';
    nArg = (int)Data[name_len + 1] % 10;
    
    rc = sqlite3_overload_function(db, func_name, nArg);
    (void)rc;
  }
  
  /* Clean up the database connection */
  sqlite3_close(db);
  
  return 0;
}