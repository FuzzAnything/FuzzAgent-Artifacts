// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_log at sqlite3.c:33450:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

/* Helper function to test sqlite3_vmprintf with a specific format and arguments */
static void test_vmprintf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char *result = sqlite3_vmprintf(fmt, ap);
  if (result) {
    sqlite3_free(result);
  }
  va_end(ap);
}

/* Helper function to test sqlite3_vsnprintf with a specific format and arguments */
static void test_vsnprintf(int n, char *buf, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  sqlite3_vsnprintf(n, buf, fmt, ap);
  va_end(ap);
}

/* Helper function to test sqlite3_str_vappendf with a specific format and arguments */
static void test_str_vappendf(sqlite3_str *s, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  sqlite3_str_vappendf(s, fmt, ap);
  va_end(ap);
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  uint8_t op = Data[0] % 6;
  const uint8_t *payload = Data + 1;
  size_t payload_size = Size - 1;

  /* Ensure payload is null-terminated for string functions if needed */
  char *safe_str = NULL;
  if (payload_size > 0) {
    safe_str = malloc(payload_size + 1);
    if (safe_str) {
      memcpy(safe_str, payload, payload_size);
      safe_str[payload_size] = '\0';
    }
  }

  const char *fmt = safe_str && strlen(safe_str) > 0 ? safe_str : "%s";

  switch (op) {
    case 0: {
      /* Test sqlite3_vmprintf */
      /* Test with NULL format (API armor) */
      {
        va_list ap;
        /* Initialize va_list manually since we are not in a varargs function */
        /* We cannot safely use va_start here without a dummy varargs function */
        /* Instead, we create a dummy function to test NULL format */
        test_vmprintf(NULL);
      }
      
      /* Test with valid format and simple args */
      test_vmprintf("%s", "test");
      test_vmprintf("%d", 123);
      test_vmprintf("%f", 1.23);
      test_vmprintf("%s %d", "hello", 42);
      
      /* Test with potentially malicious format string from input */
      test_vmprintf(fmt, "fuzz_arg");
      break;
    }
    case 1: {
      /* Test sqlite3_vsnprintf */
      char buf[256];
      
      /* Test with NULL buffer */
      {
        va_list ap;
        /* We cannot use va_start here, so we use a helper or direct call */
        /* Create a dummy varargs function to test this */
        test_vsnprintf(10, NULL, fmt);
      }
      
      /* Test with NULL format */
      {
        test_vsnprintf(10, buf, NULL);
      }
      
      /* Test with size <= 0 */
      {
        test_vsnprintf(0, buf, fmt);
        test_vsnprintf(-1, buf, fmt);
      }
      
      /* Test with valid inputs */
      test_vsnprintf(256, buf, "%s", "test");
      test_vsnprintf(256, buf, "%d", 123);
      test_vsnprintf(256, buf, fmt, "fuzz_arg");
      break;
    }
    case 2: {
      /* Test sqlite3_snprintf */
      char buf[256];
      
      /* Test with NULL buffer */
      sqlite3_snprintf(10, NULL, fmt);
      
      /* Test with NULL format */
      sqlite3_snprintf(10, buf, NULL);
      
      /* Test with size <= 0 */
      sqlite3_snprintf(0, buf, fmt);
      sqlite3_snprintf(-1, buf, fmt);
      
      /* Test with valid inputs */
      sqlite3_snprintf(256, buf, "%s", "test");
      sqlite3_snprintf(256, buf, "%d", 123);
      sqlite3_snprintf(256, buf, fmt, "fuzz_arg");
      break;
    }
    case 3: {
      /* Test sqlite3_str_vappendf */
      sqlite3_str *str = sqlite3_str_new(NULL);
      if (!str) break;
      
      /* Test with NULL format */
      {
        test_str_vappendf(str, NULL);
      }
      
      /* Test with valid format */
      test_str_vappendf(str, "%s", "test");
      test_str_vappendf(str, "%d", 123);
      test_str_vappendf(str, fmt, "fuzz_arg");
      
      /* Clean up */
      sqlite3_str_finish(str);
      break;
    }
    case 4: {
      /* Test sqlite3_log */
      /* Test with NULL format */
      sqlite3_log(SQLITE_OK, NULL);
      
      /* Test with valid format */
      sqlite3_log(SQLITE_OK, "%s", "test");
      sqlite3_log(SQLITE_OK, "%d", 123);
      sqlite3_log(SQLITE_OK, fmt, "fuzz_arg");
      break;
    }
    case 5: {
      /* Test sqlite3_mprintf */
      /* Test with NULL format */
      char *result = sqlite3_mprintf(NULL);
      if (result) sqlite3_free(result);
      
      /* Test with valid format */
      result = sqlite3_mprintf("%s", "test");
      if (result) sqlite3_free(result);
      
      result = sqlite3_mprintf("%d", 123);
      if (result) sqlite3_free(result);
      
      result = sqlite3_mprintf("%f", 1.23);
      if (result) sqlite3_free(result);
      
      result = sqlite3_mprintf(fmt, "fuzz_arg");
      if (result) sqlite3_free(result);
      
      break;
    }
  }

  if (safe_str) free(safe_str);
  return 0;
}