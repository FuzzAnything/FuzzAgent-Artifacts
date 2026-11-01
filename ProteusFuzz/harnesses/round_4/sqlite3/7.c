// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  sqlite3_str *str = NULL;
  int rc;

  /* Initialize database connection */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Create a simple table and insert data */
  const char *sql = "CREATE TABLE test (id INTEGER, name TEXT); INSERT INTO test VALUES (1, 'fuzz');";
  rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Prepare a SELECT statement */
  const char *selectSql = "SELECT id, name FROM test;";
  rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
  if (rc != SQLITE_OK || stmt == NULL) {
    sqlite3_close(db);
    return 0;
  }

  /* Step through the statement */
  rc = sqlite3_step(stmt);

  /* 
   * Fix for heap-buffer-overflow: 
   * The fuzzer input 'Data' is not null-terminated. 
   * sqlite3_strlike expects null-terminated strings.
   * We create local buffers with explicit null-termination.
   */
  size_t safeSize = Size;
  if (safeSize > 1024) safeSize = 1024; /* Limit buffer size for safety */
  
  char *bufPattern = (char *)malloc(safeSize + 1);
  char *bufStr = (char *)malloc(safeSize + 1);
  
  if (!bufPattern || !bufStr) {
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    free(bufPattern);
    free(bufStr);
    return 0;
  }

  memcpy(bufPattern, Data, safeSize);
  bufPattern[safeSize] = '\0';
  memcpy(bufStr, Data, safeSize);
  bufStr[safeSize] = '\0';

  /* 1. sqlite3_strlike */
  unsigned int cEsc = (Size > 0) ? Data[0] : 0;
  sqlite3_strlike(bufPattern, bufStr, cEsc);

  /* 2. sqlite3_column_count */
  int count = sqlite3_column_count(stmt);
  int colIndex = 0;
  if (count > 0) {
    colIndex = (Size % (size_t)count);
  }

  /* 3. sqlite3_column_name (first call) */
  const char *name1 = sqlite3_column_name(stmt, colIndex);

  /* 4. sqlite3_column_name (second call) */
  const char *name2 = sqlite3_column_name(stmt, colIndex);

  /* 5. sqlite3_stricmp */
  if (name1 && name2) {
    sqlite3_stricmp(name1, name2);
  } else if (name1) {
    sqlite3_stricmp(name1, NULL);
  } else if (name2) {
    sqlite3_stricmp(NULL, name2);
  } else {
    sqlite3_stricmp(NULL, NULL);
  }

  /* 6. sqlite3_column_type */
  int type = sqlite3_column_type(stmt, colIndex);
  (void)type;

  /* 7. sqlite3_str_appendf (first call) */
  str = sqlite3_str_new(db);
  if (str) {
    if (Size > 0) {
      sqlite3_str_appendf(str, "Value: %d", (int)Data[0]);
    } else {
      sqlite3_str_appendf(str, "Empty");
    }
  }

  /* 8. sqlite3_str_appendf (second call) */
  if (str) {
    if (Size > 1) {
      sqlite3_str_appendf(str, " - %c", (char)Data[1]);
    } else {
      sqlite3_str_appendf(str, " - End");
    }
  }

  /* 9. sqlite3_finalize */
  rc = sqlite3_finalize(stmt);
  (void)rc;

  /* 10. sqlite3_str_appendf (third call, after finalize) */
  if (str) {
    sqlite3_str_appendf(str, " Post-finalize");
  }

  /* 11. sqlite3_str_finish */
  char *result = NULL;
  if (str) {
    result = sqlite3_str_finish(str);
  }

  /* Cleanup */
  if (result) {
    sqlite3_free(result);
  }
  sqlite3_close(db);
  free(bufPattern);
  free(bufStr);

  return 0;
}