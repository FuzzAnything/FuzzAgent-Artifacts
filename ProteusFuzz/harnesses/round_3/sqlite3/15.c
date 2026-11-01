// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_libversion_number at sqlite3.c:185744:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static inline int read_int(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + sizeof(int) > size) {
    *offset = 0;
    return 0;
  }
  int val = 0;
  memcpy(&val, data + *offset, sizeof(int));
  *offset += sizeof(int);
  return val;
}

static inline sqlite3_int64 read_int64(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + sizeof(sqlite3_int64) > size) {
    *offset = 0;
    return 0;
  }
  sqlite3_int64 val = 0;
  memcpy(&val, data + *offset, sizeof(sqlite3_int64));
  *offset += sizeof(sqlite3_int64);
  return val;
}

static inline const char* read_text(const uint8_t *data, size_t size, size_t *offset, int *len) {
  if (*offset >= size) {
    *offset = 0;
    *len = 0;
    return NULL;
  }
  int max_len = (int)(size - *offset);
  if (max_len > 1000) max_len = 1000;
  *len = max_len;
  const char *text = (const char *)(data + *offset);
  *offset += max_len;
  return text;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
  sqlite3 *db = NULL;
  sqlite3_stmt *stmt = NULL;
  const char *sql = "INSERT INTO test_table VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
  
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    return 0;
  }

  const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test_table (c1 INTEGER, c2 INTEGER, c3 TEXT, c4 INTEGER, c5 INTEGER, c6 TEXT, c7 INTEGER, c8 INTEGER, c9 TEXT, c10 INTEGER, c11 INTEGER, c12 TEXT, c13 TEXT, c14 TEXT, c15 INTEGER, c16 INTEGER, c17 INTEGER, c18 INTEGER, c19 TEXT, c20 INTEGER, c21 INTEGER, c22 INTEGER, c23 INTEGER, c24 INTEGER, c25 INTEGER, c26 INTEGER, c27 INTEGER, c28 INTEGER, c29 INTEGER, c30 TEXT, c31 INTEGER);";
  sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  size_t idx = 0;
  int param_idx = 1;
  int text_len;
  const char *text;

  // 1. sqlite3_bind_int64
  sqlite3_bind_int64(stmt, param_idx++, read_int64(Data, Size, &idx));
  
  // 2. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 3. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 4. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 5. sqlite3_bind_int64
  sqlite3_bind_int64(stmt, param_idx++, read_int64(Data, Size, &idx));
  
  // 6. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 7. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 8. sqlite3_bind_int64
  sqlite3_bind_int64(stmt, param_idx++, read_int64(Data, Size, &idx));
  
  // 9. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 10. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 11. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 12. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 13. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 14. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 15. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 16. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 17. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 18. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 19. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 20. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 21. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 22. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 23. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 24. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 25. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 26. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 27. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 28. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 29. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 30. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 31. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 32. sqlite3_libversion_number
  (void)sqlite3_libversion_number();
  
  // 33. sqlite3_bind_int
  sqlite3_bind_int(stmt, param_idx++, read_int(Data, Size, &idx));
  
  // 34. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);
  
  // 35. sqlite3_bind_text
  text = read_text(Data, Size, &idx, &text_len);
  sqlite3_bind_text(stmt, param_idx++, text, text_len, SQLITE_TRANSIENT);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return 0;
}