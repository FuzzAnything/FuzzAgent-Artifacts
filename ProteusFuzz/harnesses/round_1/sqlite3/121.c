// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  int rc;

  if (Size < 2) {
    return 0;
  }

  // Create an in-memory database
  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    return 0;
  }

  // Use the input data to construct a SQL statement.
  // We ensure it's a valid C string by null-terminating if necessary.
  // We try to create a statement that likely has parameters.
  char *sql = (char *)malloc(Size + 2);
  if (!sql) {
    sqlite3_close(db);
    return 0;
  }
  memcpy(sql, Data, Size);
  // Ensure null termination
  sql[Size] = '\0';
  // If the input doesn't end with a semicolon, add one to help parsing
  if (Size > 0 && sql[Size - 1] != ';') {
    sql[Size] = ';';
    sql[Size + 1] = '\0';
  }

  // Prepare the statement
  rc = sqlite3_prepare_v2(db, sql, Size + 2, &stmt, NULL);
  free(sql);

  if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW) {
    // Preparation failed, try to close and return
    sqlite3_close(db);
    return 0;
  }

  if (stmt) {
    // 1. Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);

    // 2. Test sqlite3_bind_parameter_name for valid indices
    for (int i = 1; i <= param_count; i++) {
      const char *name = sqlite3_bind_parameter_name(stmt, i);
      // name is just a pointer, no cleanup needed for the string itself

      // 3. Test sqlite3_bind_parameter_index with the name if it exists
      if (name && strlen(name) > 0) {
        int idx = sqlite3_bind_parameter_index(stmt, name);
        (void)idx; // Use the index
      }
    }

    // 4. Test binding functions with fuzzed data
    // We use the input data to derive values for binding
    
    // Bind int64
    if (param_count >= 1) {
      sqlite3_int64 val64 = 0;
      if (Size >= 8) {
        memcpy(&val64, Data, 8);
      }
      rc = sqlite3_bind_int64(stmt, 1, val64);
      (void)rc;
    }

    // Bind int
    if (param_count >= 2) {
      int val32 = 0;
      if (Size >= 4) {
        memcpy(&val32, Data, 4);
      }
      rc = sqlite3_bind_int(stmt, 2, val32);
      (void)rc;
    }

    // Bind text
    if (param_count >= 3) {
      // Use a portion of the input as text, ensuring null termination
      size_t text_len = Size > 100 ? 100 : Size;
      // Find a null byte or use the whole chunk if none
      size_t actual_len = text_len;
      for (size_t k = 0; k < text_len; k++) {
        if (Data[k] == 0) {
          actual_len = k;
          break;
        }
      }
      
      // Create a safe buffer for text
      char *text_buf = (char *)malloc(actual_len + 1);
      if (text_buf) {
        memcpy(text_buf, Data, actual_len);
        text_buf[actual_len] = '\0';
        
        rc = sqlite3_bind_text64(stmt, 3, text_buf, actual_len, SQLITE_STATIC, SQLITE_UTF8);
        (void)rc;
        free(text_buf);
      }
    }
    
    // Try to step the statement to execute it (optional, but explores state)
    rc = sqlite3_step(stmt);
    (void)rc;
    
    // Reset to allow re-binding if we wanted to loop, but for fuzzing one pass is enough
    sqlite3_reset(stmt);
  }

  // Cleanup
  if (stmt) {
    sqlite3_finalize(stmt);
  }
  if (db) {
    sqlite3_close(db);
  }

  return 0;
}