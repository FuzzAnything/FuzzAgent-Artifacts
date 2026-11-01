// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 virtual table helper functions.
 * 
 * Note: The target functions (sqlite3_vtab_distinct, sqlite3_vtab_collation,
 * sqlite3_vtab_rhs_value, sqlite3_vtab_in) are strictly required to be called
 * from within an xBestIndex method of a virtual table. Calling them directly
 * from a fuzzer entry point results in undefined behavior.
 * 
 * To fuzz these safely and effectively, we implement a dummy virtual table
 * module. The fuzzer input drives the xBestIndex method of this module,
 * where we invoke the target APIs with the fuzzed data.
 */

/* Dummy virtual table structure */
typedef struct DummyVtab {
  sqlite3_vtab base;
  int dummyInt;
} DummyVtab;

/* Dummy cursor structure */
typedef struct DummyCursor {
  sqlite3_vtab_cursor base;
  int dummyInt;
} DummyCursor;

/*
 * xConnect: Create a virtual table instance.
 * We use sqlite3_vtab_config here to fuzz that API.
 */
static int xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                    sqlite3_vtab **ppVtab, char **pzErr) {
  DummyVtab *pNew;
  int rc;

  /* Allocate memory */
  pNew = (DummyVtab *)sqlite3_malloc(sizeof(DummyVtab));
  if (pNew == 0) {
    *pzErr = sqlite3_mprintf("Out of memory");
    return SQLITE_NOMEM;
  }
  memset(pNew, 0, sizeof(DummyVtab));
  pNew->base.pModule = (sqlite3_module *)pAux;

  /* Fuzz sqlite3_vtab_config with various options based on input if available.
   * Since we don't have direct input here, we try a few standard configs.
   * In a real fuzzing scenario, we might pass a seed to the module.
   */
  rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
  (void)rc; /* Ignore return for fuzzing stability */
  
  rc = sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS, 1);
  (void)rc;

  *ppVtab = &pNew->base;
  return SQLITE_OK;
}

/* xCreate: Same as xConnect for this dummy table */
static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                   sqlite3_vtab **ppVtab, char **pzErr) {
  return xConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

/* xBestIndex: The core fuzzing point for vtab helper functions. */
static int xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
  int i;
  int rc;
  sqlite3_value *pVal = 0;
  const char *zColl;
  int distinctVal;

  /* 
   * The input data is not directly passed to xBestIndex. 
   * However, the fuzz driver will construct the pIdxInfo structure 
   * (or let SQLite do it based on a fuzzed SQL query) to trigger 
   * different code paths. 
   * 
   * To maximize coverage, we iterate through constraints and call the APIs.
   */

  /* 1. Fuzz sqlite3_vtab_distinct */
  /* This API must be called with the pIdxInfo passed to xBestIndex. */
  distinctVal = sqlite3_vtab_distinct(pIdxInfo);
  /* distinctVal can be 0, 1, 2, or 3. We just read it to ensure no crash. */
  (void)distinctVal;

  /* 2. Fuzz sqlite3_vtab_collation */
  /* Iterate through constraints to test different indices. */
  if (pIdxInfo->aConstraint && pIdxInfo->nConstraint > 0) {
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
      zColl = sqlite3_vtab_collation(pIdxInfo, i);
      /* zColl should be a valid string pointer (e.g., "BINARY" or custom). */
      if (zColl) {
        /* Do nothing, just ensure it doesn't crash */
      }
    }
  }

  /* 3. Fuzz sqlite3_vtab_rhs_value */
  if (pIdxInfo->aConstraint && pIdxInfo->nConstraint > 0) {
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
      rc = sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
      /* rc is SQLITE_OK if value found, SQLITE_NOTFOUND otherwise. */
      if (rc == SQLITE_OK && pVal) {
        /* Value is valid only during this call. Do not store. */
        (void)sqlite3_value_int(pVal); /* Force a read to exercise code */
      }
    }
  }

  /* 4. Fuzz sqlite3_vtab_in */
  /* Check if constraints are IN operators and negotiate handling. */
  if (pIdxInfo->aConstraint && pIdxInfo->nConstraint > 0) {
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
      /* First check if it's an IN operator */
      int is_in = sqlite3_vtab_in(pIdxInfo, i, -1);
      
      if (is_in) {
        /* Try to request all-at-once processing */
        int handled = sqlite3_vtab_in(pIdxInfo, i, 1);
        (void)handled;
        
        /* Try to decline */
        int declined = sqlite3_vtab_in(pIdxInfo, i, 0);
        (void)declined;
      }
    }
  }

  /* Set a dummy index to satisfy the planner */
  pIdxInfo->idxNum = 1;
  pIdxInfo->orderByConsumed = 0;
  pIdxInfo->estimatedCost = 10.0;
  pIdxInfo->estimatedRows = 1;

  return SQLITE_OK;
}

/* Dummy xDisconnect */
static int xDisconnect(sqlite3_vtab *pVtab) {
  DummyVtab *p = (DummyVtab *)pVtab;
  sqlite3_free(p);
  return SQLITE_OK;
}

/* Dummy xDestroy */
static int xDestroy(sqlite3_vtab *pVtab) {
  DummyVtab *p = (DummyVtab *)pVtab;
  sqlite3_free(p);
  return SQLITE_OK;
}

/* Dummy xOpen */
static int xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
  DummyCursor *pCur;
  pCur = (DummyCursor *)sqlite3_malloc(sizeof(DummyCursor));
  if (pCur == 0) return SQLITE_NOMEM;
  memset(pCur, 0, sizeof(DummyCursor));
  *ppCursor = &pCur->base;
  return SQLITE_OK;
}

/* Dummy xClose */
static int xClose(sqlite3_vtab_cursor *pCursor) {
  DummyCursor *pCur = (DummyCursor *)pCursor;
  sqlite3_free(pCur);
  return SQLITE_OK;
}

/* Dummy xFilter */
static int xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                   int argc, sqlite3_value **argv) {
  /* No-op filter */
  return SQLITE_OK;
}

/* Dummy xNext */
static int xNext(sqlite3_vtab_cursor *pCursor) {
  return SQLITE_DONE;
}

/* Dummy xEof */
static int xEof(sqlite3_vtab_cursor *pCursor) {
  return 1;
}

/* Dummy xColumn */
static int xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
  sqlite3_result_null(ctx);
  return SQLITE_OK;
}

/* Dummy xRowid */
static int xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
  *pRowid = 0;
  return SQLITE_OK;
}

/* Dummy xUpdate - not strictly needed for read-only fuzzing but good for completeness */
static int xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                   sqlite3_int64 *pRowid) {
  return SQLITE_CONSTRAINT;
}

/* Dummy xBegin, xSync, xCommit, xRollback - minimal stubs */
static int xBegin(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int xSync(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int xCommit(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int xRollback(sqlite3_vtab *pVtab) { return SQLITE_OK; }

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
  sqlite3 *db = 0;
  int rc;
  const char *zSql;
  sqlite3_stmt *pStmt = 0;
  
  /* Create an in-memory database */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Register the dummy virtual table module */
  static sqlite3_module module = {
    0, /* iVersion */
    xConnect,
    xCreate,
    xBestIndex,
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
    xRollback
  };

  rc = sqlite3_create_module(db, "dummy_vtab", &module, 0);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Create a dummy table */
  rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE dummy USING dummy_vtab;", 0, 0, 0);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* 
   * Construct a SQL query from the fuzz input to drive xBestIndex.
   * We interpret the input as a SQL string. 
   * We must ensure it's null-terminated and safe-ish.
   */
  if (Size == 0) {
    sqlite3_close(db);
    return 0;
  }

  /* Allocate buffer for SQL */
  char *zSqlBuf = (char *)sqlite3_malloc(Size + 1);
  if (!zSqlBuf) {
    sqlite3_close(db);
    return 0;
  }
  memcpy(zSqlBuf, Data, Size);
  zSqlBuf[Size] = '\0';
  zSql = zSqlBuf;

  /* 
   * We want to fuzz queries that trigger xBestIndex.
   * SELECT ... FROM dummy WHERE ...
   * We prepend a safe SELECT if the input doesn't look like one,
   * but for fuzzing, raw input is often better to find edge cases.
   * However, to ensure xBestIndex is called, we need a valid query structure.
   * Let's try to execute the input as is. If it fails, we try a wrapper.
   */
  
  rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, 0);
  
  if (rc == SQLITE_OK) {
    /* Execute the statement to trigger the virtual table methods */
    rc = sqlite3_step(pStmt);
    (void)rc; /* We don't care about the result, just that it didn't crash */
    sqlite3_finalize(pStmt);
  } else {
    /* If the raw input is not valid SQL, try wrapping it in a safe query 
       that forces a scan of the dummy table.
       We use the input as a WHERE clause value if possible.
    */
    char *zSafeSql = (char *)sqlite3_malloc(256 + Size);
    if (zSafeSql) {
      /* Escape single quotes in input to prevent SQL syntax errors */
      int j = 0;
      for (int i = 0; i < Size && j < 250; i++) {
        if (Data[i] == '\'') {
          zSafeSql[j++] = '\'';
          zSafeSql[j++] = '\'';
        } else if (Data[i] >= 32 && Data[i] < 127) {
          zSafeSql[j++] = Data[i];
        } else {
          /* Skip non-printable chars for safety in this fallback */
        }
      }
      zSafeSql[j] = '\0';
      
      /* Construct: SELECT * FROM dummy WHERE col = 'input' */
      /* We assume the table has columns. Since we didn't declare them, 
         this might fail. Let's just do a simple SELECT * FROM dummy. */
      
      const char *zSimple = "SELECT * FROM dummy";
      rc = sqlite3_prepare_v2(db, zSimple, -1, &pStmt, 0);
      if (rc == SQLITE_OK) {
        rc = sqlite3_step(pStmt);
        (void)rc;
        sqlite3_finalize(pStmt);
      }
      sqlite3_free(zSafeSql);
    }
  }

  /* Fuzz sqlite3_overload_function separately as it doesn't require xBestIndex */
  /* We try to overload a function named based on the input */
  if (Size > 0) {
    char *zFuncName = (char *)sqlite3_malloc(Size + 1);
    if (zFuncName) {
      memcpy(zFuncName, Data, Size);
      zFuncName[Size] = '\0';
      /* Ensure valid function name (alphanumeric) */
      for (int i = 0; i < Size; i++) {
        if (!((zFuncName[i] >= 'a' && zFuncName[i] <= 'z') || 
              (zFuncName[i] >= 'A' && zFuncName[i] <= 'Z') ||
              (zFuncName[i] >= '0' && zFuncName[i] <= '9') ||
              zFuncName[i] == '_')) {
          zFuncName[i] = 'a';
        }
      }
      
      /* Try to overload with different argument counts */
      int nArg = (Size % 10); 
      rc = sqlite3_overload_function(db, zFuncName, nArg);
      (void)rc;
      
      sqlite3_free(zFuncName);
    }
  }

  sqlite3_free(zSqlBuf);
  sqlite3_close(db);
  return 0;
}