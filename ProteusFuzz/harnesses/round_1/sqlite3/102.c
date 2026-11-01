// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
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

/*
 * Static helper function to extract an integer from the fuzz input.
 * Returns a value within a reasonable range for the target APIs.
 */
static int extract_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        return 0;
    }
    int val = (data[*offset] << 24) | (data[*offset + 1] << 16) |
              (data[*offset + 2] << 8) | (data[*offset + 3]);
    *offset += 4;
    return val;
}

/*
 * Static helper function to extract a byte from the fuzz input.
 */
static unsigned char extract_byte(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return 0;
    }
    unsigned char val = data[*offset];
    *offset += 1;
    return val;
}

/*
 * Static busy handler callback for testing sqlite3_busy_handler.
 * Returns 1 to retry, or 0 to stop based on the counter.
 */
static int busy_handler_callback(void *arg, int count) {
    int limit = 0;
    if (arg) {
        limit = *(int *)arg;
    }
    /* Retry a few times then stop to avoid infinite loops during fuzzing */
    if (count < limit && count < 10) {
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have enough data to proceed with basic checks */
    if (Size < 4) {
        sqlite3_close_v2(db);
        return 0;
    }

    /* Extract parameters from fuzz input */
    int wal_frames = extract_int(Data, Size, &offset);
    int busy_timeout_ms = extract_int(Data, Size, &offset);
    int setlk_timeout_ms = extract_int(Data, Size, &offset);
    int setlk_flags = extract_int(Data, Size, &offset);
    int busy_limit = extract_int(Data, Size, &offset);

    /* 2. Invoke target functions diversely */

    /* Test sqlite3_wal_autocheckpoint */
    /* Try with positive, zero, and negative values */
    rc = sqlite3_wal_autocheckpoint(db, wal_frames);
    (void)rc; /* Ignore return for fuzzing stability */
    
    rc = sqlite3_wal_autocheckpoint(db, 0);
    rc = sqlite3_wal_autocheckpoint(db, -1);
    rc = sqlite3_wal_autocheckpoint(db, 1000);

    /* Test sqlite3_busy_handler */
    /* Register a custom handler */
    rc = sqlite3_busy_handler(db, busy_handler_callback, &busy_limit);
    (void)rc;

    /* Register NULL to disable */
    rc = sqlite3_busy_handler(db, NULL, NULL);
    (void)rc;

    /* Test sqlite3_busy_timeout */
    /* Try various timeout values */
    rc = sqlite3_busy_timeout(db, busy_timeout_ms);
    (void)rc;

    rc = sqlite3_busy_timeout(db, 0);
    rc = sqlite3_busy_timeout(db, -1);
    rc = sqlite3_busy_timeout(db, 5000);

    /* Test sqlite3_setlk_timeout */
    /* This may be a no-op depending on build flags, but safe to call */
    rc = sqlite3_setlk_timeout(db, setlk_timeout_ms, setlk_flags);
    (void)rc;

    rc = sqlite3_setlk_timeout(db, 0, 0);
    rc = sqlite3_setlk_timeout(db, -1, 0);
    rc = sqlite3_setlk_timeout(db, -5, 0); /* Invalid negative value */

    /* Test sqlite3_db_cacheflush */
    /* This requires a write transaction to have an effect, but calling it is safe */
    rc = sqlite3_db_cacheflush(db);
    (void)rc;

    /* Attempt to create a write transaction to trigger cache flush behavior */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Start a transaction to make db_cacheflush more interesting */
    rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_db_cacheflush(db);
        rc = sqlite3_exec(db, "INSERT INTO test_table VALUES (1);", NULL, NULL, NULL);
        rc = sqlite3_db_cacheflush(db);
        rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }

    /* 3. Cleanup */
    /* Close the database connection safely */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}