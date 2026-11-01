// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int create_dummy_file(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    const char *content = "SELECT 1;";
    write(fd, content, strlen(content));
    close(fd);
    return 0;
}

static int remove_dummy_file(const char *filename) {
    return unlink(filename);
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    int rc;

    // Ensure dummy file exists for file-based operations
    if (create_dummy_file(filename) != 0) {
        filename = ":memory:";
    }

    // 1. Test sqlite3_open_v2
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc == SQLITE_OK && db != NULL) {
        // 2. Test sqlite3_errcode and sqlite3_extended_errcode on a fresh connection
        (void)sqlite3_errcode(db);
        (void)sqlite3_extended_errcode(db);
        (void)sqlite3_system_errno(db);

        // 3. Test sqlite3_prepare16 with fuzzed input
        // The input Data is raw bytes. sqlite3_prepare16 expects UTF-16.
        // If Size is odd, the last byte is incomplete for UTF-16, which can cause
        // issues inside SQLite if it tries to read past the buffer assuming 2-byte alignment.
        // We ensure the length passed is even to avoid potential out-of-bounds reads
        // inside the UTF-16 processing logic of SQLite.
        size_t safeSize = Size;
        if (safeSize % 2 != 0) {
            safeSize--;
        }

        if (safeSize > 0) {
            rc = sqlite3_prepare16(db, Data, (int)safeSize, &stmt, &pzTail);
            if (rc != SQLITE_OK) {
                // 4. Test error retrieval functions after a failure
                (void)sqlite3_errcode(db);
                (void)sqlite3_extended_errcode(db);
                (void)sqlite3_system_errno(db);
            } else {
                // If prepared successfully, finalize it
                if (stmt != NULL) {
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }
            }
        }

        // 5. Try to induce an error to test error codes more thoroughly
        // Execute a malformed SQL via prepare to trigger syntax errors
        const char *bad_sql = "SELECT * FROM nonexistent_table_xyz;";
        rc = sqlite3_prepare_v2(db, bad_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            (void)sqlite3_errcode(db);
            (void)sqlite3_extended_errcode(db);
            (void)sqlite3_system_errno(db);
        } else {
            if (stmt != NULL) sqlite3_finalize(stmt);
        }

        // 6. Test sqlite3_open16 with fuzzed data as filename (if it looks like a path)
        // We reuse the input data as a UTF-16 filename.
        // This might fail, which is expected and good for fuzzing.
        sqlite3 *db16 = NULL;
        // Ensure even length for UTF-16 filename as well
        size_t safeSize16 = Size;
        if (safeSize16 % 2 != 0) {
            safeSize16--;
        }
        
        if (safeSize16 > 2) {
            rc = sqlite3_open16(Data, &db16);
            if (rc == SQLITE_OK && db16 != NULL) {
                // Test error functions on the new handle
                (void)sqlite3_errcode(db16);
                (void)sqlite3_extended_errcode(db16);
                (void)sqlite3_system_errno(db16);
                sqlite3_close(db16);
            }
        }

        // 7. Close the main database
        sqlite3_close(db);
        db = NULL;
    }

    // 8. Test with NULL pointer for error functions
    (void)sqlite3_errcode(NULL);
    (void)sqlite3_extended_errcode(NULL);
    (void)sqlite3_system_errno(NULL);

    // Cleanup dummy file if it was created
    if (filename != ":memory:") {
        remove_dummy_file(filename);
    }

    return 0;
}