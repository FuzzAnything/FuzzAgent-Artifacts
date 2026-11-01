// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table
    const char *create_sql = "CREATE TABLE test (id INTEGER, name TEXT, val BIGINT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with positional parameters for fuzzing
    const char *select_sql = "SELECT * FROM test WHERE id = ? AND name = ?2 AND val = ?3;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Fuzz sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Fuzz sqlite3_bind_parameter_name
    for (int i = 1; i <= param_count; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        (void)name; // Suppress unused variable warning
    }

    // Fuzz sqlite3_bind_parameter_index with data from input
    if (Size > 0) {
        // Create a null-terminated string from input data
        size_t name_len = (Size < 255) ? Size : 255;
        char name_buf[256];
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        
        int idx = sqlite3_bind_parameter_index(stmt, name_buf);
        if (idx > 0) {
            // If a valid index is found, try binding an integer
            if (Size >= 4) {
                int int_val = *(const int*)(Data + name_len);
                sqlite3_bind_int(stmt, idx, int_val);
            }
        }
    }

    // Fuzz sqlite3_bind_int with input data
    if (Size >= 4) {
        int int_val = *(const int*)(Data);
        sqlite3_bind_int(stmt, 1, int_val);
    }

    // Fuzz sqlite3_bind_int64 with input data
    if (Size >= 8) {
        sqlite3_int64 int64_val = *(const sqlite3_int64*)(Data);
        sqlite3_bind_int64(stmt, 3, int64_val);
    }

    // Fuzz sqlite3_bind_text64 with input data
    if (Size >= 4) {
        int text_len = *(const int*)(Data);
        // Ensure text_len is positive and within bounds
        if (text_len > 0 && (size_t)text_len < Size - 4) {
            const char *text_data = (const char*)(Data + 4);
            sqlite3_bind_text64(stmt, 2, text_data, (sqlite3_uint64)text_len, SQLITE_TRANSIENT, SQLITE_UTF8);
        }
    }

    // Execute the statement to trigger potential errors
    sqlite3_step(stmt);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}