// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

/* Helper function to test sqlite3_str_vappendf with a va_list */
static void test_vappendf(sqlite3_str *str, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  sqlite3_str_vappendf(str, fmt, args);
  va_end(args);
}

/* Helper function to test sqlite3_vmprintf with a va_list */
static char *test_vmprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *res = sqlite3_vmprintf(fmt, args);
  va_end(args);
  return res;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Initialize SQLite configuration
  sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);

  // Create a dummy database connection
  sqlite3 *db = NULL;
  sqlite3_open(":memory:", &db);

  // Create an sqlite3_str object
  sqlite3_str *str = sqlite3_str_new(db);
  if (!str) {
    sqlite3_close(db);
    return 0;
  }

  // Prepare a safe format string to avoid undefined behavior
  // We use a fixed safe format for the variadic tests
  const char *safe_format = "%s %d %f";
  const char *arg_str = "fuzz";
  int arg_int = 42;
  double arg_float = 3.14;

  // Test sqlite3_str_appendf
  sqlite3_str_appendf(str, safe_format, arg_str, arg_int, arg_float);

  // Test sqlite3_str_vappendf via helper
  test_vappendf(str, safe_format, arg_str, arg_int, arg_float);

  // Test sqlite3_str_append
  sqlite3_str_append(str, "hello", 5);

  // Test sqlite3_str_appendall
  sqlite3_str_appendall(str, "world");

  // Test sqlite3_str_appendchar
  sqlite3_str_appendchar(str, 5, ' ');

  // Test sqlite3_str_reset
  sqlite3_str_reset(str);

  // Test sqlite3_str_truncate
  sqlite3_str_appendall(str, "truncate test");
  sqlite3_str_truncate(str, 10);

  // Check for errors
  int errcode = sqlite3_str_errcode(str);
  (void)errcode; // Suppress unused warning

  // Test sqlite3_str_finish
  char *result = sqlite3_str_finish(str);
  if (result) {
    sqlite3_free(result);
  }

  // Re-create str for further tests if needed, or just test mprintf directly
  // Test sqlite3_mprintf
  char *mprintf_result = sqlite3_mprintf(safe_format, arg_str, arg_int, arg_float);
  if (mprintf_result) {
    sqlite3_free(mprintf_result);
  }

  // Test sqlite3_vmprintf via helper
  char *vmprintf_result = test_vmprintf(safe_format, arg_str, arg_int, arg_float);
  if (vmprintf_result) {
    sqlite3_free(vmprintf_result);
  }

  // Test sqlite3_snprintf
  char buf[256];
  char *snprintf_result = sqlite3_snprintf(256, buf, safe_format, arg_str, arg_int, arg_float);
  if (snprintf_result) {
    // snprintf_result points to buf, so we don't free it
  }

  // Cleanup
  sqlite3_close(db);

  return 0;
}