// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_destructor(void *p) {
    if (p) {
        sqlite3_free(p);
    }
}

static inline int read_int(const uint8_t *data, size_t size, size_t offset) {
    int val = 0;
    if (size >= offset + sizeof(int)) {
        memcpy(&val, data + offset, sizeof(int));
    }
    return val;
}

static inline sqlite3_int64 read_int64(const uint8_t *data, size_t size, size_t offset) {
    sqlite3_int64 val = 0;
    if (size >= offset + sizeof(sqlite3_int64)) {
        memcpy(&val, data + offset, sizeof(sqlite3_int64));
    }
    return val;
}

static inline double read_double(const uint8_t *data, size_t size, size_t offset) {
    double val = 0.0;
    if (size >= offset + sizeof(double)) {
        memcpy(&val, data + offset, sizeof(double));
    }
    return val;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to prepare a statement
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT, value REAL, code INTEGER, big_code INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare an INSERT statement with 5 parameters
    const char *insert_sql = "INSERT INTO test (id, name, value, code, big_code) VALUES (?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_bind_int (param 1)
    int val_int = read_int(Data, Size, 0);
    sqlite3_bind_int(stmt, 1, val_int);

    // 2. sqlite3_bind_int64 (param 5)
    sqlite3_int64 val_int64 = read_int64(Data, Size, sizeof(int));
    sqlite3_bind_int64(stmt, 5, val_int64);

    // 3. sqlite3_bind_double (param 3)
    double val_double = read_double(Data, Size, sizeof(int) + sizeof(sqlite3_int64));
    sqlite3_bind_double(stmt, 3, val_double);

    // 4. sqlite3_bind_text64 (param 2)
    // Use remaining data for text, or a safe default
    size_t str_len = Size;
    const char *str_data = (const char *)Data;
    
    // Cap length to avoid huge allocations and ensure we don't exceed reasonable limits
    if (str_len > 1000000) {
        str_len = 1000000;
    }

    if (str_len > 0) {
        // Allocate memory for the string to use with a custom deleter
        char *allocated_str = (char *)sqlite3_malloc(str_len);
        if (allocated_str) {
            memcpy(allocated_str, str_data, str_len);
            sqlite3_bind_text64(stmt, 2, allocated_str, str_len, dummy_destructor, SQLITE_UTF8);
        } else {
            // Fallback if malloc fails: use static pointer with 0 length or small safe string
            sqlite3_bind_text64(stmt, 2, "", 0, SQLITE_STATIC, SQLITE_UTF8);
        }
    } else {
        sqlite3_bind_text64(stmt, 2, "", 0, SQLITE_STATIC, SQLITE_UTF8);
    }

    // 5. sqlite3_bind_int (param 4)
    // Read from a different offset to explore different states, ensuring bounds check
    int val_int2 = read_int(Data, Size, sizeof(int) + sizeof(sqlite3_int64) + sizeof(double));
    sqlite3_bind_int(stmt, 4, val_int2);

    // Attempt to step the statement to trigger execution
    rc = sqlite3_step(stmt);
    // Ignore rc for fuzzing, we just want to exercise the code path

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}