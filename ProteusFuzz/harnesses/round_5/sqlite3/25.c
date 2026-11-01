// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  int rc;

  /* Open an in-memory database */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Create a simple table to ensure we have something to query */
  const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
  rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  /* Prepare a SELECT statement */
  const char *select_sql = "SELECT id, data FROM test_table;";
  rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK || stmt == NULL) {
    sqlite3_close(db);
    return 0;
  }

  /* 1. sqlite3_realloc: Allocate some memory to test realloc */
  void *ptr = sqlite3_realloc(NULL, 10);
  if (ptr != NULL) {
    /* Try to resize based on input data if available */
    int new_size = (Size > 0 && Data[0] < 1000) ? Data[0] : 20;
    void *new_ptr = sqlite3_realloc(ptr, new_size);
    if (new_ptr != NULL) {
      /* Use the memory briefly */
      memset(new_ptr, 0, new_size);
      ptr = new_ptr;
    }
  }

  /* 2. sqlite3_step: Execute the statement */
  rc = sqlite3_step(stmt);

  /* 3. sqlite3_column_count: Get column count */
  int col_count = sqlite3_column_count(stmt);
  (void)col_count; /* Use variable to avoid unused warning */

  /* 4. sqlite3_mprintf: Format a string */
  char *formatted = NULL;
  if (Size > 0) {
    formatted = sqlite3_mprintf("Value: %d, Size: %zu", col_count, Size);
  } else {
    formatted = sqlite3_mprintf("Default");
  }

  /* 5. sqlite3_column_text: Get column text if we have a row */
  if (rc == SQLITE_ROW && col_count > 0) {
    const unsigned char *text = sqlite3_column_text(stmt, 0);
    (void)text; /* Use variable to avoid unused warning */
  }

  /* 6. sqlite3_free: Free the allocated memory from mprintf and realloc */
  if (formatted != NULL) {
    sqlite3_free(formatted);
  }
  if (ptr != NULL) {
    sqlite3_free(ptr);
  }

  /* 7. sqlite3_reset: Reset the statement for potential reuse */
  rc = sqlite3_reset(stmt);
  (void)rc; /* Check return value as per documentation */

  /* Cleanup */
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return 0;
}