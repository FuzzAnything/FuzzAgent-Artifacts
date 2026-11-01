// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int get_int(const uint8_t **p, size_t *sz) {
    if (*sz < sizeof(int)) {
        return 0;
    }
    int val = 0;
    memcpy(&val, *p, sizeof(int));
    *p += sizeof(int);
    *sz -= sizeof(int);
    return val;
}

static uint64_t get_uint64(const uint8_t **p, size_t *sz) {
    if (*sz < sizeof(uint64_t)) {
        return 0;
    }
    uint64_t val = 0;
    memcpy(&val, *p, sizeof(uint64_t));
    *p += sizeof(uint64_t);
    *sz -= sizeof(uint64_t);
    return val;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;

    // Initialize in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to ensure prepared statements can be created
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB, text TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare two statements for testing transfer_bindings
    const char *insert_sql = "INSERT INTO test (data, text) VALUES (?, ?);";
    
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }

    // Reset pointers for parsing
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Fuzz sqlite3_bind_null
    if (remaining >= sizeof(int)) {
        int idx = get_int(&ptr, &remaining);
        // Ensure index is within reasonable bounds to avoid immediate crash, 
        // but allow negative or large values to test error handling
        rc = sqlite3_bind_null(stmt1, idx);
        (void)rc; // Ignore return for fuzzing, just trigger path
    }

    // Fuzz sqlite3_bind_blob
    if (remaining >= sizeof(int) * 2) {
        int idx = get_int(&ptr, &remaining);
        int len = get_int(&ptr, &remaining);
        // Clamp length to avoid huge allocations if negative or huge
        if (len < 0) len = 0;
        if (len > (int)remaining) len = (int)remaining;
        
        // Use SQLITE_TRANSIENT to let SQLite copy data
        rc = sqlite3_bind_blob(stmt1, idx, ptr, len, SQLITE_TRANSIENT);
        (void)rc;
        ptr += len;
        remaining -= len;
    }

    // Fuzz sqlite3_bind_text16 (UTF-16)
    if (remaining >= sizeof(int) * 2) {
        int idx = get_int(&ptr, &remaining);
        int len = get_int(&ptr, &remaining);
        if (len < 0) len = 0;
        if (len > (int)remaining) len = (int)remaining;
        // Ensure even length for UTF-16 simulation, though SQLite handles odd lengths
        // by padding or erroring depending on implementation details.
        // We pass raw bytes to test robustness.
        rc = sqlite3_bind_text16(stmt1, idx, ptr, len, SQLITE_TRANSIENT);
        (void)rc;
        ptr += len;
        remaining -= len;
    }

    // Fuzz sqlite3_bind_blob64
    if (remaining >= sizeof(int) + sizeof(uint64_t)) {
        int idx = get_int(&ptr, &remaining);
        uint64_t len64 = get_uint64(&ptr, &remaining);
        if (len64 > remaining) len64 = remaining;
        
        rc = sqlite3_bind_blob64(stmt1, idx, ptr, len64, SQLITE_TRANSIENT);
        (void)rc;
        ptr += len64;
        remaining -= len64;
    }

    // Fuzz sqlite3_clear_bindings
    rc = sqlite3_clear_bindings(stmt1);
    (void)rc;

    // Fuzz sqlite3_transfer_bindings
    // This requires both statements to be valid and have compatible parameters
    rc = sqlite3_transfer_bindings(stmt1, stmt2);
    (void)rc;

    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);

    return 0;
}