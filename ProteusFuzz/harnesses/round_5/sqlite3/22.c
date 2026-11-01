// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // 1. sqlite3_shutdown
  // Ensure clean state before configuration
  sqlite3_shutdown();

  // 2. sqlite3_config
  // Configure SQLite globally. 
  int config_op = 0;
  if (Size > 0) {
    config_op = Data[0] % 10; 
  }
  sqlite3_config(config_op);

  // 3. sqlite3_hard_heap_limit64 (First call)
  sqlite3_int64 limit1 = 0;
  if (Size >= 8) {
    memcpy(&limit1, Data, 8);
  } else if (Size > 0) {
    limit1 = Data[0];
  }
  if (limit1 < -1) limit1 = -1; 
  sqlite3_int64 prev_limit1 = sqlite3_hard_heap_limit64(limit1);
  (void)prev_limit1;

  // 4. sqlite3_hard_heap_limit64 (Second call)
  sqlite3_int64 limit2 = 0;
  if (Size >= 16) {
    memcpy(&limit2, Data + 8, 8);
  } else if (Size > 8) {
    limit2 = Data[8];
  }
  if (limit2 < -1) limit2 = -1;
  sqlite3_int64 prev_limit2 = sqlite3_hard_heap_limit64(limit2);
  (void)prev_limit2;

  // 5. sqlite3_config (Second call)
  int config_op2 = 0;
  if (Size > 1) {
    config_op2 = Data[1] % 10;
  }
  sqlite3_config(config_op2);

  // 6. sqlite3_snprintf
  char buffer[256];
  int buf_size = 256;
  const char *format_str = "Test: %s %d %p";
  const char *input_str = "";
  int input_int = 0;
  void *input_ptr = NULL;

  if (Size > 2) {
    size_t str_len = Size - 2;
    if (str_len > 100) str_len = 100;
    char temp_str[101];
    memcpy(temp_str, Data + 2, str_len);
    temp_str[str_len] = '\0';
    input_str = temp_str;
  }

  if (Size > 3) {
    input_int = Data[3];
  }

  if (Size > 4) {
    input_ptr = (void*)(uintptr_t)Data[4];
  }

  char *result = sqlite3_snprintf(buf_size, buffer, format_str, input_str, input_int, input_ptr);
  (void)result;

  return 0;
}