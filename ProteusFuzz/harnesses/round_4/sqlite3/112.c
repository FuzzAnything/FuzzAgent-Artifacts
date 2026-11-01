// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16 at sqlite3.c:147332:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <wchar.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*
 * Static helper to create a dummy file for testing file-related errors.
 * Returns 0 on success, -1 on failure.
 */
static int create_dummy_file(const char *path, const uint8_t *data, size_t size) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0 && data != NULL) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

/*
 * Static helper to convert a portion of the fuzz input to UTF-16.
 * Returns a newly allocated buffer that must be freed by the caller.
 * Returns NULL if size is 0.
 */
static wchar_t *create_utf16_from_input(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    /* Ensure size is even for proper UTF-16 alignment */
    size_t len = (size % 2 == 0) ? size : size - 1;
    
    /* Allocate space for len/2 wchar_t characters plus one null terminator */
    size_t num_chars = len / 2;
    wchar_t *buf = (wchar_t *)malloc((num_chars + 1) * sizeof(wchar_t));
    if (!buf) return NULL;

    /* Simple conversion: treat bytes as characters, ignoring encoding specifics for fuzzing */
    for (size_t i = 0; i < num_chars; i++) {
        buf[i] = (wchar_t)(data[i*2] | (data[i*2+1] << 8));
    }
    /* Null terminate */
    buf[num_chars] = L'\0';
    return buf;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *tail = NULL;
    int rc;
    int sys_errno;
    int ext_errcode;
    int errcode;
    wchar_t *utf16_sql = NULL;
    wchar_t *utf16_filename = NULL;

    /* 
     * Strategy:
     * 1. Attempt to open a database (potentially failing to trigger errors).
     * 2. If open succeeds, prepare a statement using the input data.
     * 3. If prepare fails or succeeds, query error codes.
     * 4. Cleanup.
     */

    /* Use the first few bytes to determine flags or just use defaults for fuzzing */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0 && Data[0] & 0x01) {
        flags |= SQLITE_OPEN_EXCLUSIVE;
    }
    if (Size > 1 && Data[1] & 0x01) {
        flags |= SQLITE_OPEN_NOFOLLOW;
    }

    /* 
     * Test sqlite3_open_v2 
     * We use a dummy file path. If the input suggests a specific file name, we could use it,
     * but for safety and simplicity, we stick to a known path or memory.
     */
    const char *filename = "./dummy_file";
    
    /* Create a dummy file with fuzz data to ensure it exists or to corrupt it for error testing */
    create_dummy_file(filename, Data, Size);

    rc = sqlite3_open_v2(filename, &db, flags, NULL);

    if (rc == SQLITE_OK && db != NULL) {
        /* 
         * Database opened successfully.
         * Now try to prepare a statement using the input data as UTF-16 SQL.
         */
        utf16_sql = create_utf16_from_input(Data, Size);
        
        if (utf16_sql) {
            size_t len = wcslen(utf16_sql);
            rc = sqlite3_prepare16(db, utf16_sql, (int)(len * sizeof(wchar_t)), &stmt, &tail);
            
            /* 
             * Regardless of success or failure, query error codes.
             * If rc != SQLITE_OK, sqlite3_errcode should return the error.
             * If rc == SQLITE_OK, it should return SQLITE_OK (or 0).
             */
            errcode = sqlite3_errcode(db);
            ext_errcode = sqlite3_extended_errcode(db);
            
            /* 
             * If there was an error, check system errno.
             * Note: sqlite3_system_errno is most useful after I/O errors like SQLITE_CANTOPEN,
             * but we call it here to exercise the code path.
             */
            sys_errno = sqlite3_system_errno(db);

            /* Cleanup statement */
            if (stmt != NULL) {
                sqlite3_finalize(stmt);
            }
            free(utf16_sql);
        } else {
            /* Fallback if UTF-16 creation failed (e.g., empty input) */
            /* Just query error codes on the open db */
            errcode = sqlite3_errcode(db);
            ext_errcode = sqlite3_extended_errcode(db);
            sys_errno = sqlite3_system_errno(db);
        }
    } else {
        /* 
         * sqlite3_open_v2 failed.
         * This is a prime spot to test error retrieval functions.
         * db might be NULL or valid depending on the error.
         */
        if (db != NULL) {
            errcode = sqlite3_errcode(db);
            ext_errcode = sqlite3_extended_errcode(db);
            sys_errno = sqlite3_system_errno(db);
        } else {
            /* If db is NULL, these functions should return 0 or SQLITE_MISUSE depending on implementation details */
            errcode = sqlite3_errcode(NULL);
            ext_errcode = sqlite3_extended_errcode(NULL);
            sys_errno = sqlite3_system_errno(NULL);
        }
    }

    /* 
     * Test sqlite3_open16 with a UTF-16 filename derived from input.
     * We reuse the dummy file path but converted to UTF-16.
     */
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }

    /* Create a UTF-16 filename from the input or a static string if input is too short */
    if (Size >= 2) {
        utf16_filename = create_utf16_from_input(Data, Size);
    } else {
        /* Fallback to a static string if input is too small */
        utf16_filename = (wchar_t *)L"./dummy_file";
    }

    if (utf16_filename) {
        rc = sqlite3_open16(utf16_filename, &db);
        
        if (rc != SQLITE_OK && db != NULL) {
            /* Query errors after open16 failure */
            errcode = sqlite3_errcode(db);
            ext_errcode = sqlite3_extended_errcode(db);
            sys_errno = sqlite3_system_errno(db);
        }
        
        /* Cleanup open16 db */
        if (db != NULL) {
            sqlite3_close(db);
            db = NULL;
        }
        
        /* Free allocated filename if it was dynamically created */
        if (Size >= 2) {
            free(utf16_filename);
        }
    }

    /* 
     * Final cleanup if db was somehow left open (should not happen with logic above, but safe to have)
     */
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}