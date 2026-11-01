// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  sqlite3 *db = NULL;
  sqlite3_str *str = NULL;
  char *result = NULL;
  int rc;

  /* Initialize a database connection (in-memory) to potentially use limits */
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    /* If opening fails, we can still test sqlite3_str_new with NULL db */
    db = NULL;
  }

  /* Strategy 1: Create string with DB context */
  str = sqlite3_str_new(db);
  if (str) {
    /* Use fuzz data as format string and arguments */
    if (Size > 0) {
      /* Create a safe buffer for the format string, ensuring null termination */
      char *fmt = (char *)sqlite3_malloc(Size + 1);
      if (fmt) {
        memcpy(fmt, Data, Size);
        fmt[Size] = '\0';

        /* Test sqlite3_str_appendf with input as format string (no extra args) */
        sqlite3_str_appendf(str, fmt);
        
        /* Test sqlite3_str_appendall with input as raw string */
        sqlite3_str_appendall(str, fmt);
        
        sqlite3_free(fmt);
      }
    }

    /* Test sqlite3_str_reset */
    sqlite3_str_reset(str);

    /* Test sqlite3_str_value */
    char *val = sqlite3_str_value(str);
    (void)val; /* Suppress unused variable warning */

    /* Test sqlite3_str_finish */
    result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
  }

  /* Strategy 2: Create string without DB context (NULL) */
  str = sqlite3_str_new(NULL);
  if (str) {
    /* Append some data */
    if (Size > 0) {
      char *fmt = (char *)sqlite3_malloc(Size + 1);
      if (fmt) {
        memcpy(fmt, Data, Size);
        fmt[Size] = '\0';
        sqlite3_str_appendf(str, fmt);
        sqlite3_str_appendall(str, fmt);
        sqlite3_free(fmt);
      }
    }

    /* Reset */
    sqlite3_str_reset(str);

    /* Get value */
    char *val = sqlite3_str_value(str);
    (void)val;

    /* Finish */
    result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
  }

  /* Strategy 3: Test sqlite3_str_free directly */
  str = sqlite3_str_new(db);
  if (str) {
    /* Do nothing, just free */
    sqlite3_str_free(str);
  }

  /* Strategy 4: Test with empty input or specific patterns */
  str = sqlite3_str_new(db);
  if (str) {
    /* Append a fixed string to ensure non-empty state before finish */
    sqlite3_str_appendall(str, "test");
    result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
  }

  /* Cleanup database */
  if (db) {
    sqlite3_close(db);
  }

  return 0;
}