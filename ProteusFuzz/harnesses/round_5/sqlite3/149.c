// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 string manipulation APIs.
 * Target APIs:
 * - sqlite3_str_new
 * - sqlite3_str_appendf
 * - sqlite3_str_value
 * - sqlite3_str_finish
 * - sqlite3_str_free
 * - sqlite3_str_reset
 *
 * Fix for ASAN error:
 * The error occurred because sqlite3_str_appendf was called on an empty string
 * with a format specifier that requires memory allocation (like %c) without
 * prior initialization or when the internal state was inconsistent.
 * We ensure we always start with a valid state and check for errors after operations.
 */

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  /*
   * Open a database connection.
   * We use ":memory:" for simplicity.
   */
  sqlite3 *db = NULL;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    /* If we cannot open a database, we can still test with NULL db pointer */
    db = NULL;
  }

  /*
   * Create a dynamic string object.
   */
  sqlite3_str *str = sqlite3_str_new(db);
  if (str == NULL) {
    if (db) sqlite3_close(db);
    return 0;
  }

  /*
   * Use the input data to drive various string operations.
   */
  size_t idx = 0;
  while (idx < Size) {
    uint8_t cmd = Data[idx++];

    if (cmd == 0) {
      /*
       * Reset the string.
       */
      sqlite3_str_reset(str);
    } else if (cmd == 1) {
      /*
       * Append a fixed string "test".
       */
      sqlite3_str_appendf(str, "test");
    } else if (cmd == 2) {
      /*
       * Append a single character from input.
       * Ensure we have data.
       */
      if (idx < Size) {
        char c = (char)Data[idx++];
        sqlite3_str_appendf(str, "%c", c);
      }
    } else if (cmd == 3) {
      /*
       * Append an integer from input.
       */
      int32_t val = 0;
      if (idx + 4 <= Size) {
        memcpy(&val, &Data[idx], 4);
        idx += 4;
        sqlite3_str_appendf(str, "%d", val);
      }
    } else if (cmd == 4) {
      /*
       * Append a substring from input as a format string.
       * We limit the length to avoid huge allocations.
       */
      size_t len = 0;
      if (idx < Size) {
        len = Data[idx++];
      }
      if (len > 64) len = 64;
      if (idx + len <= Size) {
        /*
         * We need a null-terminated string for the format.
         * We copy it to a buffer and ensure it's null-terminated.
         */
        char buf[65];
        memcpy(buf, &Data[idx], len);
        buf[len] = '\0';
        idx += len;
        sqlite3_str_appendf(str, "%s", buf);
      }
    } else if (cmd == 5) {
      /*
       * Append a variable number of characters.
       */
      uint8_t count = 0;
      char ch = 'a';
      if (idx < Size) {
        count = Data[idx++];
      }
      if (idx < Size) {
        ch = (char)Data[idx++];
      }
      if (count > 1000) count = 1000;
      for (uint8_t i = 0; i < count; i++) {
        sqlite3_str_appendf(str, "%c", ch);
      }
    } else if (cmd == 6) {
      /*
       * Truncate the string to a given length.
       */
      int32_t len = 0;
      if (idx + 4 <= Size) {
        memcpy(&len, &Data[idx], 4);
        idx += 4;
        sqlite3_str_truncate(str, (uint32_t)len);
      }
    } else if (cmd == 7) {
      /*
       * Get the current value (adds null terminator).
       */
      char *val = sqlite3_str_value(str);
      (void)val; /* Just to exercise the API */
    } else {
      /*
       * Default: append a byte as a character.
       */
      if (idx < Size) {
        char c = (char)Data[idx++];
        sqlite3_str_appendf(str, "%c", c);
      }
    }

    /*
     * Check for errors after operations.
     * If an error occurred, we should stop appending to avoid undefined behavior.
     */
    int err = sqlite3_str_errcode(str);
    if (err != SQLITE_OK) {
      /*
       * If there's an error, we can either reset or stop.
       * Here we reset to continue testing.
       */
      sqlite3_str_reset(str);
    }
  }

  /*
   * Finalize the string.
   * We try both sqlite3_str_finish and sqlite3_str_free to cover different paths.
   */
  if (Size > 0 && Data[Size - 1] % 2 == 0) {
    char *result = sqlite3_str_finish(str);
    if (result) {
      sqlite3_free(result);
    }
  } else {
    sqlite3_str_free(str);
  }

  /*
   * Clean up the database connection if we opened one.
   */
  if (db) {
    sqlite3_close(db);
  }

  return 0;
}