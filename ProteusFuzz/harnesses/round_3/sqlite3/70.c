// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:94137:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:94152:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:94004:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return 0;
    }
    return 1;
}

static int prepare_stmt(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        *stmt = NULL;
        return 0;
    }
    return 1;
}

static int bind_fuzz(sqlite3_stmt *stmt, const uint8_t *data, size_t size) {
    if (!stmt) return 0;
    
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) return 0;
    
    // Fuzz binding various types
    size_t idx = 0;
    for (int i = 1; i <= param_count; i++) {
        if (idx >= size) break;
        
        int type = data[idx] % 6;
        idx++;
        
        switch (type) {
            case 0: {
                // Bind NULL
                sqlite3_bind_null(stmt, i);
                break;
            }
            case 1: {
                // Bind zeroblob
                int len = 0;
                if (idx + 2 <= size) {
                    len = (data[idx] << 8) | data[idx+1];
                    idx += 2;
                }
                sqlite3_bind_zeroblob(stmt, i, len);
                break;
            }
            case 2: {
                // Bind zeroblob64
                sqlite3_uint64 len = 0;
                if (idx + 8 <= size) {
                    memcpy(&len, &data[idx], 8);
                    idx += 8;
                }
                sqlite3_bind_zeroblob64(stmt, i, len);
                break;
            }
            case 3: {
                // Bind blob64
                int len = 0;
                if (idx + 2 <= size) {
                    len = (data[idx] << 8) | data[idx+1];
                    idx += 2;
                }
                if (idx + len <= size) {
                    sqlite3_bind_blob64(stmt, i, (const void*)&data[idx], len, SQLITE_TRANSIENT);
                    idx += len;
                } else {
                    sqlite3_bind_blob64(stmt, i, NULL, 0, SQLITE_STATIC);
                }
                break;
            }
            case 4: {
                // Bind integer
                int val = 0;
                if (idx + 4 <= size) {
                    memcpy(&val, &data[idx], 4);
                    idx += 4;
                }
                sqlite3_bind_int(stmt, i, val);
                break;
            }
            case 5: {
                // Bind text
                int len = 0;
                if (idx + 2 <= size) {
                    len = (data[idx] << 8) | data[idx+1];
                    idx += 2;
                }
                if (idx + len <= size) {
                    sqlite3_bind_text(stmt, i, (const char*)&data[idx], len, SQLITE_TRANSIENT);
                    idx += len;
                } else {
                    sqlite3_bind_text(stmt, i, "", 0, SQLITE_STATIC);
                }
                break;
            }
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    sqlite3 *db = NULL;
    if (!create_db(&db)) return 0;
    
    // Create two statements for transfer_bindings test
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    
    // Use a simple SQL with parameters
    const char *sql = "INSERT INTO test (id, data) VALUES (?, ?)";
    
    // Create table first
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB)", NULL, NULL, NULL);
    
    if (!prepare_stmt(db, sql, &stmt1)) goto cleanup;
    if (!prepare_stmt(db, sql, &stmt2)) goto cleanup;
    
    // Fuzz binding on stmt1
    bind_fuzz(stmt1, Data, Size);
    
    // Test sqlite3_clear_bindings on stmt1
    sqlite3_clear_bindings(stmt1);
    
    // Re-bind after clear
    bind_fuzz(stmt1, Data, Size);
    
    // Test sqlite3_transfer_bindings
    // This should work if both statements have same parameter count
    sqlite3_transfer_bindings(stmt1, stmt2);
    
    // Execute statements to trigger any issues
    sqlite3_step(stmt1);
    sqlite3_step(stmt2);
    
    // Reset and try again with different bindings
    sqlite3_reset(stmt1);
    sqlite3_reset(stmt2);
    
    // Bind with different patterns
    int len = Size > 100 ? 100 : Size;
    sqlite3_bind_zeroblob(stmt1, 2, len);
    sqlite3_bind_zeroblob64(stmt2, 2, (sqlite3_uint64)len);
    
    // Try transfer again
    sqlite3_transfer_bindings(stmt1, stmt2);
    
    // Execute
    sqlite3_step(stmt1);
    sqlite3_step(stmt2);
    
cleanup:
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close(db);
    
    return 0;
}