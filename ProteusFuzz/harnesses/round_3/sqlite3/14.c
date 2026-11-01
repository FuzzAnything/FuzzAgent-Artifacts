// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_libversion at sqlite3.c:185731:24 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 API functions.
 * Target sequence:
 * sqlite3_libversion -> sqlite3_sourceid -> sqlite3_config (x7) -> sqlite3_initialize -> sqlite3_vfs_find
 */

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
  if (Data == NULL || Size == 0) {
    return 0;
  }

  /* Step 1: Call sqlite3_libversion */
  const char *libver = sqlite3_libversion();
  (void)libver;

  /* Step 2: Call sqlite3_sourceid */
  const char *sourceid = sqlite3_sourceid();
  (void)sourceid;

  /* Step 3-9: Call sqlite3_config multiple times with diverse options */
  int config_opts[7];
  int config_args[7];

  for (int i = 0; i < 7; i++) {
    /* Derive option and argument from input data */
    if (Size > (size_t)(i + 1)) {
      config_opts[i] = (int)Data[i];
      if (Size > (size_t)(i + 1 + 1)) {
        config_args[i] = (int)Data[i + 1];
      } else {
        config_args[i] = 0;
      }
    } else {
      config_opts[i] = 0;
      config_args[i] = 0;
    }

    /* Invoke sqlite3_config with derived options */
    int ret = 0;
    
    /* Handle specific known options to ensure correct argument passing */
    switch (config_opts[i]) {
      case SQLITE_CONFIG_SINGLETHREAD:
      case SQLITE_CONFIG_MULTITHREAD:
      case SQLITE_CONFIG_SERIALIZED:
        ret = sqlite3_config(config_opts[i]);
        break;
      case SQLITE_CONFIG_MEMSTATUS:
        ret = sqlite3_config(config_opts[i], config_args[i]);
        break;
      case SQLITE_CONFIG_MALLOC:
      case SQLITE_CONFIG_GETMALLOC:
        ret = sqlite3_config(config_opts[i], (void*)config_args[i]);
        break;
      case SQLITE_CONFIG_PAGECACHE:
      case SQLITE_CONFIG_SCRATCH:
      case SQLITE_CONFIG_PMASZ:
      case SQLITE_CONFIG_WIN32_HEAPSIZE:
        ret = sqlite3_config(config_opts[i], config_args[i], 0, 0);
        break;
      case SQLITE_CONFIG_URI:
      case SQLITE_CONFIG_COVERING_INDEX_SCAN:
        ret = sqlite3_config(config_opts[i], config_args[i]);
        break;
      case SQLITE_CONFIG_PCACHE:
      case SQLITE_CONFIG_GETPCACHE:
        ret = sqlite3_config(config_opts[i], (void*)config_args[i]);
        break;
      case SQLITE_CONFIG_LOG:
        /* Skip LOG config in fuzzing to avoid passing invalid function pointers */
        ret = sqlite3_config(config_opts[i], NULL, NULL);
        break;
      case SQLITE_CONFIG_PCACHE_HDRSZ: {
        /* FIX: Allocate a valid integer to receive the header size instead of passing NULL */
        int header_size = 0;
        ret = sqlite3_config(config_opts[i], &header_size);
        (void)header_size;
        break;
      }
      default:
        /* Try with a single integer argument for unknown options */
        ret = sqlite3_config(config_opts[i], config_args[i]);
        break;
    }
    (void)ret;
  }

  /* Step 10: Call sqlite3_initialize */
  int init_ret = sqlite3_initialize();
  if (init_ret != SQLITE_OK) {
    /* Initialization failed, but we continue to test other functions */
  }

  /* Step 11: Call sqlite3_vfs_find */
  /* Derive VFS name from input data */
  const char *vfs_name = NULL;
  if (Size > 0) {
    /* Use the last byte of input as a flag to decide if we pass NULL or a name */
    if (Data[Size - 1] == 0) {
      vfs_name = NULL;
    } else {
      /* Create a simple VFS name from input data */
      static char vfs_buf[256];
      size_t vfs_len = (Size > 255) ? 255 : Size - 1;
      memcpy(vfs_buf, Data, vfs_len);
      vfs_buf[vfs_len] = '\0';
      vfs_name = vfs_buf;
    }
  }

  sqlite3_vfs *vfs = sqlite3_vfs_find(vfs_name);
  (void)vfs;

  /* Cleanup: No explicit cleanup needed for these functions */
  /* sqlite3_shutdown() is not called to avoid interfering with subsequent test cases */

  return 0;
}