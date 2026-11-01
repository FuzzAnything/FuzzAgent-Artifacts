// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3 API functions:
 * sqlite3_errcode -> sqlite3_mprintf -> sqlite3_errmsg -> sqlite3_extended_errcode
 */

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  int rc;
  char *zMem = NULL;
  sqlite3_stmt *stmt = NULL;

  /* Initialize the database connection using a dummy file */
  rc = sqlite3_open("./dummy_file", &db);
  if (rc != SQLITE_OK) {
    /* If open fails, we cannot proceed with valid db handle tests effectively.
       Close if partially initialized and exit. */
    if (db) {
      sqlite3_close(db);
    }
    return 0;
  }

  /* Attempt to trigger an error state by preparing/executing potentially invalid SQL */
  if (Size > 0) {
    /* Create a safe, null-terminated string from input data for SQL execution */
    char *safe_sql = (char *)sqlite3_malloc(Size + 1);
    if (safe_sql) {
      memcpy(safe_sql, Data, Size);
      safe_sql[Size] = '\0';

      /* Try to prepare the statement. This might fail, setting error codes. */
      rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
      
      /* If preparation succeeded, try to step to potentially trigger runtime errors */
      if (stmt) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
      }
      sqlite3_free(safe_sql);
    }
  }

  /* 1. Invoke sqlite3_errcode */
  int err_code = sqlite3_errcode(db);
  (void)err_code;

  /* 2. Invoke sqlite3_mprintf */
  /* Use input data to influence the format or argument safely */
  if (Size > 0) {
    int fuzz_val = (int)Data[0];
    zMem = sqlite3_mprintf("fuzz_val=%d", fuzz_val);
  } else {
    zMem = sqlite3_mprintf("default");
  }

  /* 3. Invoke sqlite3_errmsg */
  const char *err_msg = sqlite3_errmsg(db);
  (void)err_msg;

  /* 4. Invoke sqlite3_extended_errcode */
  int ext_err_code = sqlite3_extended_errcode(db);
  (void)ext_err_code;

  /* Cleanup */
  if (zMem) {
    sqlite3_free(zMem);
  }

  if (db) {
    sqlite3_close(db);
  }

  return 0;
}