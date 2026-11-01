// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_test_control at sqlite3.c:189824:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
  int rc;
  int op;
  int n;
  void *p;
  int sleep_ms;
  int buf[256];

  if (Size == 0) {
    return 0;
  }

  // Ensure initialization is done before testing
  if (!g_initialized) {
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
      // If initialization fails, we cannot proceed safely
      return 0;
    }
    g_initialized = 1;
  }

  // Use input data to drive fuzzing logic
  if (Size >= 1) {
    op = Data[0];
  } else {
    op = 0;
  }

  if (Size >= 2) {
    n = (int)Data[1];
  } else {
    n = 0;
  }

  if (Size >= 3) {
    sleep_ms = (int)Data[2];
  } else {
    sleep_ms = 0;
  }

  // Fuzz sqlite3_randomness
  // If n > 0, allocate buffer; otherwise pass NULL
  if (n > 0) {
    // Limit buffer size to avoid OOM
    if (n > 1024) {
      n = 1024;
    }
    p = malloc(n);
    if (p) {
      sqlite3_randomness(n, p);
      free(p);
    }
  } else {
    // Seed the PRNG by passing NULL or non-positive n
    sqlite3_randomness(n, NULL);
  }

  // Fuzz sqlite3_test_control
  // We pass a few dummy arguments to explore different code paths
  // The op code determines the expected arguments, but we try common ones
  rc = sqlite3_test_control(op, 0, 0, 0);
  (void)rc;

  // Fuzz sqlite3_sleep
  // Clamp sleep_ms to a safe range to avoid hanging the fuzzer
  if (sleep_ms < 0) {
    sleep_ms = 0;
  }
  if (sleep_ms > 100) {
    sleep_ms = 100;
  }
  rc = sqlite3_sleep(sleep_ms);
  (void)rc;

  // Fuzz sqlite3_global_recover
  rc = sqlite3_global_recover();
  (void)rc;

  // Occasionally test shutdown and re-initialize to explore state transitions
  // Use a byte from the input to decide
  if (Size >= 4 && Data[3] % 10 == 0) {
    sqlite3_shutdown();
    g_initialized = 0;
    rc = sqlite3_initialize();
    if (rc == SQLITE_OK) {
      g_initialized = 1;
    } else {
      // If re-initialization fails, we stop further operations
      return 0;
    }
  }

  return 0;
}