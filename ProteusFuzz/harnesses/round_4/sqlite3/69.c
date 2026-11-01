// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:33215:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
  sqlite3_str *str = NULL;
  const char *zIn;
  int N;
  int err;
  int len;
  char *val;
  int Nchar;
  char C;
  int Ntruncate;

  if (Size < 1) {
    return 0;
  }

  // Initialize sqlite3_str with NULL database (no lookaside)
  str = sqlite3_str_new(NULL);
  if (str == NULL) {
    return 0;
  }

  // Use fuzzed data to drive various operations
  zIn = (const char *)Data;

  // 1. Test sqlite3_str_append
  if (Size >= 2) {
    N = (int)Data[0];
    // Limit N to prevent excessive memory allocation in fuzzing
    if (N > (int)Size - 1) N = (int)Size - 1;
    if (N < 0) N = 0;
    sqlite3_str_append(str, zIn + 1, N);
  }

  // 2. Test sqlite3_str_appendchar
  if (Size >= 3) {
    Nchar = (int)Data[1];
    C = (char)Data[2];
    // Limit Nchar
    if (Nchar > 1000) Nchar = 1000;
    if (Nchar < 0) Nchar = 0;
    sqlite3_str_appendchar(str, Nchar, C);
  }

  // 3. Test sqlite3_str_truncate
  if (Size >= 4) {
    Ntruncate = (int)Data[3];
    // Allow negative or large values to test edge cases
    sqlite3_str_truncate(str, Ntruncate);
  }

  // 4. Check error code
  err = sqlite3_str_errcode(str);
  (void)err; // Use err to suppress unused warning, but logic continues

  // 5. Get length
  len = sqlite3_str_length(str);
  (void)len;

  // 6. Get value
  val = sqlite3_str_value(str);
  if (val != NULL) {
    // Just to ensure the value is accessed safely
    (void)strlen(val);
  }

  // 7. Perform additional operations to explore more states
  if (Size >= 5) {
    N = (int)Data[4];
    if (N > (int)Size - 5) N = (int)Size - 5;
    if (N < 0) N = 0;
    sqlite3_str_append(str, zIn + 5, N);
    
    // Truncate again
    Ntruncate = (int)Data[5 % Size];
    sqlite3_str_truncate(str, Ntruncate);
  }

  // 8. Finalize and cleanup
  // sqlite3_str_finish returns the string and frees the object
  // We don't need to free the returned string here as it's managed by the fuzzer
  // But we must call finish to avoid memory leaks in the test
  (void)sqlite3_str_finish(str);
  str = NULL;

  return 0;
}