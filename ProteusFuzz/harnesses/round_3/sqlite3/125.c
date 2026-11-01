// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
  if (Size < 1 || Size > 4096) {
    return 0;
  }

  // Create a safe copy of the input to ensure null-termination
  char *safe_input = (char *)malloc(Size + 1);
  if (!safe_input) {
    return 0;
  }
  memcpy(safe_input, Data, Size);
  safe_input[Size] = '\0';

  sqlite3 *db = NULL;
  sqlite3_filename filename = NULL;
  int rc;

  // Attempt to open a database in memory to get a valid filename
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    free(safe_input);
    return 0;
  }

  // Execute a simple command to ensure the DB is initialized
  sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY);", NULL, NULL, NULL);

  // Get the filename from the open database
  filename = (sqlite3_filename)sqlite3_db_filename(db, "main");

  if (filename) {
    // Test sqlite3_uri_parameter
    const char *param_val = sqlite3_uri_parameter(filename, "mode");
    (void)param_val; // Use the result to avoid unused variable warnings

    // Test sqlite3_uri_int64
    sqlite3_int64 int_val = sqlite3_uri_int64(filename, "cache_size", 1000);
    (void)int_val;

    // Test sqlite3_filename_database
    const char *db_name = sqlite3_filename_database(filename);
    (void)db_name;

    // Test sqlite3_filename_journal
    const char *journal_name = sqlite3_filename_journal(filename);
    (void)journal_name;

    // Test sqlite3_filename_wal
    const char *wal_name = sqlite3_filename_wal(filename);
    (void)wal_name;

    // Note: sqlite3_free_filename should only be called on filenames 
    // allocated by sqlite3_create_filename or similar, not directly 
    // from sqlite3_db_filename which points to internal memory.
    // Therefore, we do not call sqlite3_free_filename here to avoid undefined behavior.
  }

  // Extract a parameter name from the input to test sqlite3_uri_parameter with dynamic names
  char param_name[64];
  size_t param_len = Size < 63 ? Size : 63;
  memcpy(param_name, safe_input, param_len);
  param_name[param_len] = '\0';
  
  // Sanitize param_name to ensure it's a valid identifier or string
  // (The API takes a const char*, so any string is technically valid as a key)
  if (filename) {
    const char *dyn_param = sqlite3_uri_parameter(filename, param_name);
    (void)dyn_param;
    
    sqlite3_int64 dyn_int = sqlite3_uri_int64(filename, param_name, -1);
    (void)dyn_int;
  }

  sqlite3_close(db);
  
  free(safe_input);
  return 0;
}