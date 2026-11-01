// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:187902:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
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

/*
 * Static helper to create a dummy file with fuzzing data if needed.
 * This ensures the file exists and contains the fuzzed input for testing
 * file-based operations.
 */
static int create_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (Size > 0) {
        ssize_t written = write(fd, Data, Size);
        if (written < 0) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

/*
 * Trace callback function for sqlite3_trace_v2.
 * This function is invoked by SQLite when tracing is enabled.
 * It safely handles the event and context pointers.
 */
static int trace_callback(unsigned event, void *pCtx, void *pArg1, void *pArg2) {
    (void)event;
    (void)pCtx;
    (void)pArg1;
    (void)pArg2;
    return 0;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int cur, hiwtr;

    /*
     * Step 1: Prepare the environment.
     * Create a dummy file with the fuzzing input to test file-based operations.
     * We use a subset of the input or the whole input as the filename content
     * to explore different file states.
     */
    if (Size > 0) {
        create_dummy_file(Data, Size);
    }

    /*
     * Step 2: Open the database.
     * We try opening the dummy file. If Size is 0, we might open an in-memory DB
     * or an empty string (temporary file).
     */
    const char *filename;
    if (Size == 0) {
        filename = ":memory:";
    } else if (Size < 256) {
        /* Use the data as a filename if it's short and safe */
        /* Note: We ensure null-termination for safety in a real scenario,
         * but for fuzzing, we might pass raw data to test robustness.
         * However, sqlite3_open expects a valid C string.
         * We'll use the dummy file path instead to avoid invalid string issues
         * unless we construct a valid string from Data.
         */
        filename = "./dummy_file";
    } else {
        filename = "./dummy_file";
    }

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK && rc != SQLITE_BUSY) {
        /* If opening fails but db is not NULL, we still have a handle to test.
         * If db is NULL, we cannot proceed with other tests.
         */
        if (db == NULL) {
            return 0;
        }
    }

    /*
     * Step 3: Explore program states.
     * Invoke target API functions diversely.
     */

    /* Test sqlite3_trace_v2 with various masks and callbacks */
    if (db != NULL) {
        /* Enable tracing with a specific mask */
        rc = sqlite3_trace_v2(db, SQLITE_TRACE_STMT, trace_callback, (void *)Data);
        (void)rc;

        /* Disable tracing */
        rc = sqlite3_trace_v2(db, 0, NULL, NULL);
        (void)rc;

        /* Test with different masks if available in the input */
        if (Size > 0) {
            unsigned mask = Data[0] & 0xFF;
            rc = sqlite3_trace_v2(db, mask, trace_callback, (void *)(Data + 1));
            (void)rc;
        }

        /* Test sqlite3_db_status */
        rc = sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiwtr, 0);
        (void)rc;
        rc = sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiwtr, 1); /* Reset flag */
        (void)rc;

        /* Test sqlite3_total_changes */
        int total_changes = sqlite3_total_changes(db);
        (void)total_changes;

        /* Test sqlite3_changes */
        int changes = sqlite3_changes(db);
        (void)changes;

        /*
         * Attempt to execute a simple SQL statement to trigger changes
         * if the database is writable, to exercise the change counters.
         */
        sqlite3_stmt *stmt = NULL;
        const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        /* Re-check changes after potential write */
        total_changes = sqlite3_total_changes(db);
        changes = sqlite3_changes(db);
        (void)total_changes;
        (void)changes;
    }

    /*
     * Step 4: Cleanup.
     * Close the database connection safely using sqlite3_close_v2.
     */
    if (db != NULL) {
        rc = sqlite3_close_v2(db);
        (void)rc;
    }

    /* Clean up the dummy file */
    unlink("./dummy_file");

    return 0;
}