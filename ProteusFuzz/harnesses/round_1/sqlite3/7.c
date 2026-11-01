// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  sqlite3_str *str = NULL;
  char *result_str = NULL;

  // Initialize database in memory
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    return 0;
  }

  // Create a simple table to ensure we have a valid SELECT statement
  const char *create_sql = "CREATE TABLE test (id INTEGER, name TEXT);";
  if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  // Prepare a SELECT statement to use for column functions
  const char *select_sql = "SELECT id, name FROM test;";
  if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  // 1. sqlite3_strlike
  // Use input data as pattern and string if possible, or fallback to static strings
  const char *pattern = (Size > 4) ? (const char *)Data : "a%";
  const char *str_val = (Size > 4) ? (const char *)(Data + 4) : "abc";
  unsigned int escape_char = (Size > 8) ? Data[8] : 0;
  sqlite3_strlike(pattern, str_val, escape_char);

  // 2. sqlite3_column_count
  int count = sqlite3_column_count(stmt);
  (void)count; // Suppress unused warning

  // 3. sqlite3_column_name (first call)
  if (count > 0) {
    const char *name1 = sqlite3_column_name(stmt, 0);
    (void)name1;
  }

  // 4. sqlite3_column_name (second call)
  if (count > 1) {
    const char *name2 = sqlite3_column_name(stmt, 1);
    (void)name2;
  }

  // 5. sqlite3_stricmp
  const char *cmp1 = (Size > 10) ? (const char *)(Data + 10) : "Hello";
  const char *cmp2 = (Size > 15) ? (const char *)(Data + 15) : "hello";
  sqlite3_stricmp(cmp1, cmp2);

  // 6. sqlite3_column_type
  // We need to step the statement to get a row, or it might return SQLITE_NULL/undefined
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    if (count > 0) {
      int type = sqlite3_column_type(stmt, 0);
      (void)type;
    }
  }

  // 7. sqlite3_str_appendf (first call)
  str = sqlite3_str_new(db);
  if (str) {
    const char *fmt = (Size > 20 && Data[20] != 0) ? (const char *)(Data + 20) : "Value: %d";
    int val = (Size > 25) ? Data[25] : 42;
    sqlite3_str_appendf(str, fmt, val);
  }

  // 8. sqlite3_str_appendf (second call)
  if (str) {
    const char *fmt2 = (Size > 30 && Data[30] != 0) ? (const char *)(Data + 30) : " Text: %s";
    const char *txt = (Size > 35) ? (const char *)(Data + 35) : "test";
    sqlite3_str_appendf(str, fmt2, txt);
  }

  // 9. sqlite3_finalize
  // Finalize the statement as required by the sequence
  if (stmt) {
    sqlite3_finalize(stmt);
    stmt = NULL;
  }

  // 10. sqlite3_str_appendf (third call, after finalize)
  // Note: The sequence asks for this after finalize. 
  // This is valid for the string object, though the DB connection is still open.
  if (str) {
    const char *fmt3 = (Size > 40 && Data[40] != 0) ? (const char *)(Data + 40) : " End";
    sqlite3_str_appendf(str, fmt3);
  }

  // 11. sqlite3_str_finish
  if (str) {
    result_str = sqlite3_str_finish(str);
    if (result_str) {
      sqlite3_free(result_str);
    }
  }

  // Cleanup
  if (db) {
    sqlite3_close(db);
  }

  return 0;
}