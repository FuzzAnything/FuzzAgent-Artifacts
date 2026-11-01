// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to safely open a database for fuzzing.
 * Uses a dummy file as per instructions.
 */
static sqlite3 *open_fuzz_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Static helper function to safely close a database.
 */
static void close_fuzz_db(sqlite3 *db) {
    if (db) {
        sqlite3_close_v2(db);
    }
}

/*
 * Static helper to create a prepared statement for testing sqlite3_db_handle.
 */
static sqlite3_stmt *create_fuzz_stmt(sqlite3 *db, const uint8_t *data, size_t size) {
    sqlite3_stmt *stmt = NULL;
    if (!db || !data || size == 0) return NULL;
    
    // Ensure the SQL string is null-terminated for safety
    // We create a temporary buffer to ensure null termination
    char *safe_sql = (char *)malloc(size + 1);
    if (!safe_sql) return NULL;
    
    memcpy(safe_sql, data, size);
    safe_sql[size] = '\0';
    
    int rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
    free(safe_sql);
    
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return NULL;
    }
    return stmt;
}

/*
 * WAL Hook callback function.
 */
static int fuzz_wal_hook(void *pArg, sqlite3 *db, const char *zDb, int nPage) {
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nPage;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int op_code;
    int timeout_ms;
    void *lookaside_buffer = NULL;
    size_t lookaside_size = 0;
    int lookaside_count = 0;

    // 1. Prepare the environment
    // Open a database connection using a dummy file
    db = open_fuzz_db("./dummy_file");
    if (!db) {
        return 0;
    }

    // Parse input data to drive the fuzzing logic
    // We use the first few bytes to determine operations and parameters
    
    // Determine operation code for sqlite3_db_config (0-255)
    op_code = Data[0];
    
    // Determine timeout for sqlite3_busy_timeout
    // Ensure we have at least 2 bytes before accessing Data[1]
    if (Size >= 2) {
        timeout_ms = (int)Data[1] * 10; // Scale up slightly
    } else {
        timeout_ms = 0;
    }
    
    // Determine lookaside parameters
    // We need to ensure lookaside_size is a multiple of sizeof(void*) and > 0
    // We use Data[2] and Data[3] to construct the size
    if (Size >= 4) {
        size_t raw_size = ((size_t)Data[2] << 8) | Data[3];
        if (raw_size == 0) raw_size = 1;
        
        // Align to sizeof(void*)
        size_t align = sizeof(void*);
        lookaside_size = (raw_size / align + 1) * align;
        
        // Limit size to avoid huge allocations (e.g., max 4KB for lookaside buffer)
        if (lookaside_size > 4096) lookaside_size = 4096;
    } else {
        lookaside_size = sizeof(void*); // Default safe size
    }
    
    if (Size >= 5) {
        lookaside_count = (int)Data[4]; // Non-negative check later
    } else {
        lookaside_count = 0;
    }

    // Allocate buffer for lookaside if needed
    if (op_code == SQLITE_DBCONFIG_LOOKASIDE) {
        lookaside_buffer = malloc(lookaside_size);
        if (!lookaside_buffer) {
            close_fuzz_db(db);
            return 0;
        }
    }

    // 2. Invoke target functions diversely

    // A. sqlite3_db_config
    // We try to configure the database connection.
    // Note: We must pass a valid pointer for the 3rd arg if op is LOOKASIDE.
    if (op_code == SQLITE_DBCONFIG_LOOKASIDE) {
        if (lookaside_buffer && lookaside_size > 0 && lookaside_count >= 0) {
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, lookaside_buffer, (int)lookaside_size, lookaside_count);
        } else {
            // Fallback to a safe call if parameters are invalid, or just skip
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, 0, 0);
        }
    } else {
        // Try other opcodes (most require specific args, we pass NULL/0 for safety/fuzzing)
        // We only test LOOKASIDE specifically as per the prompt's detailed requirement,
        // but we can try others with dummy args to trigger error paths.
        rc = sqlite3_db_config(db, op_code, NULL, 0, 0);
    }

    // B. sqlite3_errcode
    // Retrieve error code after the previous operation
    int err = sqlite3_errcode(db);
    (void)err; // Suppress unused warning

    // C. sqlite3_busy_timeout
    // Set a busy timeout
    rc = sqlite3_busy_timeout(db, timeout_ms);

    // D. sqlite3_wal_hook
    // Register a WAL hook
    // Ensure we have enough data for the user data pointer
    void *user_data = NULL;
    if (Size >= 6) {
        user_data = (void *)(uintptr_t)Data[5];
    }
    void *prev_hook = sqlite3_wal_hook(db, fuzz_wal_hook, user_data);
    (void)prev_hook;

    // E. sqlite3_db_handle
    // Try to get the db handle from a statement
    // We attempt to create a statement from the input data
    if (Size > 10) {
        stmt = create_fuzz_stmt(db, Data + 10, Size - 10);
        if (stmt) {
            sqlite3 *stmt_db = sqlite3_db_handle(stmt);
            (void)stmt_db;
            sqlite3_finalize(stmt);
        }
    }

    // 3. Cleanup
    // Free allocated memory
    if (lookaside_buffer) {
        free(lookaside_buffer);
    }

    // Close the database connection
    close_fuzz_db(db);

    return 0;
}