// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
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

/*
 * LLVMFuzzerTestOneInput is the entry point for the fuzzer.
 * It tests the initialization, configuration, and shutdown sequence of SQLite.
 */
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
  int rc;
  int header_size = 0;

  /* 
   * Step 1: Check threadsafety at the start.
   * This is a safe operation that doesn't require initialization.
   */
  (void)sqlite3_threadsafe();

  /*
   * Step 2: Attempt to configure SQLite before initialization.
   * We try various configuration options to explore different code paths.
   * Note: sqlite3_config is not threadsafe, but fuzzing is single-threaded.
   */
  if (Size > 0) {
    int op = Data[0] % 10; /* Limit to a small range of plausible opcodes for safety */
    
    /* 
     * Specific handling for SQLITE_CONFIG_PCACHE_HDRSZ as per instructions.
     * We pass a valid integer variable address, not NULL.
     */
    if (op == 1) {
      /* SQLITE_CONFIG_PCACHE_HDRSZ */
      rc = sqlite3_config(SQLITE_CONFIG_PCACHE_HDRSZ, &header_size);
    } else if (op == 2) {
      /* SQLITE_CONFIG_SINGLETHREAD */
      rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    } else if (op == 3) {
      /* SQLITE_CONFIG_MULTITHREAD */
      rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    } else if (op == 4) {
      /* SQLITE_CONFIG_SERIALIZED */
      rc = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    } else {
      /* Try other valid config options if available, or just return SQLITE_OK/MISUSE */
      /* We avoid passing invalid arguments to prevent crashes from bad pointers */
      rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }
  }

  /*
   * Step 3: Initialize the library.
   * This sets up global resources.
   */
  rc = sqlite3_initialize();
  if (rc != SQLITE_OK) {
    /* If initialization fails, we can't proceed with other operations safely.
     * However, we still want to test the shutdown path.
     */
    goto cleanup;
  }

  /*
   * Step 4: Test sqlite3_config after initialization.
   * Most options should return SQLITE_MISUSE here, which is expected behavior.
   */
  if (Size > 1) {
    int op2 = Data[1] % 10;
    if (op2 == 1) {
      rc = sqlite3_config(SQLITE_CONFIG_PCACHE_HDRSZ, &header_size);
    } else {
      rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    }
  }

  /*
   * Step 5: Test reset_auto_extension.
   * This clears any auto-extensions.
   */
  sqlite3_reset_auto_extension();

  /*
   * Step 6: Shutdown the library.
   * This releases resources.
   */
  rc = sqlite3_shutdown();

cleanup:
  /*
   * Step 7: Cleanup and edge case testing.
   * Call os_end directly? The documentation says "The application should never invoke 
   * either sqlite3_os_init() or sqlite3_os_end() directly." 
   * However, the task asks to fuzz sqlite3_os_end. 
   * We will call it here to test its behavior, though it's technically misuse.
   * Since it's a fuzz driver, triggering "misuse" paths is often desired to find bugs.
   * But to be safe and follow the "cleanup" instruction strictly for the library state:
   * We rely on sqlite3_shutdown to handle the os_end internally.
   * 
   * Wait, the task explicitly lists `sqlite3_os_end` as a target API.
   * To fuzz it, we must call it. We'll call it after shutdown to see if it handles
   * being called multiple times or in a shutdown state gracefully.
   */
  (void)sqlite3_os_end();

  /*
   * Step 8: Test idempotency of shutdown and initialize again.
   */
  (void)sqlite3_shutdown();
  (void)sqlite3_initialize();
  
  /*
   * Step 9: Final cleanup.
   */
  (void)sqlite3_shutdown();

  return 0;
}