// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_thread_cleanup at sqlite3.c:189619:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_global_recover at sqlite3.c:189543:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
  (void)Data;
  (void)Size;

  /*
   * Note: The target functions (sqlite3_initialize, sqlite3_shutdown, etc.)
   * do not accept arbitrary data input. They manage global library state.
   * Therefore, this fuzzer explores the state transitions of these functions
   * by invoking them in various sequences and checking return codes.
   * The 'Data' and 'Size' arguments are unused but required by the fuzzer interface.
   */

  int rc = SQLITE_OK;

  /* 1. Test sqlite3_threadsafe (always safe to call) */
  int threadsafe = sqlite3_threadsafe();
  (void)threadsafe;

  /* 2. Test sqlite3_global_recover (stub, always returns SQLITE_OK) */
  rc = sqlite3_global_recover();
  if (rc != SQLITE_OK) {
    /* Should not happen based on documentation */
    return 0;
  }

  /* 3. Test sqlite3_thread_cleanup (no-op stub) */
  sqlite3_thread_cleanup();

  /* 4. Explore initialization and shutdown cycles */
  
  /* Attempt to initialize */
  rc = sqlite3_initialize();
  if (rc == SQLITE_OK) {
    g_initialized = 1;
    
    /* Call initialize again (idempotent check) */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
      /* Should be SQLITE_OK */
      return 0;
    }

    /* Attempt to shutdown */
    rc = sqlite3_shutdown();
    if (rc == SQLITE_OK) {
      g_initialized = 0;
      
      /* Call shutdown again (idempotent check) */
      rc = sqlite3_shutdown();
      if (rc != SQLITE_OK) {
        /* Should be SQLITE_OK */
        return 0;
      }
    } else {
      /* If shutdown failed, try to recover state by re-initializing if needed */
      g_initialized = 0;
    }
  } else {
    /* Initialization failed, try to recover */
    rc = sqlite3_global_recover();
    (void)rc;
  }

  /* 5. Direct call to sqlite3_os_end (Warning: usually internal, but tested here) */
  /* Note: Calling this directly after shutdown might be redundant or unsafe in some builds,
     but the task requires testing it. We assume a clean state or that it handles no-ops. */
  rc = sqlite3_os_end();
  (void)rc;

  return 0;
}