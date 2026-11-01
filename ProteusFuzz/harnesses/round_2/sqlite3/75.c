// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  int rc;
  int logFrames = -1;
  int ckptFrames = -1;

  if (Size == 0) {
    return 0;
  }

  // Use the first byte to decide whether to open a real database or use NULL
  // This helps explore states with invalid handles as well as valid ones.
  int useRealDb = (Data[0] % 2) == 0;

  if (useRealDb) {
    // Create a dummy file for the database
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "w");
    if (f) {
      // Write some dummy data to ensure the file exists
      fwrite(Data, 1, Size, f);
      fclose(f);
    }

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
      // If opening fails, we still have a NULL db, which is a valid state to test
      db = NULL;
    }
  }

  // Target API 1: sqlite3_is_interrupted
  // Only call with valid db pointer. Calling with NULL causes SEGV in some builds.
  if (db) {
    (void)sqlite3_is_interrupted(db);
  }

  // Target API 2: sqlite3_extended_errcode
  // Only call with valid db pointer.
  if (db) {
    (void)sqlite3_extended_errcode(db);
  }

  // Target API 3: sqlite3_errcode
  // Only call with valid db pointer.
  if (db) {
    (void)sqlite3_errcode(db);
  }

  // Target API 4: sqlite3_extended_result_codes
  // Uses second byte for onoff flag (0 or 1)
  int onoff = (Size > 1) ? (Data[1] % 2) : 0;
  if (db) {
    (void)sqlite3_extended_result_codes(db, onoff);
  }

  // Target API 5: sqlite3_wal_checkpoint_v2
  // Uses third byte for mode selection
  int mode = SQLITE_CHECKPOINT_PASSIVE;
  if (Size > 2) {
    int modeIdx = Data[2] % 5;
    switch (modeIdx) {
      case 0: mode = SQLITE_CHECKPOINT_PASSIVE; break;
      case 1: mode = SQLITE_CHECKPOINT_FULL; break;
      case 2: mode = SQLITE_CHECKPOINT_RESTART; break;
      case 3: mode = SQLITE_CHECKPOINT_TRUNCATE; break;
      case 4: mode = SQLITE_CHECKPOINT_NOOP; break;
    }
  }

  // zDb can be NULL, "main", or a random string from input
  const char *zDb = NULL;
  char *dynamicDbName = NULL;

  if (Size > 3 && (Data[3] % 3) == 0) {
    zDb = "main";
  } else if (Size > 4 && (Data[3] % 3) == 1) {
    // Use a substring from the input as the database name
    // Ensure it's null-terminated for safety
    size_t len = Size;
    if (len > 100) len = 100; // Limit length to avoid huge allocations
    dynamicDbName = malloc(len + 1);
    if (dynamicDbName) {
      memcpy(dynamicDbName, Data, len);
      dynamicDbName[len] = '\0'; // Ensure termination
      zDb = dynamicDbName;
    }
  }

  if (db && zDb != NULL) {
    (void)sqlite3_wal_checkpoint_v2(db, zDb, mode, &logFrames, &ckptFrames);
  } else if (db) {
    (void)sqlite3_wal_checkpoint_v2(db, NULL, mode, &logFrames, &ckptFrames);
  }

  if (dynamicDbName) {
    free(dynamicDbName);
  }

  // Target API 6: sqlite3_close_v2
  if (db) {
    (void)sqlite3_close_v2(db);
    db = NULL;
  }

  // Cleanup dummy file if it was created
  if (useRealDb) {
    remove("./dummy_file");
  }

  return 0;
}