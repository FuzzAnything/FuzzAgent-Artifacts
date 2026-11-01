// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:93828:24 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *zTail = NULL;
  int rc;
  int col_count = 0;
  int i;

  if (Size == 0) {
    return 0;
  }

  // Open an in-memory database to avoid file I/O issues
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    return 0;
  }

  // Create a simple table to ensure we have something to query
  const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value INTEGER);";
  rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  // Insert some dummy data
  const char *insert_sql = "INSERT INTO test_table (name, value) VALUES ('fuzz', 123);";
  rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  // Prepare the SQL statement from the fuzz input
  // We use the input data as the SQL query.
  // If the input is not valid SQL, sqlite3_prepare_v2 will fail, which is fine for fuzzing.
  rc = sqlite3_prepare_v2(db, (const char *)Data, Size, &stmt, &zTail);

  if (rc == SQLITE_OK && stmt != NULL) {
    // Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      // Get column count
      col_count = sqlite3_column_count(stmt);
      if (col_count <= 0) {
        continue;
      }

      // Iterate over columns and test the target APIs
      for (i = 0; i < col_count; i++) {
        // Test sqlite3_column_name
        const char *col_name = sqlite3_column_name(stmt, i);
        (void)col_name; // Use the result to avoid unused variable warnings

        // Test sqlite3_column_int
        int col_int = sqlite3_column_int(stmt, i);
        (void)col_int;

        // Test sqlite3_column_decltype
        const char *col_decltype = sqlite3_column_decltype(stmt, i);
        (void)col_decltype;
      }
    }

    // Test sqlite3_sql on the prepared statement
    const char *sql_text = sqlite3_sql(stmt);
    (void)sql_text;

    // Finalize the statement
    sqlite3_finalize(stmt);
  }

  // Clean up
  sqlite3_close(db);

  return 0;
}