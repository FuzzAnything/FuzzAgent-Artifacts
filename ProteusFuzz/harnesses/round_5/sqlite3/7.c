// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Invokes functions in the specified order:
 * sqlite3_complete -> sqlite3_free -> sqlite3_close -> sqlite3_memory_highwater
 * -> sqlite3_memory_used -> sqlite3_status -> sqlite3_memory_used
 * -> sqlite3_hard_heap_limit64 -> sqlite3_soft_heap_limit64
 */

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
  /*
   * 1. Prepare the environment.
   * We need a valid SQL string for sqlite3_complete.
   * We also need an sqlite3* connection for sqlite3_close.
   * We might need a pointer for sqlite3_free (allocated via sqlite3_malloc).
   */

  /* Create a null-terminated SQL string from the input data.
   * Ensure it is valid UTF-8 by replacing null bytes if necessary,
   * though sqlite3_complete handles the string until the first null.
   * We limit the length to avoid excessive allocation or stack issues.
   */
  size_t max_len = Size;
  if (max_len > 1024) {
    max_len = 1024;
  }
  
  /* Allocate buffer for SQL string */
  char *sql_buf = (char *)sqlite3_malloc(max_len + 1);
  if (!sql_buf) {
    return 0; /* Allocation failed, nothing to do */
  }
  
  /* Copy data and ensure null termination */
  memcpy(sql_buf, Data, max_len);
  sql_buf[max_len] = '\0';

  /* Open a database connection for sqlite3_close */
  sqlite3 *db = NULL;
  /* Use a dummy file or in-memory database */
  int rc = sqlite3_open(":memory:", &db);
  /* Ignore rc for fuzzing purposes, we just need a handle or NULL */

  /* Allocate some memory to test sqlite3_free */
  void *mem_to_free = sqlite3_malloc(64);

  /*
   * 2. Invoke the target functions in the specified order.
   */

  /* 1. sqlite3_complete */
  int complete_result = sqlite3_complete(sql_buf);
  (void)complete_result; /* Suppress unused warning */

  /* 2. sqlite3_free */
  sqlite3_free(mem_to_free);
  /* Also free the sql_buf */
  sqlite3_free(sql_buf);

  /* 3. sqlite3_close */
  int close_result = sqlite3_close(db);
  (void)close_result; /* Suppress unused warning */

  /* 4. sqlite3_memory_highwater */
  sqlite3_int64 highwater = sqlite3_memory_highwater(0);
  (void)highwater;

  /* 5. sqlite3_memory_used */
  sqlite3_int64 mem_used_1 = sqlite3_memory_used();
  (void)mem_used_1;

  /* 6. sqlite3_status */
  int current_val = 0;
  int highwater_val = 0;
  int status_result = sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current_val, &highwater_val, 0);
  (void)status_result;

  /* 7. sqlite3_memory_used (again) */
  sqlite3_int64 mem_used_2 = sqlite3_memory_used();
  (void)mem_used_2;

  /* 8. sqlite3_hard_heap_limit64 */
  /* Use input data to derive a limit, ensuring it's positive or -1 */
  sqlite3_int64 hard_limit = -1;
  if (Size > 0) {
    /* Use first byte as a seed, ensure non-negative for setting, or -1 for query */
    if (Data[0] % 2 == 0) {
      hard_limit = (sqlite3_int64)Data[0] * 1024; /* Set a small limit */
    } else {
      hard_limit = -1; /* Query */
    }
  }
  sqlite3_int64 prev_hard = sqlite3_hard_heap_limit64(hard_limit);
  (void)prev_hard;

  /* 9. sqlite3_soft_heap_limit64 */
  sqlite3_int64 soft_limit = -1;
  if (Size > 1) {
    if (Data[1] % 2 == 0) {
      soft_limit = (sqlite3_int64)Data[1] * 1024;
    } else {
      soft_limit = -1;
    }
  }
  sqlite3_int64 prev_soft = sqlite3_soft_heap_limit64(soft_limit);
  (void)prev_soft;

  /*
   * 3. Explore more program states (optional diversification).
   * The above sequence covers the required order.
   * We could vary the limits or open/close different DBs, but the
   * input data already provides variation for the limits.
   */

  /*
   * 4. Cleanup.
   * sqlite3_close was already called.
   * sqlite3_free was called on allocated buffers.
   * No other persistent resources are held.
   */

  return 0;
}