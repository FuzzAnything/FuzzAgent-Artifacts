// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:94185:24 in sqlite3.h
// sqlite3_bind_parameter_index at sqlite3.c:94200:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:94077:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 4) {
        return 0;
    }

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to ensure valid SQL context
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Construct a SQL statement with parameters from input
    // We limit the SQL length to avoid huge allocations and ensure it's valid
    size_t sql_len = Size;
    if (sql_len > 1000) sql_len = 1000;

    // Ensure null termination
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_buf, Data, sql_len);
    sql_buf[sql_len] = '\0';

    // Prepare the statement
    rc = sqlite3_prepare_v2(db, sql_buf, -1, &stmt, NULL);
    free(sql_buf);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);

    // 2. sqlite3_bind_parameter_name
    for (int i = 1; i <= param_count; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        // Use name to try binding by name later if not NULL
        if (name && strlen(name) > 0) {
            int idx = sqlite3_bind_parameter_index(stmt, name);
            if (idx > 0) {
                // Bind int64 using index derived from name
                sqlite3_bind_int64(stmt, idx, (sqlite3_int64)Size);
            }
        }
    }

    // 3. Direct binding using index
    for (int i = 1; i <= param_count; i++) {
        // Bind int
        sqlite3_bind_int(stmt, i, (int)(Size % 10000));
        
        // Bind int64
        sqlite3_bind_int64(stmt, i, (sqlite3_int64)Size);

        // Bind text64
        // Create a small text buffer from input to avoid huge allocations
        size_t text_len = Size > 100 ? 100 : Size;
        char *text_buf = (char *)malloc(text_len + 1);
        if (text_buf) {
            memcpy(text_buf, Data, text_len);
            text_buf[text_len] = '\0';
            
            // SQLITE_STATIC: caller manages memory
            sqlite3_bind_text64(stmt, i, text_buf, text_len, SQLITE_STATIC, SQLITE_UTF8);
            free(text_buf);
        }
    }

    // Execute the statement if it's not NULL and valid
    // We don't care about the result of step, just exercising the code path
    if (stmt) {
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return 0;
}