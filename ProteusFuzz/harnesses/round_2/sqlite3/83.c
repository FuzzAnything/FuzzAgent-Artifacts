// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * Static helper to create a dummy database file if it doesn't exist.
 * This ensures sqlite3_open_v2 can open a file for testing.
 */
static int ensure_dummy_file(void) {
    const char *filename = "./dummy_file";
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        return -1;
    }
    // Write some minimal data to ensure the file is not empty if that matters for the VFS
    const char *dummy = "SQLite format 3\x00";
    write(fd, dummy, strlen(dummy));
    close(fd);
    return 0;
}

/*
 * Static busy handler callback for fuzzing sqlite3_busy_handler.
 * Returns a value based on the count to potentially retry or stop.
 */
static int dummy_busy_handler(void *arg, int count) {
    // Use the count to decide whether to retry.
    // If count is even, retry (return 1), else stop (return 0).
    // This creates diverse execution paths.
    return (count % 2 == 0) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    // Ensure the dummy file exists
    if (ensure_dummy_file() != 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;

    // Open the database in memory or file mode. Using the dummy file.
    // SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE ensures we can write if needed.
    rc = sqlite3_open_v2("./dummy_file", &db, 
                          SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, 
                          NULL);
    
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Extract fuzzing parameters from the input data
    // We use modulo operations to keep values within reasonable ranges for the APIs
    int timeout_ms = (int)(Data[0] * 1000); // 0 to ~2.5 seconds
    int wal_frames = (int)(Data[1] * 1000); // 0 to ~2.5 million frames
    int flags = (int)(Data[2] % 4);         // 0 to 3

    // 1. Test sqlite3_busy_handler
    // Register a custom busy handler
    rc = sqlite3_busy_handler(db, dummy_busy_handler, (void*)(size_t)Data[3]);
    (void)rc; // Ignore return for fuzzing, just exercise the path

    // 2. Test sqlite3_busy_timeout
    // This will replace the busy handler set above
    rc = sqlite3_busy_timeout(db, timeout_ms);
    (void)rc;

    // 3. Test sqlite3_setlk_timeout
    // This sets the WAL lock timeout.
    // We try positive, zero, and negative values based on input.
    int setlk_val = (int)Data[4] * 100;
    if (Data[5] & 1) {
        setlk_val = -1; // Indefinite
    } else if (Data[5] & 2) {
        setlk_val = 0;  // Disable blocking
    } else if (Data[5] & 4) {
        setlk_val = -5; // Invalid negative (should return SQLITE_RANGE if enabled)
    }
    
    // Flags: SQLITE_SETLK_BLOCK_ON_CONNECT is 0x0001 (assuming standard definition)
    int setlk_flags = (flags & 1) ? 1 : 0;
    
    rc = sqlite3_setlk_timeout(db, setlk_val, setlk_flags);
    (void)rc;

    // 4. Test sqlite3_wal_autocheckpoint
    // Configure auto-checkpoint.
    // If wal_frames <= 0, it disables auto-checkpoint.
    rc = sqlite3_wal_autocheckpoint(db, wal_frames);
    (void)rc;

    // 5. Test sqlite3_db_cacheflush
    // This function requires a write transaction to be open to be effective.
    // We attempt to start a transaction, insert data, then flush.
    const char *sql_begin = "BEGIN;";
    const char *sql_insert = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB); INSERT OR IGNORE INTO test VALUES (1, 'fuzz');";
    
    rc = sqlite3_exec(db, sql_begin, NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);
        if (rc == SQLITE_OK) {
            // Now try to flush the cache
            rc = sqlite3_db_cacheflush(db);
            (void)rc;
        }
    }

    // Ensure transaction is committed or rolled back to avoid leaving db in bad state
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }

    // Cleanup
    sqlite3_close_v2(db);
    db = NULL;

    return 0;
}