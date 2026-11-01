// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_libversion_number at sqlite3.c:185744:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void safe_bind_text(sqlite3_stmt *s, int idx, const uint8_t *data, size_t size) {
    if (size == 0) {
        sqlite3_bind_text(s, idx, "", 0, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(s, idx, (const char *)data, size, SQLITE_TRANSIENT);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?;";
    int rc;

    if (Size < 1) {
        return 0;
    }

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 1, (sqlite3_int64)Data[0]);

    // 2. sqlite3_bind_int
    sqlite3_bind_int(stmt, 2, Data[0]);

    // 3. sqlite3_bind_text
    safe_bind_text(stmt, 3, Data, Size);

    // 4. sqlite3_bind_int
    sqlite3_bind_int(stmt, 4, Data[0]);

    // 5. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 5, (sqlite3_int64)Data[0]);

    // 6. sqlite3_bind_text
    safe_bind_text(stmt, 6, Data, Size);

    // 7. sqlite3_bind_int
    sqlite3_bind_int(stmt, 7, Data[0]);

    // 8. sqlite3_bind_int64
    sqlite3_bind_int64(stmt, 8, (sqlite3_int64)Data[0]);

    // 9. sqlite3_bind_text
    safe_bind_text(stmt, 9, Data, Size);

    // 10. sqlite3_bind_int
    sqlite3_bind_int(stmt, 10, Data[0]);

    // 11. sqlite3_bind_int
    sqlite3_bind_int(stmt, 11, Data[0]);

    // 12. sqlite3_bind_text
    safe_bind_text(stmt, 12, Data, Size);

    // 13. sqlite3_bind_text
    safe_bind_text(stmt, 13, Data, Size);

    // 14. sqlite3_bind_text
    safe_bind_text(stmt, 14, Data, Size);

    // 15. sqlite3_bind_int
    sqlite3_bind_int(stmt, 15, Data[0]);

    // 16. sqlite3_bind_int
    sqlite3_bind_int(stmt, 16, Data[0]);

    // 17. sqlite3_bind_int
    sqlite3_bind_int(stmt, 17, Data[0]);

    // 18. sqlite3_bind_int
    sqlite3_bind_int(stmt, 18, Data[0]);

    // 19. sqlite3_bind_text
    safe_bind_text(stmt, 19, Data, Size);

    // 20. sqlite3_bind_int
    sqlite3_bind_int(stmt, 20, Data[0]);

    // 21. sqlite3_bind_int
    sqlite3_bind_int(stmt, 21, Data[0]);

    // 22. sqlite3_bind_int
    sqlite3_bind_int(stmt, 22, Data[0]);

    // 23. sqlite3_bind_int
    sqlite3_bind_int(stmt, 23, Data[0]);

    // 24. sqlite3_bind_int
    sqlite3_bind_int(stmt, 24, Data[0]);

    // 25. sqlite3_bind_int
    sqlite3_bind_int(stmt, 25, Data[0]);

    // 26. sqlite3_bind_int
    sqlite3_bind_int(stmt, 26, Data[0]);

    // 27. sqlite3_bind_int
    sqlite3_bind_int(stmt, 27, Data[0]);

    // 28. sqlite3_bind_int
    sqlite3_bind_int(stmt, 28, Data[0]);

    // 29. sqlite3_bind_text
    safe_bind_text(stmt, 29, Data, Size);

    // 30. sqlite3_bind_int
    sqlite3_bind_int(stmt, 30, Data[0]);

    // 31. sqlite3_libversion_number
    (void)sqlite3_libversion_number();

    // 32. sqlite3_bind_int
    sqlite3_bind_int(stmt, 32, Data[0]);

    // 33. sqlite3_bind_text
    safe_bind_text(stmt, 33, Data, Size);

    // 34. sqlite3_bind_text
    safe_bind_text(stmt, 34, Data, Size);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}