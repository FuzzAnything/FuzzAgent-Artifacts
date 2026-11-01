// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:94281:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:94234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static int create_in_memory_db(sqlite3 **db_out) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }
    if (db_out) {
        *db_out = db;
    }
    return 0;
}

static int prepare_utf16_statement(sqlite3 *db, const uint8_t *data, size_t size, sqlite3_stmt **stmt_out, const void **tail_out) {
    if (!db || !stmt_out || size == 0) {
        return -1;
    }

    // Create a UTF-16 buffer from input data
    // We interpret the fuzz input as raw bytes and convert to UTF-16LE for testing
    // We add a BOM to ensure it's recognized as UTF-16
    size_t utf16_size = (size + 1) * 2 + 2; // +1 for null terminator, +2 for BOM
    void *utf16_buf = malloc(utf16_size);
    if (!utf16_buf) {
        return -1;
    }

    // Write BOM (Little Endian)
    uint16_t *u16 = (uint16_t *)utf16_buf;
    *u16++ = 0xFEFF;

    // Copy data as UTF-16LE (simple conversion: 1 byte -> 2 bytes)
    for (size_t i = 0; i < size; i++) {
        if (data[i] == 0) break; // Stop at first null byte to ensure valid string
        *u16++ = (uint16_t)data[i];
    }
    *u16 = 0; // Null terminator

    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    int rc = sqlite3_prepare16_v2(db, utf16_buf, (int)(u16 - (uint16_t *)utf16_buf) * 2, &stmt, &tail);
    
    free(utf16_buf);

    if (rc != SQLITE_OK) {
        return -1;
    }

    if (stmt_out) *stmt_out = stmt;
    if (tail_out) *tail_out = tail;
    return 0;
}

static int prepare_utf8_statement(sqlite3 *db, const uint8_t *data, size_t size, sqlite3_stmt **stmt_out) {
    if (!db || !stmt_out || size == 0) {
        return -1;
    }

    // Ensure null termination
    char *sql = malloc(size + 1);
    if (!sql) return -1;
    memcpy(sql, data, size);
    sql[size] = '\0';

    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    free(sql);

    if (rc != SQLITE_OK) {
        return -1;
    }

    if (stmt_out) *stmt_out = stmt;
    return 0;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    if (create_in_memory_db(&db) != 0) {
        return 0;
    }

    // Create a simple table to ensure statements can execute
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, init_sql, NULL, NULL, NULL);

    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const void *tail = NULL;

    // Use first half of data for stmt1 (UTF-16) and second half for stmt2 (UTF-8) if possible
    // Or just try to prepare one statement based on the input
    int prepared = 0;
    
    // Try preparing a UTF-16 statement
    if (prepare_utf16_statement(db, Data, Size, &stmt1, &tail) == 0) {
        prepared = 1;
    } else {
        // Fallback to UTF-8 if UTF-16 fails or is invalid
        if (prepare_utf8_statement(db, Data, Size, &stmt1) == 0) {
            prepared = 1;
        }
    }

    if (!prepared || !stmt1) {
        sqlite3_close(db);
        return 0;
    }

    // Test sqlite3_stmt_readonly
    (void)sqlite3_stmt_readonly(stmt1);

    // Test sqlite3_reset
    (void)sqlite3_reset(stmt1);

    // Test sqlite3_clear_bindings
    (void)sqlite3_clear_bindings(stmt1);

    // Test sqlite3_stmt_explain with different modes (0, 1, 2)
    // We need to ensure the statement is reset before changing explain mode
    (void)sqlite3_reset(stmt1);
    int explain_modes[] = {0, 1, 2};
    for (int i = 0; i < 3; i++) {
        (void)sqlite3_stmt_explain(stmt1, explain_modes[i]);
        // If it fails, the statement might be invalid for that mode, try next
    }

    // Prepare a second statement to test sqlite3_transfer_bindings
    // We need a statement with the same number of parameters.
    // Let's try to prepare a statement from the tail of the input or a fixed query
    if (tail && tail != Data) {
        // Try to prepare from tail if available
        size_t tail_len = strlen((const char*)tail);
        if (tail_len > 0) {
            if (prepare_utf8_statement(db, (const uint8_t*)tail, tail_len, &stmt2) == 0) {
                // Try transfer bindings
                (void)sqlite3_transfer_bindings(stmt1, stmt2);
                sqlite3_finalize(stmt2);
                stmt2 = NULL;
            }
        }
    }

    // If we didn't get a second statement, try to prepare a simple one with a parameter
    if (!stmt2) {
        const char *simple_sql = "SELECT * FROM test_table WHERE id = ?;";
        if (sqlite3_prepare_v2(db, simple_sql, -1, &stmt2, NULL) == SQLITE_OK) {
            // Try transfer bindings (might fail if param counts differ, which is expected)
            (void)sqlite3_transfer_bindings(stmt1, stmt2);
            sqlite3_finalize(stmt2);
        }
    }

    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_close(db);

    return 0;
}