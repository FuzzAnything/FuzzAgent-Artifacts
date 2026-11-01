// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

/* Static helper to create a valid sqlite3_str object for testing */
static sqlite3_str *create_str_object(void) {
  return sqlite3_str_new(NULL);
}

/* Static helper to log a message for testing purposes */
static void dummy_log_handler(void *pArg, int iErrCode, const char *zMsg) {
  /* No-op handler */
}

/* Helper function to safely call sqlite3_vsnprintf with variadic args */
static void test_vsnprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buffer[256];
  sqlite3_vsnprintf(sizeof(buffer), buffer, format, args);
  va_end(args);
}

/* Helper function to safely call sqlite3_vmprintf with variadic args */
static void test_vmprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *result = sqlite3_vmprintf(format, args);
  va_end(args);
  if (result) {
    sqlite3_free(result);
  }
}

/* Helper function to safely call sqlite3_str_vappendf with variadic args */
static void test_str_vappendf(sqlite3_str *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  sqlite3_str_vappendf(str, format, args);
  va_end(args);
}

/* Variadic wrappers to allow va_start usage */
static void run_vsnprintf_test(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buffer[256];
  sqlite3_vsnprintf(sizeof(buffer), buffer, format, args);
  va_end(args);
}

static void run_vmprintf_test(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *result = sqlite3_vmprintf(format, args);
  va_end(args);
  if (result) {
    sqlite3_free(result);
  }
}

static void run_str_vappendf_test(sqlite3_str *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  sqlite3_str_vappendf(str, format, args);
  va_end(args);
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  /* Initialize SQLite if not already done */
  sqlite3_config(SQLITE_CONFIG_LOG, dummy_log_handler, NULL);

  /* Create a dummy database connection for context if needed */
  sqlite3 *db = NULL;
  sqlite3_open(":memory:", &db);

  /* Create a sqlite3_str object */
  sqlite3_str *str = create_str_object();

  /* Ensure we have a valid format string from input or use a default */
  char safe_format[256];
  size_t copy_len = (Size < sizeof(safe_format) - 1) ? Size : sizeof(safe_format) - 1;
  memcpy(safe_format, Data, copy_len);
  safe_format[copy_len] = '\0';
  
  /* Ensure the format string is valid for printf-like functions */
  const char *format_str = safe_format;

  /* Test sqlite3_log */
  sqlite3_log(SQLITE_ERROR, format_str, "test", 123);

  /* Test sqlite3_mprintf */
  char *mprintf_result = sqlite3_mprintf(format_str, "test", 456);
  if (mprintf_result) {
    sqlite3_free(mprintf_result);
  }

  /* Test sqlite3_snprintf */
  char buffer[256];
  sqlite3_snprintf(sizeof(buffer), buffer, format_str, "test", 789);

  /* Test sqlite3_vsnprintf via helper */
  run_vsnprintf_test(format_str, 1, 2);

  /* Test sqlite3_vmprintf via helper */
  run_vmprintf_test(format_str, 3, 4);

  /* Test sqlite3_str_vappendf via helper */
  if (str) {
    run_str_vappendf_test(str, format_str, 5, 6);
    
    /* Finish the string to clean up */
    sqlite3_str_finish(str);
  }

  /* Cleanup */
  if (db) {
    sqlite3_close(db);
  }

  return 0;
}