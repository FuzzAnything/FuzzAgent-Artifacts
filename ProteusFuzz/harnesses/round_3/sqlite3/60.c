// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
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

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

static void remove_dummy_file(const char *filename) {
    unlink(filename);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    const char *utf16_filename = NULL;
    
    // Ensure we have some data to work with for file content
    if (Size > 0) {
        write_dummy_file(filename, Data, Size);
    } else {
        // Write a minimal dummy file if input is empty
        const char *dummy = "dummy";
        write_dummy_file(filename, (const uint8_t *)dummy, strlen(dummy));
    }

    // Test 1: sqlite3_open_v2
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        // If open fails, we can still test error reporting on NULL or invalid handles if needed,
        // but usually we want a valid db to test error codes.
        // However, sqlite3_errcode on NULL returns SQLITE_MISUSE.
        (void)sqlite3_errcode(NULL);
        (void)sqlite3_extended_errcode(NULL);
        (void)sqlite3_system_errno(NULL);
        remove_dummy_file(filename);
        return 0;
    }

    // Test 2: sqlite3_errcode (should be SQLITE_OK initially if no error)
    (void)sqlite3_errcode(db);

    // Test 3: sqlite3_extended_errcode
    (void)sqlite3_extended_errcode(db);

    // Test 4: sqlite3_system_errno
    (void)sqlite3_system_errno(db);

    // Try to trigger an error to test error reporting functions
    // Execute a malformed SQL to generate an error
    const char *bad_sql = "SELECT * FROM non_existent_table WHERE 1=1;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, bad_sql, -1, &stmt, NULL);
    
    // If prepare failed, check error codes
    if (rc != SQLITE_OK) {
        int err = sqlite3_errcode(db);
        int ext_err = sqlite3_extended_errcode(db);
        int sys_err = sqlite3_system_errno(db);
        const char *msg = sqlite3_errmsg(db);
        
        // Just consume the values to avoid unused warnings
        (void)err;
        (void)ext_err;
        (void)sys_err;
        (void)msg;
    } else {
        sqlite3_finalize(stmt);
    }

    // Test 5: sqlite3_close_v2
    rc = sqlite3_close_v2(db);
    (void)rc;

    // Test 6: sqlite3_open16 (UTF-16 filename)
    // Convert filename to UTF-16
    if (Size > 0) {
        // We can try to use the input data as a potential UTF-16 string if it looks like it,
        // but for safety, let's just construct a simple UTF-16 string for the filename.
        // "dummy_file" in UTF-16LE (assuming little endian or just testing the API)
        // We'll just pass a pointer to the input data if it's large enough and looks safe,
        // or construct a static one.
        // To be safe and deterministic, let's create a small UTF-16 buffer.
        wchar_t w_filename[] = L"./dummy_file";
        rc = sqlite3_open16(w_filename, &db);
        if (rc == SQLITE_OK) {
            // Perform some operations to potentially trigger errors
            const char *sql = "CREATE TABLE test (id INT);";
            sqlite3_exec(db, sql, NULL, NULL, NULL);
            
            // Check error codes again
            (void)sqlite3_errcode(db);
            (void)sqlite3_extended_errcode(db);
            (void)sqlite3_system_errno(db);
            
            sqlite3_close_v2(db);
        }
    }

    // Cleanup
    remove_dummy_file(filename);

    return 0;
}