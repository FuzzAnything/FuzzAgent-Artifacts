// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sqlite3.h>

static void test_variadic(const char *format, int arg1, int arg2, const char *arg3, ...) {
  va_list args;
  va_start(args, arg3);
  
  // Test sqlite3_vmprintf
  va_list args_copy1;
  va_copy(args_copy1, args);
  char *vmprintf_result = sqlite3_vmprintf(format, args_copy1);
  va_end(args_copy1);
  if (vmprintf_result) {
    sqlite3_free(vmprintf_result);
  }

  // Test sqlite3_vsnprintf
  va_list args_copy2;
  va_copy(args_copy2, args);
  char buf_vsnprintf[1024];
  sqlite3_vsnprintf(sizeof(buf_vsnprintf), buf_vsnprintf, format, args_copy2);
  va_end(args_copy2);

  // Test sqlite3_str_vappendf
  sqlite3_str *str = sqlite3_str_new(NULL);
  if (str) {
    va_list args_copy3;
    va_copy(args_copy3, args);
    sqlite3_str_vappendf(str, format, args_copy3);
    va_end(args_copy3);
    sqlite3_str_finish(str);
  }
  
  va_end(args);
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  // Initialize SQLite
  sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);

  // Create a dummy database connection
  sqlite3 *db = NULL;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    return 0;
  }

  // Ensure null termination for format strings
  char *format_buf = (char *)malloc(Size + 1);
  if (!format_buf) {
    sqlite3_close(db);
    return 0;
  }
  memcpy(format_buf, Data, Size);
  format_buf[Size] = '\0';

  // Test sqlite3_snprintf
  char buf_snprintf[1024];
  sqlite3_snprintf(sizeof(buf_snprintf), buf_snprintf, format_buf, 123, 456);

  // Test sqlite3_mprintf
  char *mprintf_result = sqlite3_mprintf(format_buf, 789, 101112);
  if (mprintf_result) {
    sqlite3_free(mprintf_result);
  }

  // Test variadic functions using helper
  test_variadic(format_buf, 123, 456, "test", 789);

  // Test sqlite3_log
  // Disable logging to avoid issues
  sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
  sqlite3_log(SQLITE_ERROR, format_buf, 1, 2, 3);

  // Cleanup
  sqlite3_close(db);
  free(format_buf);

  return 0;
}