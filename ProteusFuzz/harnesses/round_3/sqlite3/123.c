// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * Static helper function to create a valid database file for fuzzing.
 * We use "./dummy_file" as requested.
 */
static int create_dummy_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        return -1;
    }
    
    // Create a simple table and insert some data to ensure the database is valid
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    // Insert a row to ensure there is data to flush
    sql = "INSERT INTO test (data) VALUES (X'0102030405');";
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_close(db);
    return 0;
}

/*
 * Static busy handler callback for fuzzing sqlite3_busy_handler.
 * Returns a value based on the iteration count to explore different states.
 */
static int busy_callback(void *arg, int count) {
    // Use the argument to decide whether to retry or stop
    int *pRetry = (int *)arg;
    if (pRetry && *pRetry > count) {
        return 1; // Retry
    }
    return 0; // Stop
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data to extract parameters
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from the fuzz input
    int timeout_ms = (int)(Data[0] << 8 | Data[1]);
    int wal_checkpoint = (int)(Data[2] << 8 | Data[3]);
    int setlk_timeout = (int)(Data[0] | Data[1]);
    int busy_retry_count = (int)(Data[2]);
    
    // Ensure we have a valid database file
    const char *db_filename = "./dummy_file";
    if (create_dummy_db(db_filename) != 0) {
        // If we can't create the DB, try to remove it and retry once
        unlink(db_filename);
        if (create_dummy_db(db_filename) != 0) {
            return 0;
        }
    }

    sqlite3 *db = NULL;
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // 1. Setup: Configure busy handlers and timeouts
    // Fuzz sqlite3_busy_timeout
    rc = sqlite3_busy_timeout(db, timeout_ms);
    
    // Fuzz sqlite3_busy_handler with a custom callback
    int retry_arg = busy_retry_count;
    rc = sqlite3_busy_handler(db, busy_callback, &retry_arg);

    // Fuzz sqlite3_setlk_timeout (may be a no-op depending on build)
    // Use negative values to test error handling (SQLITE_RANGE)
    int setlk_val = setlk_timeout;
    if (setlk_val < -1) setlk_val = -1; // Clamp to -1 to avoid undefined behavior
    rc = sqlite3_setlk_timeout(db, setlk_val, 0);

    // 2. Setup: Configure WAL auto-checkpoint
    // Fuzz sqlite3_wal_autocheckpoint
    rc = sqlite3_wal_autocheckpoint(db, wal_checkpoint);

    // 3. Execute Target: sqlite3_db_cacheflush
    // To trigger meaningful behavior, we need a write transaction.
    // We start a transaction, make a change, then try to flush.
    
    // Begin a transaction
    rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    
    // Make a small change to ensure there are dirty pages
    rc = sqlite3_exec(db, "INSERT INTO test (data) VALUES (X'FF');", NULL, NULL, NULL);
    
    // Now call the target function
    // This should attempt to flush dirty pages to disk
    rc = sqlite3_db_cacheflush(db);
    
    // Commit the transaction to clean up
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    // 4. Cleanup: Close the database connection safely
    // Use sqlite3_close_v2 to handle potential pending operations gracefully
    rc = sqlite3_close_v2(db);
    db = NULL;

    return 0;
}