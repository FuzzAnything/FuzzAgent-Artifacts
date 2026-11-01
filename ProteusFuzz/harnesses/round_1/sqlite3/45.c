// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to ensure VFS string lifetime */
static char *get_vfs_string(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return NULL;
    }
    /* Allocate on heap to ensure lifetime persists beyond stack scope */
    char *vfs_buf = malloc(Size + 1);
    if (!vfs_buf) return NULL;
    memcpy(vfs_buf, Data, Size);
    vfs_buf[Size] = '\0';
    return vfs_buf;
}

/* Progress callback implementation */
static int progress_callback(void *arg) {
    int *counter = (int *)arg;
    if (counter) {
        (*counter)++;
        /* Abort if counter exceeds a threshold to simulate cancellation */
        if (*counter > 100) {
            return 1;
        }
    }
    return 0;
}

/* Commit hook implementation */
static int commit_callback(void *arg) {
    int *counter = (int *)arg;
    if (counter) {
        (*counter)++;
        /* Rollback if counter exceeds a threshold */
        if (*counter > 50) {
            return 1;
        }
    }
    return 0;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    int flags = 0;
    const char *zVfs = NULL;
    
    /* Prepare environment: Create a dummy file if it doesn't exist to avoid I/O errors on open */
    FILE *f = fopen(filename, "w");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    /* Determine flags and VFS based on input data */
    if (Size > 0) {
        flags = Data[0] & 0xFF;
        /* Ensure flags are valid SQLite flags by masking common ones */
        flags = flags & (SQLITE_OPEN_READONLY | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI);
    }

    if (Size > 1) {
        /* Use part of the input as VFS name if available, ensuring heap allocation */
        size_t vfs_len = Size - 1;
        if (vfs_len > 255) vfs_len = 255;
        zVfs = get_vfs_string(Data + 1, vfs_len);
    }

    /* 1. Invoke sqlite3_open_v2 */
    rc = sqlite3_open_v2(filename, &db, flags, zVfs);

    /* Cleanup VFS buffer if allocated */
    if (zVfs) {
        free((void *)zVfs);
    }

    if (rc != SQLITE_OK) {
        /* 2. Handle error: Retrieve error message and code */
        if (db) {
            const char *err_msg = sqlite3_errmsg(db);
            int err_code = sqlite3_errcode(db);
            int ext_err_code = sqlite3_extended_errcode(db);
            (void)err_msg;
            (void)err_code;
            (void)ext_err_code;
            sqlite3_close(db);
            db = NULL;
        }
        return 0;
    }

    if (db) {
        /* 3. Explore program states with other APIs */
        
        /* Set progress handler */
        static int progress_counter = 0;
        progress_counter = 0;
        sqlite3_progress_handler(db, 100, progress_callback, &progress_counter);

        /* Set commit hook */
        static int commit_counter = 0;
        commit_counter = 0;
        sqlite3_commit_hook(db, commit_callback, &commit_counter);

        /* Trigger potential errors or state changes by running a simple query */
        sqlite3_exec(db, "SELECT 1;", NULL, NULL, NULL);
        
        /* Trigger potential error to test error retrieval functions */
        sqlite3_exec(db, "INVALID SQL STATEMENT;", NULL, NULL, NULL);

        /* 4. Retrieve error info after potential failure */
        const char *err_msg = sqlite3_errmsg(db);
        int err_code = sqlite3_errcode(db);
        int ext_err_code = sqlite3_extended_errcode(db);
        (void)err_msg;
        (void)err_code;
        (void)ext_err_code;

        /* Disable hooks */
        sqlite3_progress_handler(db, 0, NULL, NULL);
        sqlite3_commit_hook(db, NULL, NULL);

        /* Cleanup: Close database */
        sqlite3_close(db);
    }

    return 0;
}