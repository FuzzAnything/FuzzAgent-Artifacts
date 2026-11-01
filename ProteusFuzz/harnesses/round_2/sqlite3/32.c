// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:190798:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // 1. sqlite3_sourceid
  // Always returns a static non-null string, no cleanup needed.
  const char *source_id = sqlite3_sourceid();
  (void)source_id; // Suppress unused variable warning

  // 2. sqlite3_compileoption_get
  // Use input data to generate an index N.
  int index = 0;
  if (Size >= 4) {
    index = *(const int32_t *)(Data);
  } else if (Size > 0) {
    index = Data[0];
  }
  
  // Fuzz with positive and negative indices to explore different states
  const char *opt1 = sqlite3_compileoption_get(index);
  const char *opt2 = sqlite3_compileoption_get(-index);
  const char *opt3 = sqlite3_compileoption_get(0);
  
  (void)opt1;
  (void)opt2;
  (void)opt3;

  // 3. sqlite3_sleep
  // Use input data to generate sleep duration.
  int sleep_ms = 0;
  if (Size >= 4) {
    sleep_ms = *(const int32_t *)(Data + (Size >= 4 ? 4 : 0));
  } else if (Size > 0) {
    sleep_ms = Data[0];
  }
  
  // Fuzz with various durations including negative values (treated as 0 in newer versions)
  int ret1 = sqlite3_sleep(sleep_ms);
  int ret2 = sqlite3_sleep(-sleep_ms);
  int ret3 = sqlite3_sleep(0);
  
  (void)ret1;
  (void)ret2;
  (void)ret3;

  // 4. sqlite3_free
  // We must only free memory allocated by sqlite3_malloc/realloc.
  // Since we haven't allocated memory in this specific sequence, 
  // we can safely pass NULL or allocate a small block to test the free path.
  
  // Test freeing NULL (safe)
  sqlite3_free(NULL);
  
  // Test freeing a valid allocation
  size_t alloc_size = (Size > 0) ? Size : 1;
  void *ptr = sqlite3_malloc(alloc_size);
  if (ptr != NULL) {
    // Write some data to ensure the memory is valid
    memset(ptr, 0xFF, alloc_size);
    sqlite3_free(ptr);
  }

  return 0;
}