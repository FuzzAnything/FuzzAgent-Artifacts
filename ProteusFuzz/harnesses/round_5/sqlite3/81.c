// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_commit_hook at sqlite3.c:187962:18 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to safely read an integer from fuzz input.
 * Returns a default value if the buffer is too small.
 */
static int read_int(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) return 0;
    int val = (int)Data[*offset] | (int)Data[*offset + 1] << 8 |
              (int)Data[*offset + 2] << 16 | (int)Data[*offset + 3] << 24;
    *offset += 4;
    return val;
}

/*
 * Static helper function to safely read a string from fuzz input.
 * Returns a pointer to a static buffer containing the null-terminated string.
 */
static const char *read_string(const uint8_t *Data, size_t Size, size_t *offset) {
    static char buf[256];
    int len = 0;
    if (*offset >= Size) return "";
    len = Data[*offset];
    (*offset)++;
    if (len > 255 || *offset + len > Size) len = 0;
    memcpy(buf, Data + *offset, len);
    buf[len] = '\0';
    *offset += len;
    return buf;
}

/*
 * Progress callback for sqlite3_progress_handler.
 * Returns 0 to continue, 1 to abort.
 */
static int progress_callback(void *arg) {
    if (arg) {
        int *val = (int *)arg;
        if (*val > 0) return 1;
    }
    return 0;
}

/*
 * Commit hook callback for sqlite3_commit_hook.
 * Returns 0 to commit, 1 to rollback.
 */
static int commit_callback(void *arg) {
    if (arg) {
        int *val = (int *)arg;
        if (*val > 0) return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;

    /*
     * Step 1: Prepare environment.
     * We need a valid database connection to test most functions.
     * We use a temporary file or ":memory:" based on input.
     */
    const char *filename = ":memory:";
    if (Size > 0 && Data[0] == 1) {
        filename = "./dummy_file";
        /* Ensure dummy_file exists or can be created */
        FILE *f = fopen(filename, "w");
        if (f) {
            fprintf(f, "dummy content for fuzzing\n");
            fclose(f);
        }
    }

    /* Open database with random flags derived from input */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 1) {
        if (Data[1] & 0x01) flags |= SQLITE_OPEN_READONLY;
        /* SQLITE_OPEN_NO_MUTEX is not a standard flag in all versions, removed to fix compile error */
        if (Data[1] & 0x04) flags |= SQLITE_OPEN_URI;
    }

    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        /* If open fails, we can still test error reporting on NULL or invalid handles if needed,
           but most APIs require a valid handle. We proceed to test error functions with NULL. */
        sqlite3_errmsg(NULL);
        sqlite3_errcode(NULL);
        sqlite3_extended_errcode(NULL);
        return 0;
    }

    /*
     * Step 2 & 3: Explore program states by invoking target functions diversely.
     */

    /* Test sqlite3_progress_handler */
    int progress_n = read_int(Data, Size, &offset);
    void *progress_arg = NULL;
    if (Size > offset + 4) {
        int arg_val = read_int(Data, Size, &offset);
        progress_arg = &arg_val;
    }
    
    /* Enable progress handler */
    sqlite3_progress_handler(db, progress_n, progress_callback, progress_arg);
    
    /* Disable progress handler (N < 1 or NULL callback) */
    sqlite3_progress_handler(db, -1, NULL, NULL);
    sqlite3_progress_handler(db, 0, NULL, NULL);
    
    /* Re-enable with different N */
    int n2 = (Size > 0) ? (int)Data[0] : 100;
    if (n2 <= 0) n2 = 100;
    sqlite3_progress_handler(db, n2, progress_callback, progress_arg);

    /* Test sqlite3_commit_hook */
    int commit_n = read_int(Data, Size, &offset);
    void *commit_arg = NULL;
    if (Size > offset + 4) {
        int arg_val = read_int(Data, Size, &offset);
        commit_arg = &arg_val;
    }

    /* Register commit hook */
    void *prev_commit = sqlite3_commit_hook(db, commit_callback, commit_arg);
    (void)prev_commit; /* Suppress unused warning */

    /* Disable commit hook */
    sqlite3_commit_hook(db, NULL, NULL);

    /* Re-register */
    sqlite3_commit_hook(db, commit_callback, commit_arg);

    /* Test sqlite3_errmsg, sqlite3_errcode, sqlite3_extended_errcode */
    /* First, try to trigger an error by running invalid SQL if possible, 
       but since we don't have prepare/step in the target list, we rely on 
       the state after open or hook registration. 
       We can also just call them directly to see current state. */
    const char *msg = sqlite3_errmsg(db);
    (void)msg;
    
    int code = sqlite3_errcode(db);
    (void)code;
    
    int ext_code = sqlite3_extended_errcode(db);
    (void)ext_code;

    /* Try to trigger an error by passing invalid arguments if possible, 
       but these specific functions are safe. 
       Let's try to open a bad path to trigger an error for testing. */
    const char *bad_path = read_string(Data, Size, &offset);
    sqlite3 *bad_db = NULL;
    rc = sqlite3_open_v2(bad_path, &bad_db, SQLITE_OPEN_READONLY, NULL);
    if (bad_db) {
        msg = sqlite3_errmsg(bad_db);
        code = sqlite3_errcode(bad_db);
        ext_code = sqlite3_extended_errcode(bad_db);
        sqlite3_close(bad_db);
    }

    /*
     * Step 2: Cleanup.
     */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}