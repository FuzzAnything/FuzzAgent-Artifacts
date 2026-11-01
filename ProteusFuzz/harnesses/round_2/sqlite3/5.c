// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Ensure the input data is null-terminated for sqlite3_complete
  // We create a local buffer to hold the SQL string
  size_t sql_len = Size;
  if (sql_len > 1024 * 1024) { // Limit size to prevent DoS
    sql_len = 1024 * 1024;
  }
  
  // Allocate memory for the SQL string
  char *sql = (char *)sqlite3_malloc(sql_len + 1);
  if (!sql) {
    return 0;
  }
  memcpy(sql, Data, sql_len);
  sql[sql_len] = '\0';

  // 1. sqlite3_complete
  int complete = sqlite3_complete(sql);
  (void)complete; // Use return value to avoid warnings

  // 2. sqlite3_free
  sqlite3_free(sql);

  // 3. sqlite3_close
  // We need a valid sqlite3* to close. Since we didn't open one in this sequence,
  // we pass NULL which is safe according to documentation.
  // To explore more states, we could try to open a DB, but the prompt asks
  // to invoke specific functions in order. Passing NULL is the safest valid call here.
  sqlite3_close(NULL);

  // 4. sqlite3_memory_highwater
  // Reset flag: use data from input to vary behavior (0 or 1)
  int reset_flag_highwater = (Size > 0) ? (Data[0] % 2) : 0;
  sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag_highwater);
  (void)highwater;

  // 5. sqlite3_memory_used
  sqlite3_int64 used = sqlite3_memory_used();
  (void)used;

  // 6. sqlite3_status
  int current_val = 0;
  int highwater_val = 0;
  int reset_flag_status = (Size > 1) ? (Data[1] % 2) : 0;
  // Use SQLITE_STATUS_MEMORY_USED as a safe option
  int status_rc = sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current_val, &highwater_val, reset_flag_status);
  (void)status_rc;

  // 7. sqlite3_memory_used (again)
  used = sqlite3_memory_used();
  (void)used;

  // 8. sqlite3_hard_heap_limit64
  // Extract a value from input to set the limit, or use -1 to query
  sqlite3_int64 hard_limit = -1;
  if (Size >= 8) {
    hard_limit = *(sqlite3_int64 *)(Data + 2);
  } else if (Size > 0) {
    // Fallback if data is short
    hard_limit = (sqlite3_int64)(Data[0]);
  }
  sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(hard_limit);
  (void)prev_hard_limit;

  // 9. sqlite3_soft_heap_limit64
  // Extract another value for soft limit
  sqlite3_int64 soft_limit = -1;
  if (Size >= 16) {
    soft_limit = *(sqlite3_int64 *)(Data + 10);
  } else if (Size > 2) {
    soft_limit = (sqlite3_int64)(Data[2]);
  }
  sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(soft_limit);
  (void)prev_soft_limit;

  return 0;
}