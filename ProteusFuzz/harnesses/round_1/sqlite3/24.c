// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
  (void)Data;
  (void)Size;

  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *filename = "./dummy_file";

  /* Initialize database connection */
  if (sqlite3_open(filename, &db) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Create a dummy table to ensure the statement can be prepared and executed */
  const char *createTableSQL = "CREATE TABLE IF NOT EXISTS dummy (id INTEGER PRIMARY KEY, data BLOB);";
  if (sqlite3_exec(db, createTableSQL, NULL, NULL, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Prepare a statement */
  const char *sql = "SELECT * FROM dummy;";
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Step 1: sqlite3_column_bytes */
  int bytes1 = sqlite3_column_bytes(stmt, 0);

  /* Step 2: sqlite3_column_blob */
  const void *blob1 = sqlite3_column_blob(stmt, 0);

  /* Step 3: sqlite3_column_bytes */
  int bytes2 = sqlite3_column_bytes(stmt, 1);

  /* Step 4: sqlite3_column_blob */
  const void *blob2 = sqlite3_column_blob(stmt, 1);

  /* Step 5: sqlite3_stmt_isexplain */
  int isExplain = sqlite3_stmt_isexplain(stmt);

  /* Step 6: sqlite3_reset */
  sqlite3_reset(stmt);

  /* Step 7: sqlite3_step */
  sqlite3_step(stmt);

  /* Step 8: sqlite3_errmsg */
  const char *errmsg1 = sqlite3_errmsg(db);

  /* Step 9: sqlite3_errmsg */
  const char *errmsg2 = sqlite3_errmsg(db);

  /* Step 10: sqlite3_finalize */
  sqlite3_finalize(stmt);

  /* Cleanup */
  sqlite3_close(db);

  /* Use variables to prevent unused variable warnings */
  (void)bytes1;
  (void)blob1;
  (void)bytes2;
  (void)blob2;
  (void)isExplain;
  (void)errmsg1;
  (void)errmsg2;

  return 0;
}