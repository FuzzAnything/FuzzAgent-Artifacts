// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_double at sqlite3.c:92472:19 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
  (void)Data;
  (void)Size;

  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, val REAL, txt TEXT); "
                    "INSERT INTO test VALUES (1, 1.5, 'hello'); "
                    "INSERT INTO test VALUES (2, 2.5, 'world'); "
                    "SELECT * FROM test;";

  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    return 0;
  }

  if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  if (sqlite3_prepare_v2(db, "SELECT * FROM test", -1, &stmt, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    // Column 0: INTEGER
    {
      sqlite3_value *val = sqlite3_column_value(stmt, 0);
      if (val) {
        sqlite3_value *dup = sqlite3_value_dup(val);
        if (dup) {
          double d = sqlite3_value_double(dup);
          (void)d;
          const void *t16 = sqlite3_value_text16(dup);
          (void)t16;
          const void *t16be = sqlite3_value_text16be(dup);
          (void)t16be;
          const void *t16le = sqlite3_value_text16le(dup);
          (void)t16le;
          sqlite3_value_free(dup);
        }
      }
    }

    // Column 1: REAL
    {
      sqlite3_value *val = sqlite3_column_value(stmt, 1);
      if (val) {
        sqlite3_value *dup = sqlite3_value_dup(val);
        if (dup) {
          double d = sqlite3_value_double(dup);
          (void)d;
          const void *t16 = sqlite3_value_text16(dup);
          (void)t16;
          const void *t16be = sqlite3_value_text16be(dup);
          (void)t16be;
          const void *t16le = sqlite3_value_text16le(dup);
          (void)t16le;
          sqlite3_value_free(dup);
        }
      }
    }

    // Column 2: TEXT
    {
      sqlite3_value *val = sqlite3_column_value(stmt, 2);
      if (val) {
        sqlite3_value *dup = sqlite3_value_dup(val);
        if (dup) {
          double d = sqlite3_value_double(dup);
          (void)d;
          const void *t16 = sqlite3_value_text16(dup);
          (void)t16;
          const void *t16be = sqlite3_value_text16be(dup);
          (void)t16be;
          const void *t16le = sqlite3_value_text16le(dup);
          (void)t16le;
          sqlite3_value_free(dup);
        }
      }
    }
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}