// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 filename manipulation APIs.
 * Target APIs:
 * - sqlite3_filename_journal
 * - sqlite3_uri_parameter
 * - sqlite3_filename_wal
 * - sqlite3_free_filename
 * - sqlite3_filename_database
 * - sqlite3_database_file_object
 * - sqlite3_create_filename (used to construct valid inputs)
 */

static void fuzz_filename_constructors(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    /* Ensure null-termination for string parsing */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /*
     * Construct valid filenames using sqlite3_create_filename.
     * Signature: sqlite3_create_filename(const char *zDatabase, const char *zJournal,
     *                                    const char *zWal, int nParam, const char **azParam)
     * We pass the input string as the database name and NULL for others to test
     * the parsing of the database name and any URI parameters it might contain.
     * 
     * CRITICAL FIX: The original code passed NULL for zJournal and zWal.
     * The internal implementation of sqlite3_create_filename calls strlen on these arguments.
     * Passing NULL causes a segmentation fault. We must pass empty strings "" instead.
     */
    sqlite3_filename name = sqlite3_create_filename(input_str, "", "", 0, NULL);

    if (name) {
        /*
         * Test sqlite3_filename_database
         * Translates journal/WAL names to the main database file.
         */
        const char *db_name = sqlite3_filename_database(name);
        (void)db_name;

        /*
         * Test sqlite3_filename_journal
         * Retrieves the journal filename.
         */
        const char *journal_name = sqlite3_filename_journal(name);
        (void)journal_name;

        /*
         * Test sqlite3_filename_wal
         * Retrieves the WAL filename.
         */
        const char *wal_name = sqlite3_filename_wal(name);
        (void)wal_name;

        /*
         * Test sqlite3_uri_parameter
         * Extract various parameters from the URI.
         */
        const char *params[] = {"mode", "immutable", "cache", "temp_store", "uri", "journal_size_limit", NULL};
        for (int i = 0; params[i]; i++) {
            const char *val = sqlite3_uri_parameter(name, params[i]);
            (void)val;
        }

        /*
         * Test sqlite3_uri_key
         * Iterate through keys to ensure the parser handles iteration correctly.
         */
        for (int i = 0; i < 10; i++) {
            const char *key = sqlite3_uri_key(name, i);
            (void)key;
        }

        /*
         * REMOVED: sqlite3_database_file_object
         * This function requires a filename passed to xOpen with specific flags (SQLITE_OPEN_MAIN_JOURNAL or SQLITE_OPEN_WAL).
         * Passing a filename constructed via sqlite3_create_filename triggers undefined behavior
         * and leads to NULL pointer dereferences (as seen in the ASAN log: pPager->fd where pPager is NULL).
         * It is unsafe to fuzz this function with constructed filenames.
         */

        /*
         * Cleanup: Free the filename structure.
         */
        sqlite3_free_filename(name);
    }

    free(input_str);
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /*
     * Strategy 1: Treat input as a raw filename string.
     */
    fuzz_filename_constructors(Data, Size);

    /*
     * Strategy 2: Try to construct a URI specifically.
     * Prefix with "file:" to test URI parsing paths.
     */
    if (Size > 0) {
        size_t new_size = Size + 6; /* "file:" + null */
        char *uri_input = (char *)malloc(new_size);
        if (uri_input) {
            memcpy(uri_input, "file:", 5);
            memcpy(uri_input + 5, Data, Size);
            uri_input[new_size - 1] = '\0';

            /* Re-run fuzzing logic on the URI-ified string */
            fuzz_filename_constructors((const uint8_t *)uri_input, new_size - 1);

            free(uri_input);
        }
    }

    /*
     * Strategy 3: Test with a simple dummy file path to ensure basic paths work.
     */
    {
        const char *dummy_path = "./dummy_file";
        sqlite3_filename name = sqlite3_create_filename(dummy_path, "", "", 0, NULL);
        if (name) {
            (void)sqlite3_filename_database(name);
            (void)sqlite3_filename_journal(name);
            (void)sqlite3_filename_wal(name);
            (void)sqlite3_uri_parameter(name, "mode");
            (void)sqlite3_uri_key(name, 0);
            sqlite3_free_filename(name);
        }
    }

    return 0;
}