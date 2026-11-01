// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
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
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_libversion_number at sqlite3.c:185744:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int get_int(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + 4 > size) {
        *idx = size;
        return 0;
    }
    int val = (data[*idx] << 24) | (data[*idx + 1] << 16) | (data[*idx + 2] << 8) | data[*idx + 3];
    *idx += 4;
    return val;
}

static sqlite3_int64 get_int64(const uint8_t *data, size_t *idx, size_t size) {
    if (*idx + 8 > size) {
        *idx = size;
        return 0;
    }
    sqlite3_int64 val = 0;
    for (int i = 0; i < 8; i++) {
        val = (val << 8) | data[*idx + i];
    }
    *idx += 8;
    return val;
}

static const char* get_text(const uint8_t *data, size_t *idx, size_t size, int *len) {
    if (*idx >= size) {
        *len = 0;
        return "";
    }
    int length = data[*idx];
    *idx += 1;
    if (*idx + length > size) {
        length = size - *idx;
    }
    *len = length;
    *idx += length;
    return (const char*)(data - length - 1);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to prepare a statement with many parameters
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table ("
                             "id INTEGER PRIMARY KEY, "
                             "col1 INTEGER, col2 INTEGER, col3 TEXT, col4 INTEGER, col5 INTEGER, "
                             "col6 TEXT, col7 INTEGER, col8 INTEGER, col9 TEXT, col10 INTEGER, "
                             "col11 INTEGER, col12 TEXT, col13 TEXT, col14 TEXT, col15 INTEGER, "
                             "col16 INTEGER, col17 INTEGER, col18 INTEGER, col19 TEXT, col20 INTEGER, "
                             "col21 INTEGER, col22 INTEGER, col23 INTEGER, col24 INTEGER, col25 INTEGER, "
                             "col26 INTEGER, col27 INTEGER, col28 INTEGER, col29 TEXT, col30 INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare an INSERT statement with many parameters matching the bind sequence
    const char *insert_sql = "INSERT INTO test_table "
                             "(col1, col2, col3, col4, col5, col6, col7, col8, col9, col10, "
                             "col11, col12, col13, col14, col15, col16, col17, col18, col19, col20, "
                             "col21, col22, col23, col24, col25, col26, col27, col28, col29, col30) "
                             "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    size_t idx = 0;

    // 1. sqlite3_bind_int64
    rc = sqlite3_bind_int64(stmt, 1, get_int64(Data, &idx, Size));
    // 2. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 2, get_int(Data, &idx, Size));
    // 3. sqlite3_bind_text
    int len;
    const char *txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 3, txt, len, SQLITE_TRANSIENT);
    // 4. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 4, get_int(Data, &idx, Size));
    // 5. sqlite3_bind_int64
    rc = sqlite3_bind_int64(stmt, 5, get_int64(Data, &idx, Size));
    // 6. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 6, txt, len, SQLITE_TRANSIENT);
    // 7. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 7, get_int(Data, &idx, Size));
    // 8. sqlite3_bind_int64
    rc = sqlite3_bind_int64(stmt, 8, get_int64(Data, &idx, Size));
    // 9. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 9, txt, len, SQLITE_TRANSIENT);
    // 10. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 10, get_int(Data, &idx, Size));
    // 11. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 11, get_int(Data, &idx, Size));
    // 12. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 12, txt, len, SQLITE_TRANSIENT);
    // 13. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 13, txt, len, SQLITE_TRANSIENT);
    // 14. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 14, txt, len, SQLITE_TRANSIENT);
    // 15. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 15, get_int(Data, &idx, Size));
    // 16. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 16, get_int(Data, &idx, Size));
    // 17. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 17, get_int(Data, &idx, Size));
    // 18. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 18, get_int(Data, &idx, Size));
    // 19. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 19, txt, len, SQLITE_TRANSIENT);
    // 20. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 20, get_int(Data, &idx, Size));
    // 21. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 21, get_int(Data, &idx, Size));
    // 22. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 22, get_int(Data, &idx, Size));
    // 23. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 23, get_int(Data, &idx, Size));
    // 24. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 24, get_int(Data, &idx, Size));
    // 25. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 25, get_int(Data, &idx, Size));
    // 26. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 26, get_int(Data, &idx, Size));
    // 27. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 27, get_int(Data, &idx, Size));
    // 28. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 28, get_int(Data, &idx, Size));
    // 29. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 29, txt, len, SQLITE_TRANSIENT);
    // 30. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 30, get_int(Data, &idx, Size));

    // Call sqlite3_libversion_number (can be called anytime)
    int version = sqlite3_libversion_number();
    (void)version; // Suppress unused warning

    // 31. sqlite3_bind_int
    rc = sqlite3_bind_int(stmt, 1, get_int(Data, &idx, Size));
    // 32. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 2, txt, len, SQLITE_TRANSIENT);
    // 33. sqlite3_bind_text
    txt = get_text(Data, &idx, Size, &len);
    rc = sqlite3_bind_text(stmt, 3, txt, len, SQLITE_TRANSIENT);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}