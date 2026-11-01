// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 filename manipulation APIs.
 *
 * This driver focuses on:
 * 1. sqlite3_create_filename() to generate valid sqlite3_filename objects.
 * 2. sqlite3_filename_journal(), sqlite3_filename_wal(), sqlite3_filename_database()
 * 3. sqlite3_uri_parameter()
 * 4. sqlite3_free_filename() for cleanup.
 *
 * Note: sqlite3_database_file_object() is intentionally omitted as it requires
 * specific internal memory layouts only present during VFS xOpen calls,
 * which cannot be safely replicated in a standalone fuzz driver.
 */

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Ensure the input is null-terminated to safely parse as a string.
     * We create a buffer with a guaranteed null terminator.
     */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /*
     * Check if input_str is empty or contains only nulls to avoid passing NULL
     * to sqlite3_create_filename which expects a valid string pointer for zDatabase.
     * Although the API might handle empty strings, passing a string with length 0
     * is safer than passing a pointer that might be treated as invalid in some contexts.
     * More importantly, we must ensure we don't pass NULL to sqlite3_create_filename
     * if the input was all nulls or if we decided to skip it.
     * However, sqlite3_create_filename documentation says zDatabase can be NULL,
     * but the ASAN error indicates it calls strlen on it. So we must pass a valid string.
     * If the fuzzed data is all nulls, input_str[0] is '\0', which is a valid empty string.
     * The error trace shows strlen(zDatabase) was called on a NULL pointer.
     * This implies that in some cases, we might be passing NULL explicitly or the 
     * input_str logic failed. Let's ensure we always pass a valid non-NULL pointer.
     * The previous code passed input_str which is never NULL if malloc succeeded.
     * Wait, the error trace shows:
     * 190438|  nByte = strlen(zDatabase) + strlen(zJournal) + strlen(zWal) + 10;
     * This means zDatabase, zJournal, or zWal was NULL inside sqlite3_create_filename.
     * We passed (input_str, NULL, NULL, ...).
     * So sqlite3_create_filename is calling strlen on NULL for zJournal or zWal.
     * This suggests the version of sqlite3 being fuzzed expects non-NULL strings 
     * or handles NULL differently than expected, or the API signature in the 
     * documentation provided in the prompt is slightly different from the 
     * implementation being tested.
     *
     * Looking at the error: "AddressSanitizer: SEGV on unknown address 0x000000000000"
     * and the line "nByte = strlen(zDatabase) + strlen(zJournal) + strlen(zWal) + 10;"
     * It seems the internal implementation of sqlite3_create_filename in this build
     * does not check for NULL before calling strlen.
     *
     * Fix: Pass empty strings "" instead of NULL for zJournal and zWal.
     */
    
    sqlite3_filename fn = sqlite3_create_filename(input_str, "", "", 0, NULL);
    
    /* 
     * If creation failed (e.g., out of memory), clean up and return.
     */
    if (fn == NULL) {
        free(input_str);
        return 0;
    }

    /*
     * Invoke target APIs to explore different code paths.
     * These functions are safe to call on filenames created by sqlite3_create_filename.
     */

    /* 1. Get the database name */
    const char *db_name = sqlite3_filename_database(fn);
    (void)db_name; /* Suppress unused variable warning */

    /* 2. Get the journal name */
    const char *journal_name = sqlite3_filename_journal(fn);
    (void)journal_name;

    /* 3. Get the WAL name */
    const char *wal_name = sqlite3_filename_wal(fn);
    (void)wal_name;

    /* 4. Query URI parameters */
    /* We try a few common parameter names to trigger different branches */
    const char *param_mode = sqlite3_uri_parameter(fn, "mode");
    const char *param_cache = sqlite3_uri_parameter(fn, "cache");
    const char *param_vfs = sqlite3_uri_parameter(fn, "vfs");
    const char *param_nonexistent = sqlite3_uri_parameter(fn, "nonexistent_param_xyz");
    
    (void)param_mode;
    (void)param_cache;
    (void)param_vfs;
    (void)param_nonexistent;

    /*
     * Cleanup: Free the filename structure allocated by sqlite3_create_filename.
     * This is critical to prevent memory leaks.
     */
    sqlite3_free_filename(fn);

    /* Free the temporary input string */
    free(input_str);

    return 0;
}