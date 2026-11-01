// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to prepare a statement against
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with parameters based on input data
    const char *sql_template = "INSERT INTO test (id, name) VALUES (?, ?);";
    
    rc = sqlite3_prepare_v2(db, sql_template, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Fuzzing logic:
    // 1. Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // 2. Test sqlite3_bind_parameter_name for each parameter
    for (int i = 1; i <= param_count; i++) {
        const char *name = sqlite3_bind_parameter_name(stmt, i);
        (void)name; // Use the result to avoid unused warning
    }

    // 3. Test sqlite3_bind_parameter_index with names from input
    for (size_t i = 0; i + 1 < Size; i++) {
        char name_buf[64];
        size_t len = 0;
        // Extract a potential name from input
        if (Data[i] == ':' || Data[i] == '$' || Data[i] == '@' || Data[i] == '?') {
            size_t j = i + 1;
            while (j < Size && len < 63 && (isalnum((unsigned char)Data[j]) || Data[j] == '_')) {
                name_buf[len++] = Data[j++];
            }
            name_buf[len] = '\0';
            if (len > 0) {
                int idx = sqlite3_bind_parameter_index(stmt, name_buf);
                if (idx > 0) {
                    // If valid index, try binding
                    if (idx <= param_count) {
                        // Bind a small integer
                        sqlite3_bind_int(stmt, idx, Data[i] & 0xFF);
                    }
                }
            }
        }
    }

    // 4. Directly bind using indices derived from input
    for (int i = 1; i <= param_count; i++) {
        // Bind int
        if (Size > 0) {
            sqlite3_bind_int(stmt, i, Data[0]);
        }
        
        // Bind int64
        if (Size >= 8) {
            sqlite3_int64 val64 = 0;
            memcpy(&val64, Data, 8);
            sqlite3_bind_int64(stmt, i, val64);
        }
        
        // Bind text
        if (Size >= 2) {
            const char *text = (const char *)Data;
            sqlite3_uint64 len = (Size > 100) ? 100 : Size;
            // Use SQLITE_STATIC to avoid memory management issues in fuzzing
            sqlite3_bind_text64(stmt, i, text, len, SQLITE_STATIC, SQLITE_UTF8);
        }
    }

    // 5. Reset and try again to explore more states
    sqlite3_reset(stmt);
    
    // Re-bind with different values
    for (int i = 1; i <= param_count; i++) {
        sqlite3_bind_int(stmt, i, 0);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    stmt = NULL;

    // Close the database
    sqlite3_close(db);
    db = NULL;

    return 0;
}