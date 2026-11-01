// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:93692:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_strlike at sqlite3.c:133222:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  sqlite3_str *str = NULL;
  const char *zPattern = NULL;
  const char *zString = NULL;
  unsigned int cEsc = 0;
  int rc;

  if (Size < 2) {
    return 0;
  }

  // Initialize database in memory
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  // Create a simple table to ensure we can prepare a SELECT statement
  const char *createSQL = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);";
  rc = sqlite3_exec(db, createSQL, NULL, NULL, NULL);
  (void)rc;

  // Prepare a SELECT statement to get a valid stmt for column functions
  const char *selectSQL = "SELECT id, name FROM test;";
  rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);

  // 1. sqlite3_strlike
  // We must ensure the strings passed are null-terminated and within bounds.
  // Fuzz input is not guaranteed to be null-terminated.
  // We will create temporary buffers with a safe length limit.
  size_t maxLen = Size > 1024 ? 1024 : Size;
  char *safePattern = NULL;
  char *safeString = NULL;

  if (Size >= 2) {
    // Find the first null byte to determine safe length for pattern
    size_t pLen = 0;
    while (pLen < Size - 1 && Data[pLen] != 0) {
      pLen++;
    }
    if (pLen > 0) {
      safePattern = (char *)sqlite3_malloc(pLen + 1);
      if (safePattern) {
        memcpy(safePattern, Data, pLen);
        safePattern[pLen] = '\0';
      }
    }

    // Find the first null byte for string starting from offset 1 or where pattern ended
    size_t sStart = (pLen > 0) ? pLen + 1 : 1;
    if (sStart < Size) {
      size_t sLen = 0;
      while (sStart + sLen < Size && Data[sStart + sLen] != 0) {
        sLen++;
      }
      if (sLen > 0) {
        safeString = (char *)sqlite3_malloc(sLen + 1);
        if (safeString) {
          memcpy(safeString, Data + sStart, sLen);
          safeString[sLen] = '\0';
        }
      }
    }
    
    cEsc = Data[Size - 1];
  }

  // Fallback to defaults if allocation failed or input was insufficient
  if (!safePattern) safePattern = "test";
  if (!safeString) safeString = "test";

  rc = sqlite3_strlike(safePattern, safeString, cEsc);
  (void)rc;

  // 2. sqlite3_column_count
  if (stmt) {
    rc = sqlite3_column_count(stmt);
    (void)rc;
  }

  // 3. sqlite3_column_name (first call)
  if (stmt) {
    const char *name = sqlite3_column_name(stmt, 0);
    (void)name;
  }

  // 4. sqlite3_column_name (second call)
  if (stmt) {
    const char *name = sqlite3_column_name(stmt, 1);
    (void)name;
  }

  // 5. sqlite3_stricmp
  // Ensure null termination for stricmp as well
  if (Size >= 2) {
    rc = sqlite3_stricmp(safePattern, safeString);
  } else {
    rc = sqlite3_stricmp("hello", "HELLO");
  }
  (void)rc;

  // 6. sqlite3_column_type
  if (stmt) {
    rc = sqlite3_column_type(stmt, 0);
    (void)rc;
  }

  // 7. sqlite3_str_appendf (first call)
  str = sqlite3_str_new(db);
  if (str) {
    if (Size > 0) {
      sqlite3_str_appendf(str, "Data: %d", Data[0]);
    } else {
      sqlite3_str_appendf(str, "Empty");
    }
  }

  // 8. sqlite3_str_appendf (second call)
  if (str) {
    if (Size > 1) {
      sqlite3_str_appendf(str, " %d", Data[1]);
    } else {
      sqlite3_str_appendf(str, " Done");
    }
  }

  // 9. sqlite3_finalize
  if (stmt) {
    rc = sqlite3_finalize(stmt);
    stmt = NULL; // Prevent reuse
    (void)rc;
  }

  // 10. sqlite3_str_appendf (third call, after finalize but str is still valid)
  if (str) {
    sqlite3_str_appendf(str, " Final");
  }

  // 11. sqlite3_str_finish
  if (str) {
    char *result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
    str = NULL;
  }

  // Cleanup database
  sqlite3_close(db);
  
  // Free temporary buffers using sqlite3_free to match allocation
  // Only free if they were successfully allocated (non-NULL)
  if (safePattern != NULL) {
    sqlite3_free(safePattern);
    safePattern = NULL;
  }
  if (safeString != NULL) {
    sqlite3_free(safeString);
    safeString = NULL;
  }

  return 0;
}