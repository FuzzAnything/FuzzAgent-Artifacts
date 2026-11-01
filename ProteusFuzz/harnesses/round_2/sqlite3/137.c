// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite filename manipulation APIs.
 *
 * This driver focuses on:
 * 1. sqlite3_create_filename (to generate valid sqlite3_filename pointers)
 * 2. sqlite3_filename_journal
 * 3. sqlite3_filename_wal
 * 4. sqlite3_filename_database
 * 5. sqlite3_uri_parameter
 * 6. sqlite3_free_filename
 */

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Step 1: Prepare the environment.
     * We need to construct a valid null-terminated string from the fuzz input
     * to pass to sqlite3_create_filename.
     */
    
    /* Ensure we have space for a null terminator */
    size_t safe_size = Size;
    if (safe_size > 4096) {
        safe_size = 4096; /* Limit size to prevent excessive allocation */
    }

    char *input_buf = (char *)malloc(safe_size + 1);
    if (!input_buf) {
        return 0;
    }

    memcpy(input_buf, Data, safe_size);
    input_buf[safe_size] = '\0';

    /* 
     * Step 2: Create a valid sqlite3_filename using sqlite3_create_filename.
     * Signature: sqlite3_create_filename(const char *zDatabase, const char *zJournal,
     *                                    const char *zWal, int nParam, const char **azParam)
     * We pass the input as the database name, and NULL for others to test basic paths.
     */
    sqlite3_filename zFilename = sqlite3_create_filename(input_buf, NULL, NULL, 0, NULL);
    
    /* 
     * If creation failed (e.g., out of memory), clean up and exit.
     * Note: sqlite3_create_filename returns NULL on failure.
     */
    if (zFilename == NULL) {
        free(input_buf);
        return 0;
    }

    /*
     * Step 3: Explore program states by invoking target functions.
     * We invoke the APIs on the valid filename pointer.
     */

    /* 1. sqlite3_filename_journal */
    const char *journal_name = sqlite3_filename_journal(zFilename);
    (void)journal_name; /* Suppress unused variable warning */

    /* 2. sqlite3_filename_wal */
    const char *wal_name = sqlite3_filename_wal(zFilename);
    (void)wal_name;

    /* 3. sqlite3_filename_database */
    const char *db_name = sqlite3_filename_database(zFilename);
    (void)db_name;

    /* 4. sqlite3_uri_parameter */
    /* We try a few common parameter names to exercise the parsing logic */
    const char *param_mode = sqlite3_uri_parameter(zFilename, "mode");
    const char *param_cache = sqlite3_uri_parameter(zFilename, "cache");
    const char *param_mutex = sqlite3_uri_parameter(zFilename, "mutex");
    const char *param_nolock = sqlite3_uri_parameter(zFilename, "nolock");
    const char *param_fuzz = sqlite3_uri_parameter(zFilename, "fuzz");
    
    /* Also try to extract parameters from the fuzz input itself if it looks like a param name */
    if (Size > 2 && Size < 20) {
        char param_buf[21];
        memcpy(param_buf, Data, Size);
        param_buf[Size] = '\0';
        (void)sqlite3_uri_parameter(zFilename, param_buf);
    }

    /*
     * Step 4: Cleanup.
     * Free the filename structure using the specific deallocation function.
     * Also free the manually allocated input buffer.
     */
    sqlite3_free_filename(zFilename);
    free(input_buf);

    return 0;
}