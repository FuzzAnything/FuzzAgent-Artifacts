// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 filename and URI handling functions.
 *
 * Target functions:
 * - sqlite3_filename_journal
 * - sqlite3_uri_parameter
 * - sqlite3_filename_wal
 * - sqlite3_filename_database
 * - sqlite3_free_filename
 *
 * Note: sqlite3_database_file_object has been removed as it is an internal
 * function relying on specific memory layouts not present in sqlite3_filename
 * structures, leading to crashes when called with public API objects.
 */

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a mutable copy of the input to ensure null termination */
    char *input = (char *)malloc(Size + 1);
    if (!input) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    /* 
     * Limit the length to avoid excessive allocations.
     * SQLite filenames are typically short; 4096 bytes is more than enough.
     */
    if (Size > 4096) {
        input[4095] = '\0';
    }

    /* 
     * Ensure the input is not empty before passing to sqlite3_create_filename.
     */
    if (input[0] == '\0') {
        free(input);
        return 0;
    }

    /* 
     * Construct a filename using sqlite3_create_filename().
     * Pass empty strings "" instead of NULL for zJournal and zWal
     * to prevent internal strlen() calls from crashing on NULL pointers.
     */
    sqlite3_filename *fn = sqlite3_create_filename(input, "", "", 0, NULL);
    
    if (fn) {
        /* 
         * 1. Test sqlite3_filename_journal
         */
        const char *journal = sqlite3_filename_journal(fn);
        (void)journal; 

        /* 
         * 2. Test sqlite3_filename_wal
         */
        const char *wal = sqlite3_filename_wal(fn);
        (void)wal;

        /* 
         * 3. Test sqlite3_filename_database
         */
        const char *db = sqlite3_filename_database(fn);
        (void)db;

        /* 
         * 4. Test sqlite3_uri_parameter with various parameter names
         */
        const char *params[] = {
            "mode", "immutable", "cache", "locking", 
            "journal_size_limit", "temp_store", "uri"
        };
        int num_params = sizeof(params) / sizeof(params[0]);
        
        for (int i = 0; i < num_params; i++) {
            const char *val = sqlite3_uri_parameter(fn, params[i]);
            (void)val;
        }

        /* 
         * Also try to extract parameter names from the input string itself if possible.
         * We look for '?' to indicate a query string.
         */
        char *query_start = strchr(input, '?');
        if (query_start) {
            /* Create a copy for strtok_r as it modifies the string */
            char *query_copy = strdup(query_start + 1);
            if (query_copy) {
                char *saveptr;
                char *token = strtok_r(query_copy, "&", &saveptr);
                while (token != NULL) {
                    char *eq = strchr(token, '=');
                    if (eq) {
                        *eq = '\0';
                        /* token is now the parameter name */
                        const char *val = sqlite3_uri_parameter(fn, token);
                        (void)val;
                    }
                    token = strtok_r(NULL, "&", &saveptr);
                }
                free(query_copy);
            }
        }

        /* 
         * REMOVED: sqlite3_database_file_object call.
         * This function is internal and expects a specific memory layout 
         * associated with open database handles, not sqlite3_filename objects.
         * Calling it here causes undefined behavior and segmentation faults.
         */

        /* 
         * 5. Cleanup: Free the filename structure
         */
        sqlite3_free_filename(fn);
    }

    /* Free the input buffer */
    free(input);

    return 0;
}