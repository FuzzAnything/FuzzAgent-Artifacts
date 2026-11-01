// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int g_initialized = 0;

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // 1. Prepare environment
  // Ensure SQLite is initialized before testing other functions if not already done.
  // We track state to allow testing the initialization path and the shutdown path.
  int rc = sqlite3_initialize();
  if (rc == SQLITE_OK) {
    g_initialized = 1;
  }

  // 2. Execute target functions based on input data to explore states

  // Fuzz sqlite3_global_recover (always returns SQLITE_OK, no args)
  // We call it regardless of input to ensure coverage of the stub.
  (void)sqlite3_global_recover();

  // Fuzz sqlite3_sleep
  // Use input bytes to determine sleep duration.
  // Clamp to reasonable values to avoid hanging the fuzzer or sleeping too long.
  int sleep_ms = 0;
  if (Size >= 1) {
    sleep_ms = (int)Data[0];
  }
  if (Size >= 2) {
    sleep_ms = (sleep_ms << 8) | (int)Data[1];
  }
  // Limit sleep to 100ms to keep fuzzing fast, but allow 0 and negative for edge cases
  if (sleep_ms > 100) sleep_ms = 100;
  if (sleep_ms < -100) sleep_ms = -100;
  
  (void)sqlite3_sleep(sleep_ms);

  // Fuzz sqlite3_randomness
  // Use input to determine buffer size and seed if needed.
  int rand_n = 0;
  if (Size >= 3) {
    rand_n = (int)Data[2];
  }
  if (Size >= 4) {
    rand_n = (rand_n << 8) | (int)Data[3];
  }
  
  // Limit allocation size to prevent OOM
  if (rand_n > 1024) rand_n = 1024;
  if (rand_n < 0) rand_n = 0;

  void *rand_buf = NULL;
  if (rand_n > 0) {
    rand_buf = malloc((size_t)rand_n);
    if (rand_buf) {
      sqlite3_randomness(rand_n, rand_buf);
      free(rand_buf);
    }
  } else {
    // Test seeding path (N <= 0 or P == NULL)
    sqlite3_randomness(rand_n, NULL);
  }

  // Fuzz sqlite3_test_control
  // This is an internal API, but we can fuzz the operation code and arguments.
  // We use a subset of known operations or random ones to trigger internal state checks.
  // Note: Behavior is unstable, but fuzzing helps find crashes in test infrastructure.
  if (Size >= 5) {
    int op = (int)Data[4];
    // We pass a few arguments based on input to vary the va_list behavior
    int arg1 = (Size >= 6) ? (int)Data[5] : 0;
    int arg2 = (Size >= 7) ? (int)Data[6] : 0;
    
    // We only pass arguments if the op code suggests it (heuristic) or just try blindly
    // Many test_control ops take specific args. We try a few common patterns.
    // SQLITE_TESTCTRL_FIRST is 0, SQLITE_TESTCTRL_LAST is 20 (approx).
    // We just pass random ints to see if it crashes on bad args.
    (void)sqlite3_test_control(op, arg1, arg2, 0);
  }

  // 3. Cleanup and explore shutdown state
  // Randomly decide to shutdown to test the lifecycle and subsequent calls
  // We use the last byte of input to make this decision
  int should_shutdown = (Size > 0) && (Data[Size-1] % 2 == 0);
  
  if (should_shutdown && g_initialized) {
    rc = sqlite3_shutdown();
    if (rc == SQLITE_OK) {
      g_initialized = 0;
    }
    
    // After shutdown, try calling functions again to test idempotency or error handling
    // sqlite3_initialize should be callable again
    (void)sqlite3_initialize();
    g_initialized = 1;
    
    // sqlite3_shutdown called twice (idempotent)
    (void)sqlite3_shutdown();
    g_initialized = 0;
  }

  return 0;
}