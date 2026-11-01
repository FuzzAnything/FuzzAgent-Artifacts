// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int g_initialized = 0;

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
  // Avoid running expensive or blocking operations if input is empty
  if (Size == 0) {
    return 0;
  }

  // Use input data to determine which API to fuzz
  uint8_t op = Data[0] % 6;

  switch (op) {
    case 0: {
      // Fuzz sqlite3_initialize
      // It is idempotent, so calling it multiple times is safe
      int rc = sqlite3_initialize();
      (void)rc; // Ignore return value for fuzzing stability
      g_initialized = 1;
      break;
    }
    case 1: {
      // Fuzz sqlite3_shutdown
      // Only call if we think we are initialized to avoid undefined behavior in some builds
      // Note: sqlite3_shutdown is idempotent and safe to call multiple times
      if (g_initialized) {
        int rc = sqlite3_shutdown();
        (void)rc;
        g_initialized = 0;
      }
      break;
    }
    case 2: {
      // Fuzz sqlite3_os_init
      // This is internal but callable. Idempotent.
      int rc = sqlite3_os_init();
      (void)rc;
      break;
    }
    case 3: {
      // Fuzz sqlite3_os_end
      // This is internal. Always returns SQLITE_OK.
      int rc = sqlite3_os_end();
      (void)rc;
      break;
    }
    case 4: {
      // Fuzz sqlite3_sleep
      // Use input to determine sleep duration, but cap it to avoid long hangs
      // Ensure we don't sleep for too long (max 100ms)
      int ms = 0;
      if (Size >= 2) {
        ms = (Data[1] % 101); // 0 to 100 ms
      } else if (Size >= 1) {
        ms = (Data[0] % 101);
      }
      // Also test negative values as per documentation (treated as 0 in 3.42+)
      if (Size > 1 && (Data[1] & 0x01)) {
        ms = -ms;
      }
      int actual = sqlite3_sleep(ms);
      (void)actual;
      break;
    }
    case 5: {
      // Fuzz sqlite3_randomness
      // Determine N from input, cap at reasonable size (e.g., 1024)
      int n = 0;
      if (Size >= 2) {
        n = (Data[1] % 1025);
      } else if (Size >= 1) {
        n = (Data[0] % 1025);
      }

      if (n > 0) {
        uint8_t *buf = (uint8_t *)malloc(n);
        if (buf) {
          sqlite3_randomness(n, buf);
          // Optionally use the buffer to trigger more state changes if needed,
          // but for this API, just generating randomness is the test.
          free(buf);
        }
      } else {
        // Test seeding path (N <= 0 or NULL)
        sqlite3_randomness(n, NULL);
      }
      break;
    }
  }

  return 0;
}