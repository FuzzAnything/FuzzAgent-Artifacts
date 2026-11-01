// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_threadsafe at sqlite3.c:185750:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_os_end at sqlite3.c:47927:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static int g_initialized = 0;

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
  (void)Data;
  (void)Size;

  // Fuzz sqlite3_threadsafe (always safe to call)
  int threadsafe = sqlite3_threadsafe();
  (void)threadsafe;

  // Fuzz sqlite3_os_init (idempotent, safe to call multiple times)
  int rc_os_init = sqlite3_os_init();
  if (rc_os_init != SQLITE_OK) {
    // If os_init fails, we might not be able to proceed safely, but we continue to test other paths
  }

  // Fuzz sqlite3_config with various options
  // SQLITE_CONFIG_SINGLETHREAD
  int rc_config1 = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
  // SQLITE_CONFIG_MULTITHREAD
  int rc_config2 = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
  // SQLITE_CONFIG_SERIALIZED
  int rc_config3 = sqlite3_config(SQLITE_CONFIG_SERIALIZED);
  
  // SQLITE_CONFIG_PCACHE_HDRSZ (requires a valid int pointer)
  int header_size = 0;
  int rc_config4 = sqlite3_config(SQLITE_CONFIG_PCACHE_HDRSZ, &header_size);
  (void)rc_config1;
  (void)rc_config2;
  (void)rc_config3;
  (void)rc_config4;
  (void)header_size;

  // Fuzz sqlite3_initialize (idempotent)
  int rc_init = sqlite3_initialize();
  if (rc_init == SQLITE_OK) {
    g_initialized = 1;
  }

  // If initialized, we can try to call config again (should return SQLITE_MISUSE for non-anytime options)
  // This tests the state transition
  if (g_initialized) {
    int rc_config5 = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    (void)rc_config5;
  }

  // Fuzz sqlite3_shutdown (idempotent)
  int rc_shutdown = sqlite3_shutdown();
  if (rc_shutdown == SQLITE_OK) {
    g_initialized = 0;
  }

  // Fuzz sqlite3_os_end (should be called after shutdown, but is idempotent)
  int rc_os_end = sqlite3_os_end();
  (void)rc_os_end;

  // Re-initialize to leave the library in a consistent state for the next fuzz iteration
  // This ensures the next call to LLVMFuzzerTestOneInput starts with a clean slate if needed
  // although sqlite3_initialize is idempotent.
  int rc_reinit = sqlite3_initialize();
  if (rc_reinit == SQLITE_OK) {
    g_initialized = 1;
  }

  // Ensure we don't leak resources if the test logic was complex, 
  // but for these global functions, we just rely on their idempotency.
  // We do not call shutdown here to avoid leaving the library in a shut-down state 
  // which might break subsequent calls if the fuzzer expects a running library.
  // However, since sqlite3_initialize is called at the start of most ops, 
  // leaving it initialized is safer.

  return 0;
}