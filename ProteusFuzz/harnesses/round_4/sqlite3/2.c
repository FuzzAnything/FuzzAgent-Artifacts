// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to ensure valid SQL context
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with named parameters to test binding functions
    const char *sql = "INSERT INTO test_table (id, name) VALUES (:id, :name);";
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
    (void)param_name; // Suppress unused warning

    // 3. sqlite3_bind_int
    // Bind an integer to the first parameter if it exists
    if (param_count >= 1) {
        int int_val = 0;
        if (Size > 0) {
            int_val = (int)Data[0];
        }
        rc = sqlite3_bind_int(stmt, 1, int_val);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    // 4. sqlite3_malloc64
    sqlite3_uint64 alloc_size = 0;
    if (Size > 0) {
        alloc_size = (sqlite3_uint64)Data[0] * 100;
        if (alloc_size == 0) alloc_size = 10; // Ensure non-zero allocation
    } else {
        alloc_size = 10;
    }

    // Limit allocation size to prevent excessive memory usage or overflow
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    void *ptr = sqlite3_malloc64(alloc_size);
    if (ptr == NULL) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Fill allocated memory with fuzz data if available
    if (Size > 0 && alloc_size > 0) {
        size_t copy_size = (alloc_size < Size) ? (size_t)alloc_size : Size;
        memcpy(ptr, Data, copy_size);
    }

    // 5. sqlite3_bind_text64
    // Bind text to the second parameter if it exists
    if (param_count >= 2) {
        int text_len = 0;
        if (Size > 1) {
            text_len = Data[1] % 100; // Limit length
        }
        
        // Ensure we have enough data for the text
        const char *text_data = "default";
        size_t text_len_actual = 7;
        
        if (Size > 2) {
            // We have data after the first two bytes
            if (text_len == 0) text_len = 7;
            if ((size_t)text_len > (Size - 2)) {
                text_len = (int)(Size - 2);
            }
            if (text_len > 0) {
                text_data = (const char *)(Data + 2);
                text_len_actual = (size_t)text_len;
            }
        } else {
            // Not enough data, use default
            text_len_actual = 7;
        }

        // Use SQLITE_TRANSIENT to indicate SQLite should make a copy
        // Ensure text_len_actual matches the actual string length or is safe
        rc = sqlite3_bind_text64(stmt, 2, text_data, (sqlite3_uint64)text_len_actual, SQLITE_TRANSIENT, SQLITE_UTF8);
        
        // Free the allocated memory if binding succeeded or failed, but we own it
        if (ptr != NULL) {
            sqlite3_free(ptr);
        }

        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    } else {
        // If no second parameter, just free the memory
        if (ptr != NULL) {
            sqlite3_free(ptr);
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}