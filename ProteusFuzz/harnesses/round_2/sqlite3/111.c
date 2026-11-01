// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <sys/stat.h>

/*
 * Helper function to safely create a valid sqlite3_filename from raw fuzz data.
 * 
 * CRITICAL FIX:
 * The functions sqlite3_filename_journal, sqlite3_filename_wal, and sqlite3_filename_database
 * expect the input string to be a "compound" filename string with the format:
 * "main.db\0journal-name\0wal-name\0" (or similar internal structure).
 * 
 * The previous implementation only provided the base name. When the library tried to 
 * parse the subsequent segments (journal/wal) by skipping the null terminator of the 
 * base name, it read past the allocated buffer, causing a heap-buffer-overflow.
 * 
 * This fix ensures we allocate and initialize a full compound string structure:
 * 1. Base filename (from fuzz data or default)
 * 2. Null terminator
 * 3. Empty journal name (null terminator)
 * 4. Empty wal name (null terminator)
 * 5. Trailing null terminator
 * 
 * We also maintain the prefix padding to satisfy any internal checks that look backwards.
 */
static sqlite3_filename create_safe_filename(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;

    /* Find the first null terminator to ensure safety */
    size_t len = Size;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == '\0') {
            len = i;
            break;
        }
    }

    /* 
     * Allocate buffer with:
     * - 64 bytes prefix padding
     * - Base filename length
     * - 1 byte for base null
     * - 1 byte for journal null (empty string)
     * - 1 byte for wal null (empty string)
     * - 1 byte for final null
     * - Extra safety margin
     */
    size_t prefix_padding = 64;
    size_t total_len = prefix_padding + len + 5; 
    char *buf = (char *)malloc(total_len);
    if (!buf) return NULL;

    /* Initialize the entire buffer to 0 to ensure all segments are null-terminated */
    memset(buf, 0, total_len);

    /* Copy data starting after the padding */
    memcpy(buf + prefix_padding, Data, len);

    /* 
     * Construct the compound structure explicitly:
     * [padding][base_name]\0\0\0
     * The library expects: base_name \0 journal \0 wal \0
     * By zeroing the buffer, we ensure:
     * buf+prefix_padding+len is '\0' (end of base)
     * buf+prefix_padding+len+1 is '\0' (empty journal)
     * buf+prefix_padding+len+2 is '\0' (empty wal)
     * buf+prefix_padding+len+3 is '\0' (end of structure)
     */
    
    /* Return the pointer to the actual start of the string */
    return (sqlite3_filename)(buf + prefix_padding);
}

/*
 * Helper to create a valid URI string for testing URI parameter functions.
 * URIs are handled differently and do not require the compound filename structure.
 */
static sqlite3_filename create_safe_uri(const uint8_t *Data, size_t Size) {
    if (Size == 0) return NULL;

    /* Find null terminator */
    size_t len = Size;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == '\0') {
            len = i;
            break;
        }
    }

    /* Allocate buffer with safety prefix padding */
    size_t prefix_padding = 64;
    char *buf = (char *)malloc(prefix_padding + len + 32);
    if (!buf) return NULL;

    /* Initialize padding */
    memset(buf, 0, prefix_padding);

    /* Construct a valid URI: file:<path>?<params> */
    strcpy(buf + prefix_padding, "file:");
    size_t prefix_len = 5;
    
    /* Copy data, but replace '?' with '& ' if it's not the first param to keep structure */
    size_t copy_len = 0;
    int first_qmark = 1;
    for (size_t i = 0; i < len; i++) {
        char c = Data[i];
        if (c == '\0') break;
        if (c == '?' && first_qmark) {
            buf[prefix_padding + prefix_len + copy_len] = c;
            copy_len++;
            first_qmark = 0;
        } else if (c == '?' && !first_qmark) {
            /* Skip extra question marks to avoid malformed URIs */
            continue;
        } else {
            buf[prefix_padding + prefix_len + copy_len] = c;
            copy_len++;
        }
    }
    buf[prefix_padding + prefix_len + copy_len] = '\0';

    return (sqlite3_filename)(buf + prefix_padding);
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /*
     * Test 1: sqlite3_filename_journal, sqlite3_filename_wal, sqlite3_filename_database
     * These functions operate on raw filename strings. We construct safe strings.
     */
    sqlite3_filename fname = create_safe_filename(Data, Size);
    if (fname) {
        const char *journal = sqlite3_filename_journal(fname);
        (void)journal; /* Use result to suppress warnings */
        
        const char *wal = sqlite3_filename_wal(fname);
        (void)wal;

        const char *db = sqlite3_filename_database(fname);
        (void)db;

        /* Test with URI constructed from data */
        sqlite3_filename uri = create_safe_uri(Data, Size);
        if (uri) {
            const char *j_uri = sqlite3_filename_journal(uri);
            (void)j_uri;
            const char *w_uri = sqlite3_filename_wal(uri);
            (void)w_uri;
            const char *d_uri = sqlite3_filename_database(uri);
            (void)d_uri;
            free((void *)uri);
        }
        free((void *)fname);
    }

    /*
     * Test 2: sqlite3_db_filename, sqlite3_uri_parameter
     * These require a valid sqlite3* connection.
     */
    sqlite3 *db = NULL;
    int rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        /* If we can't open the file, we can't test these specific APIs effectively.
         * However, we can still test sqlite3_uri_parameter with constructed URIs. */
    } else {
        /* Get the filename for the main database */
        sqlite3_filename db_fname = sqlite3_db_filename(db, "main");
        if (db_fname) {
            /* Test sqlite3_uri_parameter with the retrieved filename */
            const char *param_val = sqlite3_uri_parameter(db_fname, "mode");
            (void)param_val;

            /* Test with a parameter name derived from fuzz data */
            char param_name[32];
            size_t param_len = (Size < 31) ? Size : 31;
            memcpy(param_name, Data, param_len);
            param_name[param_len] = '\0';
            
            /* Ensure param_name doesn't contain nulls in the middle if we want to be strict, 
               but sqlite3_uri_parameter handles strings, so it's okay. */
            const char *param_val2 = sqlite3_uri_parameter(db_fname, param_name);
            (void)param_val2;

            /* Test other filename functions on the connection-derived filename */
            const char *j_conn = sqlite3_filename_journal(db_fname);
            (void)j_conn;
            const char *w_conn = sqlite3_filename_wal(db_fname);
            (void)w_conn;
            const char *d_conn = sqlite3_filename_database(db_fname);
            (void)d_conn;
        }

        /* Test with a constructed URI passed directly to sqlite3_uri_parameter */
        sqlite3_filename test_uri = create_safe_uri(Data, Size);
        if (test_uri) {
            const char *val = sqlite3_uri_parameter(test_uri, "mode");
            (void)val;
            const char *val2 = sqlite3_uri_parameter(test_uri, "immutable");
            (void)val2;
            
            /* Test with dynamic param name */
            char p_name[16];
            size_t p_len = (Size < 15) ? Size : 15;
            memcpy(p_name, Data, p_len);
            p_name[p_len] = '\0';
            const char *val3 = sqlite3_uri_parameter(test_uri, p_name);
            (void)val3;

            free((void *)test_uri);
        }

        sqlite3_close(db);
    }

    /*
     * Test 3: sqlite3_free_filename
     * This function is designed to free filenames returned by sqlite3_create_filename
     * or similar internal allocations. 
     * Note: The functions sqlite3_filename_journal, sqlite3_filename_wal, etc.
     * typically return pointers into the input string or static buffers, NOT allocated memory
     * that needs sqlite3_free_filename.
     * However, to test the function itself, we can pass NULL (safe) or potentially
     * a pointer if we had one from sqlite3_create_filename (not exposed in this API set).
     * Passing NULL is the only safe generic test here without sqlite3_create_filename.
     */
    sqlite3_free_filename(NULL);

    /*
     * Additional exploration: Create a file to ensure sqlite3_open_v2 works if needed
     * and test edge cases with empty or minimal strings.
     */
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    return 0;
}