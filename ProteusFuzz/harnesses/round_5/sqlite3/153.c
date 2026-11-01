// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_open16 at sqlite3.c:189330:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <wchar.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_system_errno
 * - sqlite3_extended_errcode
 * - sqlite3_open_v2
 * - sqlite3_close_v2
 * - sqlite3_errcode
 * - sqlite3_open16
 */

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int sys_errno;
    int ext_errcode;
    int errcode;

    if (Size < 1) {
        /* Test with NULL handle immediately */
        sys_errno = sqlite3_system_errno(NULL);
        (void)sys_errno;
        ext_errcode = sqlite3_extended_errcode(NULL);
        (void)ext_errcode;
        errcode = sqlite3_errcode(NULL);
        (void)errcode;
        return 0;
    }

    /*
     * Prepare a safe filename from input data.
     * We ensure it's null-terminated and limit length.
     */
    size_t max_len = Size < 255 ? Size : 255;
    char filename[256];
    memcpy(filename, Data, max_len);
    filename[max_len] = '\0';

    /* Ensure filename doesn't contain dangerous paths for the fuzzer */
    for (size_t i = 0; i < max_len; i++) {
        if (filename[i] == '/' || filename[i] == '\\') {
            filename[i] = '_';
        }
    }

    /*
     * Case 1: Open with sqlite3_open_v2 using various flags derived from input
     */
    int flags = 0;
    if (Size > 0) {
        /* Use first byte to determine flags */
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_READWRITE;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_CREATE;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_URI; /* Replaced SQLITE_OPEN_NO_MUTEX */
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x20) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x40) flags |= SQLITE_OPEN_SHAREDCACHE;
        if (flag_byte & 0x80) flags |= SQLITE_OPEN_PRIVATECACHE;
    }

    rc = sqlite3_open_v2(filename, &db, flags, NULL);

    if (rc != SQLITE_OK) {
        /*
         * If open failed, try to retrieve error codes.
         */
        if (db != NULL) {
            sys_errno = sqlite3_system_errno(db);
            (void)sys_errno;
            ext_errcode = sqlite3_extended_errcode(db);
            (void)ext_errcode;
            errcode = sqlite3_errcode(db);
            (void)errcode;
        } else {
            /* db is NULL, these should return 0 or SQLITE_MISUSE */
            sys_errno = sqlite3_system_errno(NULL);
            (void)sys_errno;
            ext_errcode = sqlite3_extended_errcode(NULL);
            (void)ext_errcode;
            errcode = sqlite3_errcode(NULL);
            (void)errcode;
        }
    } else {
        /*
         * Database opened successfully.
         * Perform some operations to potentially trigger errors, then check error codes.
         */
        
        /* Try to execute an invalid SQL to trigger an error */
        sqlite3_stmt *stmt = NULL;
        const char *sql = "SELECT * FROM non_existent_table_xyz";
        
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            /* Error occurred, check error codes */
            sys_errno = sqlite3_system_errno(db);
            (void)sys_errno;
            ext_errcode = sqlite3_extended_errcode(db);
            (void)ext_errcode;
            errcode = sqlite3_errcode(db);
            (void)errcode;
        }
        if (stmt) {
            sqlite3_finalize(stmt);
        }

        /* Try another operation that might fail */
        rc = sqlite3_exec(db, "DROP TABLE IF EXISTS fake_table", NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            sys_errno = sqlite3_system_errno(db);
            (void)sys_errno;
            ext_errcode = sqlite3_extended_errcode(db);
            (void)ext_errcode;
            errcode = sqlite3_errcode(db);
            (void)errcode;
        }
    }

    /*
     * Case 2: Test sqlite3_open16 with UTF-16 filename
     * We construct a simple UTF-16 string from the input data.
     */
    sqlite3 *db16 = NULL;
    if (Size > 0) {
        /* Create a wide char string */
        size_t wlen = (Size < 127) ? Size : 127;
        wchar_t *wfilename = (wchar_t *)malloc((wlen + 1) * sizeof(wchar_t));
        if (wfilename) {
            for (size_t i = 0; i < wlen; i++) {
                /* Simple conversion, assuming ASCII for simplicity in fuzzing */
                wfilename[i] = (wchar_t)Data[i];
            }
            wfilename[wlen] = L'\0';

            rc = sqlite3_open16(wfilename, &db16);
            
            if (rc != SQLITE_OK) {
                if (db16 != NULL) {
                    sys_errno = sqlite3_system_errno(db16);
                    (void)sys_errno;
                    ext_errcode = sqlite3_extended_errcode(db16);
                    (void)ext_errcode;
                    errcode = sqlite3_errcode(db16);
                    (void)errcode;
                }
            } else {
                /* Successfully opened, try to trigger an error */
                sqlite3_stmt *stmt = NULL;
                rc = sqlite3_prepare_v2(db16, "SELECT * FROM invalid_table", -1, &stmt, NULL);
                if (rc != SQLITE_OK) {
                    sys_errno = sqlite3_system_errno(db16);
                    (void)sys_errno;
                    ext_errcode = sqlite3_extended_errcode(db16);
                    (void)ext_errcode;
                    errcode = sqlite3_errcode(db16);
                    (void)errcode;
                }
                if (stmt) sqlite3_finalize(stmt);
            }
            
            free(wfilename);
        }
    }

    /*
     * Cleanup
     * Use sqlite3_close_v2 to safely close connections.
     */
    if (db) {
        sqlite3_close_v2(db);
    }
    if (db16) {
        sqlite3_close_v2(db16);
    }

    return 0;
}