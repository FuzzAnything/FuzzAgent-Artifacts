// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper to safely close a database connection.
 */
static void safe_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close_v2(db);
    }
}

/*
 * Helper to create a temporary file with fuzzed content if needed.
 * Returns 0 on success, -1 on failure.
 */
static int create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return -1;
    }
    if (size > 0) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int err_code;
    int ext_err_code;
    int sys_errno;

    /*
     * Strategy:
     * 1. Use the fuzzed data to construct filenames, flags, and VFS names.
     * 2. Try opening databases using sqlite3_open_v2 and sqlite3_open16.
     * 3. Query error codes using sqlite3_errcode, sqlite3_extended_errcode, and sqlite3_system_errno.
     * 4. Ensure proper cleanup with sqlite3_close_v2.
     */

    if (Size == 0) {
        return 0;
    }

    /*
     * Create a dummy file with the fuzzed input to test file-based operations.
     * This ensures the file exists for read-only or read-write tests.
     */
    create_dummy_file(Data, Size);

    /*
     * Extract parameters from the fuzzed data.
     * We use the first few bytes to determine flags and VFS selection.
     */
    int flags = (int)Data[0];
    
    /*
     * Prepare a VFS name buffer.
     * The instructions warn against stack-allocated buffers if the library might defer processing.
     * We allocate on the heap to be safe.
     */
    const char *zVfs = NULL;
    char *vfs_buf = NULL;
    
    if (Size > 1) {
        size_t vfs_len = Data[1] % 32; /* Limit VFS name length */
        if (vfs_len == 0) vfs_len = 1;
        
        /* Ensure we have enough data to copy */
        if (Size < 2 + vfs_len) {
            vfs_len = Size - 2;
            if (vfs_len <= 0) vfs_len = 0;
        }

        if (vfs_len > 0) {
            vfs_buf = malloc(vfs_len + 1);
            if (vfs_buf) {
                memcpy(vfs_buf, Data + 2, vfs_len);
                vfs_buf[vfs_len] = '\0';
                zVfs = vfs_buf;
            }
        }
    }

    /*
     * Test Case 1: sqlite3_open_v2
     * We try opening the dummy file with various flags.
     */
    rc = sqlite3_open_v2("./dummy_file", &db, flags, zVfs);
    
    if (db) {
        /*
         * If the database opened, try to query error codes.
         * Even if rc is SQLITE_OK, we can call these to exercise the logic.
         * If rc was an error, these will return the error details.
         */
        err_code = sqlite3_errcode(db);
        ext_err_code = sqlite3_extended_errcode(db);
        sys_errno = sqlite3_system_errno(db);
        
        /*
         * Close the database safely.
         */
        safe_close_db(db);
        db = NULL;
    } else {
        /*
         * If open failed, we cannot query error codes on the handle.
         */
        (void)err_code; /* Suppress unused warning if logic changes */
    }

    /*
     * Test Case 2: sqlite3_open16 (UTF-16 filename)
     * We construct a UTF-16 string from the fuzzed data.
     * We need to ensure the string is null-terminated.
     */
    if (Size > 2) {
        size_t utf16_len = (Size - 2) / 2;
        if (utf16_len > 100) utf16_len = 100; /* Limit size to prevent OOM */
        
        wchar_t *utf16_filename = malloc((utf16_len + 1) * sizeof(wchar_t));
        if (utf16_filename) {
            for (size_t i = 0; i < utf16_len; i++) {
                /* Simple conversion: take two bytes as one char */
                utf16_filename[i] = (wchar_t)(Data[2 + i*2]);
            }
            utf16_filename[utf16_len] = L'\0';

            /*
             * Attempt to open with UTF-16 filename.
             * Note: The filename might be invalid, which is expected for fuzzing.
             */
            rc = sqlite3_open16((const void *)utf16_filename, &db);
            
            if (db) {
                err_code = sqlite3_errcode(db);
                ext_err_code = sqlite3_extended_errcode(db);
                sys_errno = sqlite3_system_errno(db);
                safe_close_db(db);
                db = NULL;
            }
            
            free(utf16_filename);
        }
    }

    /*
     * Test Case 3: Re-open with different flags to explore more states.
     * Try opening as read-only or with no-create if the file exists.
     */
    int flags2 = (int)Data[0] | SQLITE_OPEN_READONLY;
    rc = sqlite3_open_v2("./dummy_file", &db, flags2, zVfs);
    
    if (db) {
        err_code = sqlite3_errcode(db);
        ext_err_code = sqlite3_extended_errcode(db);
        sys_errno = sqlite3_system_errno(db);
        safe_close_db(db);
        db = NULL;
    }

    /*
     * Cleanup allocated VFS buffer.
     */
    if (vfs_buf) {
        free(vfs_buf);
    }

    /*
     * Clean up the dummy file.
     */
    remove("./dummy_file");

    return 0;
}