// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int get_int_from_data(const uint8_t *data, size_t size, int offset) {
    if (offset + 4 > (int)size) return 0;
    int val;
    memcpy(&val, data + offset, sizeof(int));
    return val;
}

static int64_t get_int64_from_data(const uint8_t *data, size_t size, int offset) {
    if (offset + 8 > (int)size) return 0;
    int64_t val;
    memcpy(&val, data + offset, sizeof(int64_t));
    return val;
}

static double get_double_from_data(const uint8_t *data, size_t size, int offset) {
    if (offset + 8 > (int)size) return 0.0;
    double val;
    memcpy(&val, data + offset, sizeof(double));
    return val;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5;";
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Prepare statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_bind_int
    if (Size >= 4) {
        int val1 = get_int_from_data(Data, Size, 0);
        sqlite3_bind_int(stmt, 1, val1);
    }

    // 2. sqlite3_bind_int64
    if (Size >= 12) {
        int64_t val2 = get_int64_from_data(Data, Size, 4);
        sqlite3_bind_int64(stmt, 2, val2);
    }

    // 3. sqlite3_bind_double
    if (Size >= 20) {
        double val3 = get_double_from_data(Data, Size, 12);
        sqlite3_bind_double(stmt, 3, val3);
    }

    // 4. sqlite3_bind_text64
    if (Size >= 24) {
        sqlite3_uint64 text_len = (sqlite3_uint64)get_int64_from_data(Data, Size, 20);
        if (text_len > Size - 24) {
            text_len = Size - 24;
        }
        if (text_len > 1000000) {
            text_len = 1000000;
        }
        
        const char *text_data = (const char *)(Data + 24);
        // Use SQLITE_TRANSIENT to avoid memory management issues
        sqlite3_bind_text64(stmt, 4, text_data, text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
    }

    // 5. sqlite3_bind_int (second call)
    if (Size >= 28) {
        int val5 = get_int_from_data(Data, Size, 28);
        sqlite3_bind_int(stmt, 5, val5);
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}