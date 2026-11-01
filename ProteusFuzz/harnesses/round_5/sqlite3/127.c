// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *zSql = NULL;
  char *safeSql = NULL;
  int rc;

  if (Size == 0) {
    return 0;
  }

  /* Create a dummy file to use for opening a database if needed */
  FILE *f = fopen("./dummy_file", "wb");
  if (f) {
    fwrite(Data, 1, Size, f);
    fclose(f);
  }

  rc = sqlite3_open("./dummy_file", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Ensure SQL is null-terminated */
  if (Size > 0 && Data[Size - 1] != '\0') {
    safeSql = (char *)malloc(Size + 1);
    if (!safeSql) {
      sqlite3_close(db);
      return 0;
    }
    memcpy(safeSql, Data, Size);
    safeSql[Size] = '\0';
    zSql = safeSql;
  } else {
    zSql = (const char *)Data;
  }

  /* Prepare a statement to potentially trigger errors */
  rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    /* Error state set, proceed to test error APIs */
  } else {
    /* Execute step to potentially trigger errors */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
      /* Error state set */
    }
  }

  /* Test sqlite3_errmsg */
  (void)sqlite3_errmsg(db);

  /* Test sqlite3_errmsg16 */
  (void)sqlite3_errmsg16(db);

  /* Test sqlite3_errcode */
  (void)sqlite3_errcode(db);

  /* Test sqlite3_extended_errcode */
  (void)sqlite3_extended_errcode(db);

  /* Test sqlite3_db_handle with a valid statement */
  if (stmt != NULL) {
    (void)sqlite3_db_handle(stmt);
  }

  /* Test sqlite3_db_handle with NULL */
  (void)sqlite3_db_handle(NULL);

  /* Test error APIs with NULL database handle */
  (void)sqlite3_errmsg(NULL);
  (void)sqlite3_errmsg16(NULL);
  (void)sqlite3_errcode(NULL);
  (void)sqlite3_extended_errcode(NULL);

  /* Cleanup */
  if (stmt != NULL) {
    sqlite3_finalize(stmt);
  }
  sqlite3_close(db);
  if (safeSql) {
    free(safeSql);
  }

  return 0;
}