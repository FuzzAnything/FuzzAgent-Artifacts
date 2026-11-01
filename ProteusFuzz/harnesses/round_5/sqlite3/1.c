// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 10) {
        return 0;
    }

    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Prepare a simple SQL statement with named parameters
    const char *sql = "SELECT * FROM dummy WHERE id = :id AND name = :name;";
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
    // Try to get the name of the first parameter
    const char *param_name = sqlite3_bind_parameter_name(stmt, 1);
    (void)param_name; // Use the value to avoid unused variable warning

    // 3. sqlite3_bind_int
    // Bind an integer to the first parameter if it exists
    if (param_count >= 1) {
        int bind_val = (Size > 1) ? (int)Data[1] : 0;
        sqlite3_bind_int(stmt, 1, bind_val);
    }

    // 4. sqlite3_malloc64
    // Allocate some memory based on input size (bounded to prevent huge allocations)
    sqlite3_uint64 alloc_size = (Size > 1000000) ? 1000000 : Size;
    void *mem = sqlite3_malloc64(alloc_size);
    if (mem != NULL) {
        // Optionally use the memory, but for fuzzing, just freeing is enough
        sqlite3_free(mem);
    }

    // 5. sqlite3_bind_text64
    // Bind text to the second parameter if it exists
    if (param_count >= 2) {
        // Create a text buffer from input data
        size_t text_len = (Size > 2) ? (Size - 2) : 0;
        const char *text_data = (text_len > 0) ? (const char *)(Data + 2) : "";
        
        // Use SQLITE_TRANSIENT to let SQLite copy the data
        sqlite3_bind_text64(stmt, 2, text_data, text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
    }

    // Cleanup
    cleanup_stmt(stmt);
    cleanup_db(db);

    return 0;
}