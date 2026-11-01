// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *p) {
    (void)p;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to ensure the statement can be prepared
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT); "
                      "INSERT INTO test VALUES (?, ?);";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // 2. sqlite3_bind_parameter_name
    // Try to get the name of the first parameter
    const char *param_name = sqlite3_bind_parameter_name(stmt, 1);
    (void)param_name; // Suppress unused warning, just exercising the API

    // 3. sqlite3_bind_int
    // Bind an integer to the first parameter if available
    if (param_count >= 1) {
        int fuzz_int = 0;
        if (Size >= 4) {
            memcpy(&fuzz_int, Data, 4);
        }
        sqlite3_bind_int(stmt, 1, fuzz_int);
    }

    // 4. sqlite3_malloc64
    // Allocate memory based on input size, capped to avoid huge allocations
    sqlite3_uint64 alloc_size = 0;
    if (Size > 0) {
        alloc_size = (sqlite3_uint64)Data[0] * 1024; // Scale up a bit but keep it reasonable
        if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024; // Cap at 1MB
    }
    void *mem = sqlite3_malloc64(alloc_size);
    if (mem != NULL) {
        // Zero out the allocated memory to ensure clean state
        memset(mem, 0, alloc_size);
        sqlite3_free(mem);
    }

    // 5. sqlite3_bind_text64
    // Bind text to the second parameter if available
    if (param_count >= 2 && Size > 0) {
        // Use a portion of the input as the text to bind
        // Ensure we don't bind a string with internal nulls as a C string if we want strict behavior,
        // but sqlite3_bind_text64 takes a length, so it's fine.
        sqlite3_uint64 text_len = Size;
        if (text_len > 10000) text_len = 10000; // Cap text length
        
        // Ensure we don't go out of bounds
        if (text_len > Size) text_len = Size;

        // Use the input data directly
        const char *text_data = (const char *)Data;
        
        // SQLITE_STATIC means we don't need to free the text buffer
        // SQLITE_UTF8 is the standard encoding
        sqlite3_bind_text64(stmt, 2, text_data, text_len, SQLITE_STATIC, SQLITE_UTF8);
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}