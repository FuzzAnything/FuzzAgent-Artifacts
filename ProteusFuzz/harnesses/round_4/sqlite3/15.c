// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
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
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static inline int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int val;
    memcpy(&val, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return val;
}

static inline sqlite3_int64 read_int64(const uint8_t **data, size_t *size) {
    if (*size < sizeof(sqlite3_int64)) return 0;
    sqlite3_int64 val;
    memcpy(&val, *data, sizeof(sqlite3_int64));
    *data += sizeof(sqlite3_int64);
    *size -= sizeof(sqlite3_int64);
    return val;
}

static inline const char *read_text(const uint8_t **data, size_t *size, int *len) {
    if (*size < sizeof(int)) return NULL;
    int l;
    memcpy(&l, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    if (l < 0 || *size < (size_t)l) {
        return NULL;
    }
    *len = l;
    const char *text = (const char *)*data;
    *data += l;
    *size -= l;
    return text;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO test VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (c1 INTEGER, c2 INTEGER, c3 TEXT, c4 INTEGER, c5 INTEGER, c6 TEXT, c7 INTEGER, c8 INTEGER, c9 TEXT, c10 INTEGER, c11 INTEGER, c12 TEXT, c13 TEXT, c14 TEXT, c15 INTEGER, c16 INTEGER, c17 INTEGER, c18 INTEGER, c19 TEXT, c20 INTEGER, c21 INTEGER, c22 INTEGER, c23 INTEGER, c24 INTEGER, c25 INTEGER, c26 INTEGER, c27 INTEGER, c28 INTEGER, c29 INTEGER, c30 TEXT, c31 INTEGER, c32 INTEGER, c33 TEXT, c34 TEXT);";
    char *err_msg = NULL;
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    const uint8_t *ptr = Data;
    size_t remaining = Size;
    int param_idx = 1;

    // 1. sqlite3_bind_int64
    if (remaining >= sizeof(sqlite3_int64)) {
        sqlite3_int64 val64 = read_int64(&ptr, &remaining);
        sqlite3_bind_int64(stmt, param_idx++, val64);
    }
    // 2. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 3. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 4. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 5. sqlite3_bind_int64
    if (remaining >= sizeof(sqlite3_int64)) {
        sqlite3_int64 val64 = read_int64(&ptr, &remaining);
        sqlite3_bind_int64(stmt, param_idx++, val64);
    }
    // 6. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 7. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 8. sqlite3_bind_int64
    if (remaining >= sizeof(sqlite3_int64)) {
        sqlite3_int64 val64 = read_int64(&ptr, &remaining);
        sqlite3_bind_int64(stmt, param_idx++, val64);
    }
    // 9. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 10. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 11. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 12. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 13. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 14. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 15. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 16. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 17. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 18. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 19. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 20. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 21. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 22. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 23. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 24. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 25. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 26. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 27. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 28. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 29. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 30. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 31. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 32. sqlite3_libversion_number
    (void)sqlite3_libversion_number();
    // 33. sqlite3_bind_int
    if (remaining >= sizeof(int)) {
        int val = read_int(&ptr, &remaining);
        sqlite3_bind_int(stmt, param_idx++, val);
    }
    // 34. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }
    // 35. sqlite3_bind_text
    if (remaining >= sizeof(int)) {
        int len;
        const char *text = read_text(&ptr, &remaining, &len);
        if (text) sqlite3_bind_text(stmt, param_idx++, text, len, SQLITE_TRANSIENT);
    }

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}