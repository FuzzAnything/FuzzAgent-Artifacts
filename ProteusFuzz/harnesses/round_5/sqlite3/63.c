// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:94093:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    size_t idx = 0;

    // Initialize in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to allow parameter binding
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB, text TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with parameters
    const char *sql = "INSERT INTO test (data, text) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Fuzzing logic: iterate through input data to generate diverse bindings
    while (idx < Size) {
        // Determine operation type based on input byte
        uint8_t op = (idx < Size) ? Data[idx] : 0;
        idx++;

        // Ensure we have enough data for the operation
        if (idx >= Size) break;

        int param_idx = (Data[idx] % 2) + 1; // 1 or 2
        idx++;

        switch (op % 6) {
            case 0: {
                // sqlite3_bind_null
                rc = sqlite3_bind_null(stmt, param_idx);
                break;
            }
            case 1: {
                // sqlite3_bind_blob
                if (idx + 1 > Size) break;
                int len = Data[idx];
                idx++;
                if (idx + len > Size) len = Size - idx;
                rc = sqlite3_bind_blob(stmt, param_idx, (const void*)&Data[idx], len, SQLITE_TRANSIENT);
                idx += len;
                break;
            }
            case 2: {
                // sqlite3_bind_blob64
                if (idx + 8 > Size) break;
                sqlite3_uint64 len = 0;
                memcpy(&len, &Data[idx], 8);
                idx += 8;
                // Cap length to avoid huge allocations
                if (len > Size - idx) len = Size - idx;
                rc = sqlite3_bind_blob64(stmt, param_idx, (const void*)&Data[idx], len, SQLITE_TRANSIENT);
                idx += len;
                break;
            }
            case 3: {
                // sqlite3_bind_text16 (simulate UTF-16)
                if (idx + 1 > Size) break;
                int len = Data[idx] * 2; // Ensure even length for UTF-16
                idx++;
                if (idx + len > Size) len = (Size - idx) & ~1;
                if (len == 0) len = 2; // Minimum UTF-16 char
                if (idx + len > Size) break;
                
                // Create a temporary buffer for UTF-16 data
                uint16_t *utf16_buf = (uint16_t*)malloc(len);
                if (utf16_buf) {
                    memcpy(utf16_buf, &Data[idx], len);
                    rc = sqlite3_bind_text16(stmt, param_idx, utf16_buf, len, SQLITE_TRANSIENT);
                    free(utf16_buf);
                }
                idx += len;
                break;
            }
            case 4: {
                // Execute and reset cycle
                rc = sqlite3_step(stmt);
                rc = sqlite3_reset(stmt);
                break;
            }
            case 5: {
                // Check busy state
                int busy = sqlite3_stmt_busy(stmt);
                (void)busy; // Suppress unused warning
                break;
            }
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}