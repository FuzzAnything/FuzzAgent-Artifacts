// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static void cleanup_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int errcode;

    // Use a dummy file for database operations
    const char *filename = "./dummy_file";

    // Open database with various flags to explore different states
    int flags = (Data[0] & 0x03) | SQLITE_OPEN_CREATE;
    if (Data[0] & 0x04) flags |= SQLITE_OPEN_READONLY;
    if (Data[0] & 0x08) flags |= SQLITE_OPEN_READWRITE;
    if (Data[0] & 0x10) flags |= SQLITE_OPEN_URI;

    // Ensure VFS string is safe (heap allocated or static)
    static const char *vfs_name = NULL;

    rc = sqlite3_open_v2(filename, &db, flags, vfs_name);
    if (rc != SQLITE_OK && rc != SQLITE_CANTOPEN && rc != SQLITE_BUSY) {
        // Ignore non-critical open errors, but clean up if we got a db handle
        cleanup_db(db);
        return 0;
    }

    if (db) {
        // Test sqlite3_set_errmsg
        // Use fuzz data to determine error code and message
        errcode = (Data[1] & 0xFF);
        if (errcode == 0) errcode = SQLITE_ERROR; // Default to a valid error code if 0

        // Create a safe message buffer from fuzz data
        size_t msg_len = (Size > 10) ? (Size - 10) : 0;
        char *msg_buf = NULL;
        if (msg_len > 0) {
            msg_buf = malloc(msg_len + 1);
            if (msg_buf) {
                memcpy(msg_buf, &Data[2], msg_len);
                msg_buf[msg_len] = '\0';
            }
        }

        // Call set_errmsg with potentially invalid/valid parameters
        if (msg_buf) {
            sqlite3_set_errmsg(db, errcode, msg_buf);
            free(msg_buf);
        } else {
            // Try with NULL message
            sqlite3_set_errmsg(db, errcode, NULL);
        }

        // Test sqlite3_errcode to verify state
        int current_err = sqlite3_errcode(db);
        (void)current_err; // Suppress unused warning

        // Test sqlite3_prepare and sqlite3_prepare_v3
        // Use fuzz data as SQL string
        size_t sql_len = Size;
        const char *sql = (const char *)Data;

        const char *tail = NULL;

        // Try sqlite3_prepare (legacy)
        rc = sqlite3_prepare(db, sql, (int)sql_len, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            // Bind text if possible
            if (Size > 20) {
                int param_idx = (Data[20] % 10) + 1; // 1-based index
                size_t bind_len = (Size > 30) ? (Size - 30) : 0;
                if (bind_len > 0) {
                    sqlite3_bind_text(stmt, param_idx, (const char *)&Data[21], (int)bind_len, SQLITE_TRANSIENT);
                }
            }
            cleanup_stmt(stmt);
            stmt = NULL;
        }

        // Try sqlite3_prepare_v3
        unsigned int prepFlags = (Data[0] & 0x20) ? SQLITE_PREPARE_PERSISTENT : 0;
        rc = sqlite3_prepare_v3(db, sql, (int)sql_len, prepFlags, &stmt, &tail);
        if (rc == SQLITE_OK && stmt) {
            // Bind text again with different parameters
            if (Size > 25) {
                int param_idx = (Data[25] % 10) + 1;
                size_t bind_len = (Size > 35) ? (Size - 35) : 0;
                if (bind_len > 0) {
                    sqlite3_bind_text(stmt, param_idx, (const char *)&Data[26], (int)bind_len, SQLITE_STATIC);
                }
            }
            cleanup_stmt(stmt);
            stmt = NULL;
        }

        // Additional check for error code after operations
        int final_err = sqlite3_errcode(db);
        (void)final_err;
    }

    cleanup_db(db);
    return 0;
}