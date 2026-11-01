// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
  // 1. Prepare environment: Ensure clean state by shutting down first
  sqlite3_shutdown();

  // 2. Invoke sqlite3_config (first call)
  // Use SQLITE_CONFIG_SINGLETHREAD (1) or SQLITE_CONFIG_MULTITHREAD (2)
  int config_op1 = 1;
  if (Size > 0) {
    config_op1 = (Data[0] % 2) + 1;
  }
  sqlite3_config(config_op1);

  // 3. Invoke sqlite3_hard_heap_limit64 (first call)
  sqlite3_int64 limit1 = 0;
  if (Size > 1) {
    limit1 = (sqlite3_int64)Data[1];
    if (limit1 < 0) limit1 = 0;
  }
  sqlite3_hard_heap_limit64(limit1);

  // 4. Invoke sqlite3_hard_heap_limit64 (second call)
  sqlite3_int64 limit2 = 0;
  if (Size > 2) {
    limit2 = (sqlite3_int64)Data[2];
    if (limit2 < 0) limit2 = 0;
  }
  sqlite3_hard_heap_limit64(limit2);

  // 5. Invoke sqlite3_config (second call)
  int config_op2 = 1;
  if (Size > 3) {
    config_op2 = (Data[3] % 2) + 1;
  }
  sqlite3_config(config_op2);

  // 6. Invoke sqlite3_snprintf
  // Use a static buffer to avoid stack allocation issues with internal sqlite3 memory management
  // or ensure we don't pass stack memory that sqlite3 might try to manage as heap.
  // However, sqlite3_snprintf writes to the buffer provided. The error suggests 
  // sqlite3 is trying to free a pointer that looks like stack memory.
  // This often happens if the format string or arguments are misinterpreted.
  // We will use a safe, static buffer and a simple format string to avoid complex parsing.
  
  static char buffer[256];
  int buf_size = 256;
  
  // Prepare a safe format string. We avoid passing raw input as the format string
  // to prevent sqlite3 from interpreting stack data as pointers to free.
  const char *fmt = "safe_string";
  
  // If we have enough data, we can try to use part of it as arguments, but 
  // for safety against the specific ASAN error (freeing non-malloc), 
  // we will stick to a simple format with no variable arguments that could 
  // be misinterpreted as pointers.
  
  // If the input suggests a different format, we only use it if it's strictly safe.
  // Given the error, passing raw input as the format string is risky if it contains 
  // pointer-like values that sqlite3 might try to process.
  // We will strictly use a constant format string to ensure stability.
  
  sqlite3_snprintf(buf_size, buffer, fmt);

  return 0;
}