// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_stmt_readonly at sqlite3.c:94266:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:93531:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:147344:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:92300:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:94317:16 in sqlite3.h
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

static int create_dummy_db(sqlite3 **pdb) {
    const char *filename = "./dummy_file";
    int rc = sqlite3_open(filename, pdb);
    if (rc != SQLITE_OK) {
        return -1;
    }
    // Initialize with a simple table to ensure schema exists for testing
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);";
    char *err_msg = NULL;
    rc = sqlite3_exec(*pdb, init_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(*pdb);
        return -1;
    }
    return 0;
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
    // Remove dummy file if it exists
    unlink("./dummy_file");
}

static void cleanup_stmt(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;

    // 1. Prepare the environment
    if (create_dummy_db(&db) != 0) {
        return 0;
    }

    // Ensure we have enough data to create a minimal UTF-16 string
    // Minimum 2 bytes for a single char + null terminator in UTF-16 is 4 bytes usually,
    // but we will handle small sizes by padding or skipping.
    if (Size < 2) {
        cleanup_db(db);
        return 0;
    }

    // Convert input to UTF-16 for sqlite3_prepare16_v2
    // We interpret the input as raw bytes and try to form a valid UTF-16 sequence.
    // We allocate a buffer large enough to hold the input doubled (worst case 1 byte -> 2 bytes) + nulls.
    size_t utf16_size = Size * 2 + 4;
    uint16_t *utf16_buf = (uint16_t *)malloc(utf16_size);
    if (!utf16_buf) {
        cleanup_db(db);
        return 0;
    }

    // Simple conversion: treat input bytes as low-order bytes of UTF-16 chars
    // This is not valid text, but good for fuzzing the parser's robustness.
    for (size_t i = 0; i < Size; i++) {
        utf16_buf[i] = Data[i];
    }
    // Null terminate
    utf16_buf[Size] = 0;
    utf16_buf[Size + 1] = 0;

    // 2. Invoke target functions diversely

    // Test sqlite3_prepare16_v2
    rc = sqlite3_prepare16_v2(db, utf16_buf, Size * 2, &stmt, &pzTail);

    // Test sqlite3_expired on NULL
    int expired_null = sqlite3_expired(NULL);
    (void)expired_null;

    // Test sqlite3_stmt_busy on NULL
    int busy_null = sqlite3_stmt_busy(NULL);
    (void)busy_null;

    // Test sqlite3_data_count on NULL
    int data_count_null = sqlite3_data_count(NULL);
    (void)data_count_null;

    // Test sqlite3_stmt_readonly on NULL
    int readonly_null = sqlite3_stmt_readonly(NULL);
    (void)readonly_null;

    if (stmt != NULL) {
        // Test sqlite3_expired on valid stmt
        int expired = sqlite3_expired(stmt);
        (void)expired;

        // Test sqlite3_stmt_busy on valid stmt (before step)
        int busy = sqlite3_stmt_busy(stmt);
        (void)busy;

        // Test sqlite3_data_count on valid stmt (before step)
        int data_count = sqlite3_data_count(stmt);
        (void)data_count;

        // Test sqlite3_stmt_readonly on valid stmt
        int readonly = sqlite3_stmt_readonly(stmt);
        (void)readonly;

        // Try to step the statement to change state
        rc = sqlite3_step(stmt);

        // Test sqlite3_data_count after step
        int data_count_after = sqlite3_data_count(stmt);
        (void)data_count_after;

        // Test sqlite3_stmt_busy after step
        int busy_after = sqlite3_stmt_busy(stmt);
        (void)busy_after;

        // Try to reset the statement
        rc = sqlite3_reset(stmt);

        // Test sqlite3_expired after reset (should be false unless schema changed)
        int expired_after = sqlite3_expired(stmt);
        (void)expired_after;

        // Test sqlite3_stmt_busy after reset (should be false)
        int busy_after_reset = sqlite3_stmt_busy(stmt);
        (void)busy_after_reset;

        // Try to step again if previous step returned SQLITE_ROW
        if (rc == SQLITE_ROW) {
             rc = sqlite3_step(stmt);
        }
    }

    // 3. Cleanup
    cleanup_stmt(stmt);
    free(utf16_buf);
    cleanup_db(db);

    return 0;
}