// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_libversion at sqlite3.c:185731:24 in sqlite3.h
// sqlite3_sourceid at sqlite3.c:266758:24 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  /* Step 1: sqlite3_libversion */
  const char *ver = sqlite3_libversion();
  if (ver == NULL) {
    return 0;
  }

  /* Step 2: sqlite3_sourceid */
  const char *srcid = sqlite3_sourceid();
  if (srcid == NULL) {
    return 0;
  }

  /* Step 3-9: sqlite3_config calls with diverse options */
  /* We use a subset of valid config options that do not require extra arguments 
     or use safe default arguments to avoid crashes. 
     SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, SQLITE_CONFIG_SERIALIZED
     take no extra args. SQLITE_CONFIG_MEMSTATUS takes an int.
     We avoid options like SQLITE_CONFIG_MALLOC which require function pointers 
     or complex structures that could cause segfaults if not properly set up.
  */
  int cfg_opts[] = {
    SQLITE_CONFIG_SINGLETHREAD,
    SQLITE_CONFIG_MULTITHREAD,
    SQLITE_CONFIG_SERIALIZED,
    SQLITE_CONFIG_MEMSTATUS,
    SQLITE_CONFIG_SINGLETHREAD,
    SQLITE_CONFIG_MULTITHREAD,
    SQLITE_CONFIG_SERIALIZED
  };

  /* Select options based on input data, ensuring we stay within bounds */
  int idx1 = (Size > 0) ? Data[0] % 7 : 0;
  int idx2 = (Size > 1) ? Data[1] % 7 : 0;
  int idx3 = (Size > 2) ? Data[2] % 7 : 0;
  int idx4 = (Size > 3) ? Data[3] % 7 : 0;
  int idx5 = (Size > 4) ? Data[4] % 7 : 0;
  int idx6 = (Size > 5) ? Data[5] % 7 : 0;
  int idx7 = (Size > 6) ? Data[6] % 7 : 0;

  sqlite3_config(cfg_opts[idx1]);
  sqlite3_config(cfg_opts[idx2]);
  sqlite3_config(cfg_opts[idx3]);
  sqlite3_config(cfg_opts[idx4]);
  sqlite3_config(cfg_opts[idx5]);
  sqlite3_config(cfg_opts[idx6]);
  sqlite3_config(cfg_opts[idx7]);

  /* Step 10: sqlite3_initialize */
  int rc = sqlite3_initialize();
  if (rc != SQLITE_OK) {
    /* Initialization failed, but we continue to test other functions */
  }

  /* Step 11: sqlite3_vfs_find */
  /* Allocate memory for the VFS name to avoid stack-use-after-scope */
  char *vfs_name = NULL;
  if (Size > 0) {
    vfs_name = (char *)malloc(Size + 1);
    if (vfs_name != NULL) {
      memcpy(vfs_name, Data, Size);
      vfs_name[Size] = '\0';
      
      /* Ensure the string is valid UTF-8 and not empty for better testing, 
         though sqlite3_vfs_find handles empty/invalid gracefully by returning NULL */
      sqlite3_vfs *vfs = sqlite3_vfs_find(vfs_name);
      /* vfs can be NULL if name not found, which is expected */
      (void)vfs; /* Suppress unused variable warning */
      
      free(vfs_name);
    }
  }

  return 0;
}