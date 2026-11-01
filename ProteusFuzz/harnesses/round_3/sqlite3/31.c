// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  int rc;

  /* 1. Prepare the environment */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Create table first to ensure valid schema */
  const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);";
  rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Prepare a statement with a parameter */
  const char *sql = "INSERT INTO test (val) VALUES (?);";
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK || stmt == NULL) {
    sqlite3_close(db);
    return 0;
  }

  /* 2. Invoke the target function sequence */

  /* sqlite3_reset: Reset the statement to initial state */
  rc = sqlite3_reset(stmt);
  (void)rc;

  /* sqlite3_bind_text: Bind data from fuzz input */
  if (Size > 0) {
    /* Use the fuzz data as the text to bind. 
       Ensure we don't read past bounds. 
       SQLITE_TRANSIENT tells sqlite to copy the data. */
    rc = sqlite3_bind_text(stmt, 1, (const char *)Data, (int)Size, SQLITE_TRANSIENT);
    (void)rc;
  }

  /* sqlite3_step: Execute the statement */
  rc = sqlite3_step(stmt);
  (void)rc;

  /* sqlite3_errmsg: Retrieve error message if any (uses db connection) */
  const char *errmsg = sqlite3_errmsg(db);
  (void)errmsg;

  /* sqlite3_reset: Reset again as per sequence */
  rc = sqlite3_reset(stmt);
  (void)rc;

  /* sqlite3_changes: Get number of rows modified */
  int changes = sqlite3_changes(db);
  (void)changes;

  /* 3. Cleanup */
  if (stmt != NULL) {
    sqlite3_finalize(stmt);
  }
  if (db != NULL) {
    sqlite3_close(db);
  }

  return 0;
}