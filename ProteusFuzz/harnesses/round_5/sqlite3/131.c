// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:63856:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  // We need to construct a valid filename string from the input.
  // Ensure it's null-terminated.
  char *filename_str = (char *)malloc(Size + 1);
  if (!filename_str) return 0;
  memcpy(filename_str, Data, Size);
  filename_str[Size] = '\0';

  // sqlite3_create_filename requires 5 arguments:
  // zDatabase, zJournal, zWal, nParam, azParam
  // We will pass the input string as the database name.
  // Journal and WAL are set to NULL.
  // Parameters are empty.
  const char *zDatabase = filename_str;
  const char *zJournal = NULL;
  const char *zWal = NULL;
  int nParam = 0;
  const char **azParam = NULL;

  sqlite3_filename zFilename = sqlite3_create_filename(
      zDatabase, zJournal, zWal, nParam, azParam
  );

  free(filename_str);

  if (!zFilename) return 0;

  // Test sqlite3_filename_database
  const char *db_name = sqlite3_filename_database(zFilename);
  (void)db_name;

  // Test sqlite3_filename_journal
  const char *journal_name = sqlite3_filename_journal(zFilename);
  (void)journal_name;

  // Test sqlite3_filename_wal
  const char *wal_name = sqlite3_filename_wal(zFilename);
  (void)wal_name;

  // Test sqlite3_uri_parameter with various parameter names derived from input
  if (Size > 0) {
    // Create a safe parameter name from the input data
    char param_name[32];
    size_t param_len = (Size < 31) ? Size : 31;
    memcpy(param_name, Data, param_len);
    param_name[param_len] = '\0';

    const char *param_value = sqlite3_uri_parameter(zFilename, param_name);
    (void)param_value;

    // Test sqlite3_uri_boolean
    int bool_result = sqlite3_uri_boolean(zFilename, param_name, 0);
    (void)bool_result;

    // Test sqlite3_uri_int64
    int64_t int_result = sqlite3_uri_int64(zFilename, param_name, -1);
    (void)int_result;

    // Test sqlite3_uri_key
    for (int i = 0; i < 5; i++) {
      const char *key = sqlite3_uri_key(zFilename, i);
      (void)key;
    }
  }

  // Test sqlite3_database_file_object
  // Note: This function expects a filename pointer that was passed to xOpen
  // with specific flags. Passing a generic created filename here is technically
  // undefined behavior per the docs, but useful for fuzzing to check robustness.
  // We cast zFilename to const char* as required by the API.
  sqlite3_file *file_obj = sqlite3_database_file_object((const char *)zFilename);
  (void)file_obj;

  // Cleanup
  sqlite3_free_filename(zFilename);

  return 0;
}