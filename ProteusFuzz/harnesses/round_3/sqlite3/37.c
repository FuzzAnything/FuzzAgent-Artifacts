// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 error reporting and formatting functions.
 * Target sequence: sqlite3_errcode -> sqlite3_mprintf -> sqlite3_errmsg -> sqlite3_extended_errcode
 */

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  int rc;
  const char *errmsg;
  int errcode;
  int ext_errcode;
  char *fmt_str = NULL;

  /*
   * Step 1: Prepare the environment.
   * Initialize the database connection. We use ":memory:" to avoid file system dependencies
   * and ensure a clean state for each fuzzing iteration.
   */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    /* If opening fails, we can still test error functions with NULL or invalid handles,
     * but for this specific sequence, we prefer a valid handle to trigger real error states.
     * If open fails, we just return early to avoid undefined behavior with NULL db. */
    return 0;
  }

  /*
   * To ensure we have an error state to report, we attempt to execute an invalid SQL statement.
   * We use the fuzzing input as the SQL string if it looks like text, or a fixed invalid query.
   */
  const char *sql_query = "SELECT INVALID_SYNTAX_FOR_FUZZING";
  
  /* Optionally, try to use input data as SQL if it's printable and not too large, 
     to trigger different parse errors. */
  if (Size > 0 && Size < 4096) {
    /* Create a temporary buffer to ensure null termination */
    char *safe_sql = malloc(Size + 1);
    if (safe_sql) {
      memcpy(safe_sql, Data, Size);
      safe_sql[Size] = '\0';
      /* Basic check to avoid binary garbage causing immediate crashes in parser logic 
         (though SQLite is robust, this helps focus on error handling paths) */
      if (strchr(safe_sql, 0) == safe_sql + Size) { 
        /* If it's pure text, use it. Otherwise stick to default. */
        /* Note: We don't strictly need to validate, SQLite handles garbage well, 
           but this adds diversity. */
        sql_query = safe_sql;
      }
      free(safe_sql);
    }
  }

  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    /*
     * We have a failed operation. Now we invoke the target functions in the specified order.
     */

    /* 1. sqlite3_errcode: Retrieve the numeric error code. */
    errcode = sqlite3_errcode(db);
    (void)errcode; /* Use the value to prevent unused variable warnings */

    /* 2. sqlite3_mprintf: Format a string using the error code.
     * This tests the formatting logic and memory allocation.
     * We use a format string that incorporates the error code.
     */
    if (Size > 0 && Data[0] != 0) {
        /* Use input data as part of the format string if safe, or just use fixed format */
        /* To be safe and avoid format string vulnerabilities in the fuzzer itself, 
           we use a fixed format and pass the error code. */
        fmt_str = sqlite3_mprintf("Error code: %d, Message: %s", errcode, sqlite3_errmsg(db));
    } else {
        fmt_str = sqlite3_mprintf("Error code: %d", errcode);
    }

    /* 3. sqlite3_errmsg: Retrieve the human-readable error message.
     * Note: The documentation says this is an error-code preserving interface.
     * Calling it here retrieves the message associated with the error.
     */
    errmsg = sqlite3_errmsg(db);
    (void)errmsg; /* Use the value */

    /* 4. sqlite3_extended_errcode: Retrieve the extended error code.
     * This provides more detailed diagnostics.
     */
    ext_errcode = sqlite3_extended_errcode(db);
    (void)ext_errcode; /* Use the value */

    /* Cleanup: Free the string allocated by sqlite3_mprintf */
    if (fmt_str) {
      sqlite3_free(fmt_str);
    }

    /* Finalize the statement if it was partially prepared */
    if (stmt) {
      sqlite3_finalize(stmt);
    }
  } else {
    /*
     * If the SQL executed successfully, we still need to test the functions.
     * In a success state, sqlite3_errcode returns SQLITE_OK (0) and errmsg returns NULL.
     * We invoke the sequence anyway to test the "no error" path.
     */
    errcode = sqlite3_errcode(db);
    (void)errcode;

    fmt_str = sqlite3_mprintf("Success code: %d", errcode);
    if (fmt_str) {
      sqlite3_free(fmt_str);
    }

    errmsg = sqlite3_errmsg(db);
    (void)errmsg;

    ext_errcode = sqlite3_extended_errcode(db);
    (void)ext_errcode;

    if (stmt) {
      sqlite3_finalize(stmt);
    }
  }

  /*
   * Step 2: Conduct cleanup.
   * Close the database connection.
   */
  rc = sqlite3_close(db);
  if (rc != SQLITE_OK) {
    /* sqlite3_close should always succeed if the connection is valid.
     * If it fails, it might be due to unfinished statements, but we finalized them.
     * We ignore this for fuzzing as long as we don't leak memory. */
  }

  return 0;
}