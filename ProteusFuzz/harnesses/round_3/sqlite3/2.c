// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to prepare a statement with parameters
    const char *create_sql = "CREATE TABLE test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    // Prepare a statement with named and positional parameters
    const char *sql = "INSERT INTO test (id, name) VALUES (:id, ?1);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        cleanup_db(db);
        return 0;
    }

    // 1. sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) {
        cleanup_stmt(stmt);
        cleanup_db(db);
        return 0;
    }

    // 2. sqlite3_bind_parameter_name
    // Try to get the name of the first parameter (1-indexed)
    const char *param_name = sqlite3_bind_parameter_name(stmt, 1);
    (void)param_name; // Use the result to avoid unused variable warning

    // 3. sqlite3_bind_int
    // Bind an integer to the first parameter
    int bind_int_val = 0;
    if (Size > 0) {
        bind_int_val = (int)Data[0];
    }
    rc = sqlite3_bind_int(stmt, 1, bind_int_val);
    if (rc != SQLITE_OK) {
        cleanup_stmt(stmt);
        cleanup_db(db);
        return 0;
    }

    // 4. sqlite3_malloc64
    // Allocate some memory based on input size
    sqlite3_uint64 alloc_size = (Size > 0) ? (sqlite3_uint64)Data[0] : 100;
    void *ptr = sqlite3_malloc64(alloc_size);
    if (ptr == NULL) {
        // Allocation failed, but we can continue or return
        cleanup_stmt(stmt);
        cleanup_db(db);
        return 0;
    }
    // Initialize the allocated memory to avoid uninitialized read issues
    memset(ptr, 0, alloc_size);

    // 5. sqlite3_bind_text64
    // Bind text to the second parameter (index 2)
    // Use the allocated memory as the text buffer
    sqlite3_uint64 text_len = (Size > 1) ? (sqlite3_uint64)Data[1] : 0;
    if (text_len > alloc_size) {
        text_len = alloc_size;
    }
    
    // Ensure we have a valid string (null-terminate if needed for safety, though bind_text64 uses length)
    if (text_len < alloc_size) {
        ((char*)ptr)[text_len] = '\0';
    }

    // Bind the text. Use SQLITE_STATIC so SQLite doesn't try to free the memory
    // The xDel argument is SQLITE_STATIC because we allocated with sqlite3_malloc64
    // and will free it ourselves.
    rc = sqlite3_bind_text64(stmt, 2, (const char *)ptr, text_len, SQLITE_STATIC, SQLITE_UTF8);
    
    // Free the allocated memory
    sqlite3_free(ptr);

    // Cleanup
    cleanup_stmt(stmt);
    cleanup_db(db);

    return 0;
}