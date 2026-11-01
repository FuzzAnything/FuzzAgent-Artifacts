// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* Forward declarations for internal structures not exposed in sqlite3.h */

/* Helper to cast const char* to const char* const* for the module interface */
static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVtab, char **pzErr) {
  (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
  /* Allocate a dummy vtab structure to prevent NULL pointer dereference in xCreate */
  *ppVtab = (sqlite3_vtab *)sqlite3_malloc64(sizeof(sqlite3_vtab));
  if (*ppVtab) {
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
  }
  return SQLITE_OK;
}

static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
  (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
  /* Allocate a dummy vtab structure */
  *ppVtab = (sqlite3_vtab *)sqlite3_malloc64(sizeof(sqlite3_vtab));
  if (*ppVtab) {
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
  }
  return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
  (void)pVtab;
  (void)pIdxInfo;
  return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
  if (pVtab) {
    sqlite3_free(pVtab);
  }
  return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVtab) {
  if (pVtab) {
    sqlite3_free(pVtab);
  }
  return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
  (void)pVtab;
  *ppCursor = NULL;
  return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *cur) {
  (void)cur;
  return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *cur, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
  (void)cur; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
  return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *cur) {
  (void)cur;
  return SQLITE_DONE;
}

static int dummy_xEof(sqlite3_vtab_cursor *cur) {
  (void)cur;
  return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i) {
  (void)cur; (void)ctx; (void)i;
  sqlite3_result_null(ctx);
  return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid) {
  (void)cur;
  *pRowid = 0;
  return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                         sqlite_int64 *pRowid) {
  (void)pVtab; (void)argc; (void)argv; (void)pRowid;
  return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVtab) {
  (void)pVtab;
  return SQLITE_OK;
}

static const sqlite3_module dummy_module = {
  0,
  dummy_xConnect,
  dummy_xCreate,
  dummy_xBestIndex,
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
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  int rc;

  if (Size < 2) return 0;

  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == NULL) return 0;

  rc = sqlite3_create_module(db, "fuzz_vtab", &dummy_module, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE fuzz_test USING fuzz_vtab;", NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  const uint8_t *ptr = Data;
  size_t remaining = Size;

  while (remaining > 0) {
    uint8_t op = *ptr;
    ptr++;
    remaining--;

    switch (op % 6) {
      case 0: {
        int opt_code = (remaining > 0) ? (int)*ptr : 0;
        ptr++;
        remaining--;
        int dummy_int = (remaining > 0) ? (int)*ptr : 0;
        ptr++;
        remaining--;
        sqlite3_vtab_config(db, opt_code, dummy_int);
        break;
      }
      case 1: {
        int arg_count = (remaining > 0) ? (int)*ptr : 0;
        ptr++;
        remaining--;
        char func_name[64] = {0};
        int name_len = (remaining > 0) ? (int)*ptr : 0;
        ptr++;
        remaining--;
        if (name_len > 0 && remaining >= (size_t)name_len) {
          memcpy(func_name, ptr, name_len);
          ptr += name_len;
          remaining -= name_len;
        }
        sqlite3_overload_function(db, func_name, arg_count);
        break;
      }
      case 2: {
        sqlite3_stmt *stmt = NULL;
        const char *sql = "SELECT * FROM fuzz_test WHERE col1 = 1 ORDER BY col2;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt != NULL) {
          sqlite3_index_info info = {0};
          info.nConstraint = 1;
          
          struct sqlite3_index_constraint *cons = malloc(sizeof(struct sqlite3_index_constraint));
          if (cons) {
            info.aConstraint = cons;
            info.aConstraint[0].iColumn = 0;
            info.aConstraint[0].op = SQLITE_INDEX_CONSTRAINT_EQ;
            info.aConstraint[0].usable = 1;
            info.aConstraint[0].iTermOffset = 0;
          }

          info.nOrderBy = 1;
          struct sqlite3_index_orderby *order = malloc(sizeof(struct sqlite3_index_orderby));
          if (order) {
            info.aOrderBy = order;
            info.aOrderBy[0].iColumn = 1;
            info.aOrderBy[0].desc = 0;
          }

          struct sqlite3_index_constraint_usage *usage = malloc(sizeof(struct sqlite3_index_constraint_usage));
          if (usage) {
            info.aConstraintUsage = usage;
            info.aConstraintUsage[0].argvIndex = 1;
            info.aConstraintUsage[0].omit = 0;
          }

          sqlite3_value *val = NULL;
          sqlite3_vtab_rhs_value(&info, 0, &val);
          if (val) {
            sqlite3_value_free(val);
          }

          int distinct_mode = sqlite3_vtab_distinct(&info);
          (void)distinct_mode;

          const char *coll = sqlite3_vtab_collation(&info, 0);
          (void)coll;

          int in_result = sqlite3_vtab_in(&info, 0, -1);
          (void)in_result;
          int in_request = sqlite3_vtab_in(&info, 0, 1);
          (void)in_request;

          if (info.aConstraintUsage) free(info.aConstraintUsage);
          if (info.aOrderBy) free(info.aOrderBy);
          if (info.aConstraint) free(info.aConstraint);
        }
        if (stmt) sqlite3_finalize(stmt);
        break;
      }
      case 3: {
        sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
        break;
      }
      case 4: {
        sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);
        break;
      }
      case 5: {
        sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS, 1);
        break;
      }
    }

    if (remaining < 1) break;
  }

  sqlite3_close(db);
  return 0;
}