// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_stmt(sqlite3_stmt **stmt) {
    if (stmt && *stmt) {
        sqlite3_finalize(*stmt);
        *stmt = NULL;
    }
}

static void cleanup_db(sqlite3 **db) {
    if (db && *db) {
        sqlite3_close(*db);
        *db = NULL;
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *zTail = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Ensure we have at least 1 byte to avoid empty SQL
    if (Size == 0) {
        cleanup_db(&db);
        return 0;
    }

    // Create a safe SQL string from the fuzz input
    // We prepend a simple CREATE TABLE to ensure valid SQL structure
    // and append the fuzz data as a comment or part of the string to test parsing
    size_t sql_len = Size + 20;
    char *sql = (char *)malloc(sql_len);
    if (!sql) {
        cleanup_db(&db);
        return 0;
    }

    // Construct SQL: "CREATE TABLE test(id INTEGER); " + fuzz_data
    // We limit the fuzz data part to avoid excessive memory usage in SQL parsing
    size_t fuzz_part = (Size > 1000) ? 1000 : Size;
    snprintf(sql, sql_len, "CREATE TABLE test(id INTEGER); -- %.*s", (int)fuzz_part, Data);

    // Prepare the statement using sqlite3_prepare_v3
    rc = sqlite3_prepare_v3(db, sql, -1, 0, &stmt, &zTail);
    free(sql);

    if (rc != SQLITE_OK) {
        // If preparation fails, try sqlite3_prepare (legacy) as a secondary test path
        // Re-construct sql for legacy call
        sql = (char *)malloc(sql_len);
        if (!sql) {
            cleanup_db(&db);
            return 0;
        }
        snprintf(sql, sql_len, "CREATE TABLE test(id INTEGER); -- %.*s", (int)fuzz_part, Data);
        
        rc = sqlite3_prepare(db, sql, -1, &stmt, &zTail);
        free(sql);
        
        if (rc != SQLITE_OK) {
            cleanup_db(&db);
            return 0;
        }
    }

    // Fuzz sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);

    // Fuzz sqlite3_bind_parameter_name
    // Try to get names for indices 0 to param_count + 2 (to test out of bounds)
    for (int i = 0; i <= param_count + 2; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        (void)name; // Use the result to avoid unused warning
    }

    // Fuzz sqlite3_bind_parameter_index
    // Try various names derived from the input data
    if (Size > 0) {
        // Create a temporary buffer for parameter names
        char *param_name = (char *)malloc(Size + 2);
        if (param_name) {
            // Try binding with the raw input as a name
            param_name[0] = ':'; // Prefix with ':' to make it a valid named param pattern
            memcpy(param_name + 1, Data, Size);
            param_name[Size + 1] = '\0';
            
            int idx = sqlite3_bind_parameter_index(stmt, param_name);
            (void)idx;

            // Try with a truncated version
            if (Size > 10) {
                param_name[11] = '\0';
                idx = sqlite3_bind_parameter_index(stmt, param_name);
                (void)idx;
            }
            free(param_name);
        }
    }

    // Fuzz sqlite3_bind_text64
    // Bind the fuzz data to the first parameter if it exists
    if (param_count > 0) {
        // Use the fuzz data as text to bind
        // We use SQLITE_TRANSIENT for xDel to let SQLite manage memory
        // Encoding: SQLITE_UTF8
        rc = sqlite3_bind_text64(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT, SQLITE_UTF8);
        (void)rc;

        // Try binding with a custom destructor (dummy)
        // Note: We pass the data again, but this time we don't free it in the destructor
        // to simulate a scenario where the user handles memory.
        // However, for fuzzing, we usually want to avoid leaks, so we stick to SQLITE_TRANSIENT or SQLITE_STATIC.
        // Let's try SQLITE_STATIC with a small subset if Size is large to avoid copying issues
        if (Size > 0) {
            rc = sqlite3_bind_text64(stmt, 1, (const char *)Data, 1, SQLITE_STATIC, SQLITE_UTF8);
            (void)rc;
        }
    }

    // Cleanup
    cleanup_stmt(&stmt);
    cleanup_db(&db);

    return 0;
}