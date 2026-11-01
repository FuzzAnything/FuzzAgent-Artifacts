// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* 
 * The fuzzer entry point. 
 * This function must be global (not static) to be linked correctly by libFuzzer.
 */
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to ensure valid SQL for preparation
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with named parameters to test binding functions
    const char *sql = "INSERT INTO test (id, name) VALUES (:id, :name);";
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
    (void)param_name; // Use the value to avoid unused warning

    // 3. sqlite3_bind_int
    // Bind an integer to the first parameter if it exists
    int bind_int_val = 0;
    if (Size > 0) {
        bind_int_val = (int)Data[0];
    }
    rc = sqlite3_bind_int(stmt, 1, bind_int_val);
    // Ignore rc for fuzzing purposes, we want to explore states

    // 4. sqlite3_malloc64
    // Allocate memory based on input size, clamped to reasonable limits
    sqlite3_uint64 alloc_size = 0;
    if (Size > 0) {
        alloc_size = (sqlite3_uint64)Data[0];
    }
    // Clamp to avoid huge allocations in fuzzing
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }
    
    void *ptr = sqlite3_malloc64(alloc_size);
    if (ptr != NULL) {
        // Write some data to the allocated memory to ensure it's valid
        memset(ptr, 0xAA, alloc_size);
        sqlite3_free(ptr);
    }

    // 5. sqlite3_bind_text64
    // Prepare text data for binding
    const char *text_data = (const char *)Data;
    sqlite3_uint64 text_len = Size;
    
    // Ensure we don't bind more than available data
    if (Size > 0 && text_len > Size) {
        text_len = Size;
    }
    
    // Bind text to the second parameter (index 2)
    // Use SQLITE_TRANSIENT to make a copy.
    rc = sqlite3_bind_text64(stmt, 2, text_data, text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
    // Ignore rc to explore error states

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}