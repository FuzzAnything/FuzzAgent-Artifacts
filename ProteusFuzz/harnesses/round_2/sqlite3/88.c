// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_blob64 at sqlite3.c:92722:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:92788:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:92844:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
  int choice = 0;
  if (argc > 0) {
    choice = sqlite3_value_int(argv[0]) % 6;
  }

  switch (choice) {
    case 0:
      sqlite3_result_null(ctx);
      break;
    case 1:
      sqlite3_result_error_toobig(ctx);
      break;
    case 2: {
      if (argc > 1) {
        const void *blob = sqlite3_value_blob(argv[1]);
        int len = sqlite3_value_bytes(argv[1]);
        sqlite3_result_blob64(ctx, blob, (sqlite3_uint64)len, NULL);
      } else {
        sqlite3_result_null(ctx);
      }
      break;
    }
    case 3: {
      void *ptr = malloc(10);
      if (ptr) {
        sqlite3_result_pointer(ctx, ptr, "my_type", free);
      } else {
        sqlite3_result_null(ctx);
      }
      break;
    }
    case 4: {
      if (argc > 1) {
        const char *text = (const char *)sqlite3_value_text(argv[1]);
        int len = sqlite3_value_bytes(argv[1]);
        sqlite3_result_text64(ctx, text, (sqlite3_uint64)len, NULL, SQLITE_UTF8);
      } else {
        sqlite3_result_null(ctx);
      }
      break;
    }
    case 5: {
      if (argc > 1) {
        const char *text = (const char *)sqlite3_value_text(argv[1]);
        int len = sqlite3_value_bytes(argv[1]);
        sqlite3_result_text(ctx, text, len, NULL);
      } else {
        sqlite3_result_null(ctx);
      }
      break;
    }
  }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  int rc;

  rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, NULL, dummy_function, NULL, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  const char *sql = "SELECT fuzz_func(?, ?)";
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  if (Size > 0) {
    sqlite3_bind_int(stmt, 1, Data[0]);
  }
  if (Size > 1) {
    sqlite3_bind_text(stmt, 2, (const char *)&Data[1], Size - 1, SQLITE_TRANSIENT);
  }

  rc = sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return 0;
}