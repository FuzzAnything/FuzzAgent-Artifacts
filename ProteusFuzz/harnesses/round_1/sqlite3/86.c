// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  int rc;

  /*
   * Step 1: Prepare the environment.
   * Initialize the database connection. We use an in-memory database to avoid
   * file system dependencies, but we also support a dummy file if needed.
   */
  if (Size > 0 && Data[0] == 1) {
    /* Use a dummy file if the first byte indicates it */
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
      fwrite(Data, 1, Size, f);
      fclose(f);
    }
    rc = sqlite3_open("./dummy_file", &db);
  } else {
    /* Default to in-memory database */
    rc = sqlite3_open(":memory:", &db);
  }

  /* If opening failed, we can still test error handling with NULL or invalid handles */
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    db = NULL;
  }

  /*
   * Step 3: Explore program states by invoking target functions diversely.
   * We use the input data to drive different code paths.
   */
  if (db != NULL) {
    /* Create a table to ensure WAL mode can be enabled and errors can occur */
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    /* Enable WAL mode to test sqlite3_wal_checkpoint_v2 */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    /*
     * Fuzz sqlite3_errmsg16
     * This function is safe to call even without errors, but we try to induce
     * errors first by running invalid SQL.
     */
    const char *bad_sql = "SELECT * FROM nonexistent_table;";
    sqlite3_exec(db, bad_sql, NULL, NULL, NULL);
    const void *errmsg16 = sqlite3_errmsg16(db);
    (void)errmsg16; /* Suppress unused variable warning */

    /*
     * Fuzz sqlite3_error_offset
     * This retrieves the offset of the error in the SQL statement.
     */
    int offset = sqlite3_error_offset(db);
    (void)offset;

    /*
     * Fuzz sqlite3_wal_checkpoint_v2
     * We use input data to select the checkpoint mode.
     */
    int eMode = SQLITE_CHECKPOINT_PASSIVE;
    if (Size > 1) {
      int mode_val = Data[1] % 5;
      switch (mode_val) {
        case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
        case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
        case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
        case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
        case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
      }
    }

    int pnLog = -1;
    int pnCkpt = -1;
    rc = sqlite3_wal_checkpoint_v2(db, NULL, eMode, &pnLog, &pnCkpt);
    (void)rc;
    (void)pnLog;
    (void)pnCkpt;

    /*
     * Fuzz sqlite3_errcode
     * Retrieves the numeric error code.
     */
    int errcode = sqlite3_errcode(db);
    (void)errcode;

    /*
     * Fuzz sqlite3_get_autocommit
     * Checks if the connection is in autocommit mode.
     */
    int autoCommit = sqlite3_get_autocommit(db);
    (void)autoCommit;

    /*
     * Fuzz sqlite3_extended_errcode
     * Retrieves extended error codes.
     */
    int extErrcode = sqlite3_extended_errcode(db);
    (void)extErrcode;

    /*
     * Additional exploration: Try to induce specific errors to test error reporting.
     */
    if (Size > 2) {
      int action = Data[2] % 3;
      if (action == 0) {
        /* Try to insert invalid data */
        sqlite3_exec(db, "INSERT INTO test (id, data) VALUES (1, NULL);", NULL, NULL, NULL);
      } else if (action == 1) {
        /* Try to execute a syntax error */
        sqlite3_exec(db, "SELEC * FROM test;", NULL, NULL, NULL);
      } else {
        /* Try to start a transaction */
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        autoCommit = sqlite3_get_autocommit(db);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
      }
    }

    /* Re-check error functions after inducing errors */
    errmsg16 = sqlite3_errmsg16(db);
    offset = sqlite3_error_offset(db);
    errcode = sqlite3_errcode(db);
    extErrcode = sqlite3_extended_errcode(db);
    autoCommit = sqlite3_get_autocommit(db);

    /*
     * Test with a specific database name if provided in input
     */
    if (Size > 4) {
      char dbName[32];
      size_t len = Size - 4;
      if (len > 31) len = 31;
      memcpy(dbName, Data + 4, len);
      dbName[len] = '\0';
      rc = sqlite3_wal_checkpoint_v2(db, dbName, eMode, &pnLog, &pnCkpt);
      (void)rc;
    }
  } else {
    /*
     * Test functions with NULL database handle
     * According to documentation, these should return specific error messages/codes.
     */
    const void *errmsg16 = sqlite3_errmsg16(NULL);
    (void)errmsg16;

    int offset = sqlite3_error_offset(NULL);
    (void)offset;

    int errcode = sqlite3_errcode(NULL);
    (void)errcode;

    int autoCommit = sqlite3_get_autocommit(NULL);
    (void)autoCommit;

    int extErrcode = sqlite3_extended_errcode(NULL);
    (void)extErrcode;

    int pnLog = -1;
    int pnCkpt = -1;
    int eMode = SQLITE_CHECKPOINT_PASSIVE;
    rc = sqlite3_wal_checkpoint_v2(NULL, NULL, eMode, &pnLog, &pnCkpt);
    (void)rc;
  }

  /*
   * Step 2: Cleanup
   * Close the database connection if it was opened.
   */
  if (db != NULL) {
    sqlite3_close(db);
  }

  return 0;
}