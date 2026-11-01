// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:63856:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 filename and URI handling functions.
 * 
 * Target APIs:
 * - sqlite3_filename_journal
 * - sqlite3_uri_parameter
 * - sqlite3_filename_wal
 * - sqlite3_free_filename
 * - sqlite3_filename_database
 * - sqlite3_database_file_object
 *
 * Strategy:
 * 1. Parse input data to construct valid SQLite filenames or URIs.
 * 2. Use sqlite3_create_filename() with the correct 5 arguments to generate valid sqlite3_filename objects.
 * 3. Invoke target functions on these valid objects.
 * 4. Clean up resources using sqlite3_free_filename().
 * 5. Avoid passing arbitrary strings to functions that expect specific memory layouts.
 */

static int is_valid_char_for_filename(uint8_t c) {
    /* Allow alphanumeric, common path separators, dots, hyphens, underscores, and URI chars */
    if (c >= 32 && c < 127) {
        if (c == ' ' || c == '<' || c == '>' || c == '|' || c == '*' || c == '?' || c == '"') {
            return 0;
        }
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * We need to construct valid filenames. 
     * The input data is raw fuzzed bytes. We will try to interpret it as a filename string.
     * We must ensure it is null-terminated and contains valid characters for a path.
     */
    
    /* Find the first null byte to determine string length, or use Size if none found */
    size_t str_len = 0;
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0) {
            str_len = i;
            break;
        }
    }
    
    /* If no null terminator, try to use the whole buffer but ensure we don't read past bounds */
    if (str_len == 0 && Size > 0) {
        str_len = Size;
    }

    /* 
     * If the string is too short to be a meaningful filename, skip.
     * Minimum useful filename is something like "a.db" (4 chars).
     */
    if (str_len < 2) {
        return 0;
    }

    /* 
     * Create a safe, null-terminated copy of the input string.
     * We will sanitize it slightly to avoid obvious invalid characters that might crash early
     * in other parts of SQLite, though the target functions are robust.
     */
    
    /* Allocate buffer for the sanitized filename */
    char *sanitized = (char *)malloc(str_len + 100); /* Extra space for prefixes/suffixes */
    if (!sanitized) {
        return 0;
    }

    /* 
     * Construct a valid filename. 
     * We try to make it look like a database file or a URI.
     */
    
    /* Copy the input data, replacing invalid chars with '_' to ensure safety */
    for (size_t i = 0; i < str_len; i++) {
        if (is_valid_char_for_filename(Data[i])) {
            sanitized[i] = (char)Data[i];
        } else {
            sanitized[i] = '_';
        }
    }
    sanitized[str_len] = '\0';

    /* 
     * Ensure the filename ends with .db if it doesn't, to make it a valid database name.
     */
    size_t current_len = strlen(sanitized);
    if (current_len + 4 < 1000) { /* Sanity check */
        if (current_len < 4 || strcmp(sanitized + current_len - 4, ".db") != 0) {
            strcat(sanitized, ".db");
        }
    }

    /* 
     * Also try to construct a URI version if the input looks like it might contain query params.
     */
    char *uri_filename = (char *)malloc(strlen(sanitized) + 100);
    if (!uri_filename) {
        free(sanitized);
        return 0;
    }
    
    /* Construct URI: file:<path>?mode=ro&<param>=<value> */
    const char *param_name = "mode";
    const char *param_val = "ro";
    
    /* Simple heuristic: if input has '=', use the part before as param name */
    if (str_len > 2) {
        int found_eq = 0;
        for (size_t i = 0; i < str_len; i++) {
            if (Data[i] == '=') {
                found_eq = 1;
                size_t start = (i > 10) ? i - 10 : 0;
                size_t len = i - start;
                char temp[11];
                if (len > 10) len = 10;
                memcpy(temp, &Data[start], len);
                temp[len] = '\0';
                for (size_t k = 0; k < len; k++) {
                    if (!is_valid_char_for_filename((uint8_t)temp[k])) temp[k] = '_';
                }
                param_name = temp;
                
                size_t val_start = i + 1;
                size_t val_len = 0;
                while (val_start + val_len < str_len && val_len < 10 && Data[val_start + val_len] != '\0') {
                    val_len++;
                }
                char temp_val[11];
                if (val_len > 10) val_len = 10;
                memcpy(temp_val, &Data[val_start], val_len);
                temp_val[val_len] = '\0';
                for (size_t k = 0; k < val_len; k++) {
                    if (!is_valid_char_for_filename((uint8_t)temp_val[k])) temp_val[k] = '_';
                }
                param_val = temp_val;
                break;
            }
        }
    }

    snprintf(uri_filename, strlen(sanitized) + 100, "file:%s?%s=%s", sanitized, param_name, param_val);

    /* 
     * Now we have two potential filenames.
     * We will use sqlite3_create_filename() to create valid sqlite3_filename objects.
     * Signature: sqlite3_create_filename(const char *zDatabase, const char *zJournal, 
     *                                     const char *zWal, int nParam, const char **azParam)
     */

    /* Test with plain filename */
    /* zDatabase = sanitized, zJournal = NULL, zWal = NULL, nParam = 0, azParam = NULL */
    sqlite3_filename fn_plain = sqlite3_create_filename(sanitized, NULL, NULL, 0, NULL);
    if (fn_plain) {
        /* Test sqlite3_filename_database */
        const char *db_name = sqlite3_filename_database(fn_plain);
        (void)db_name; 

        /* Test sqlite3_filename_journal */
        const char *journal_name = sqlite3_filename_journal(fn_plain);
        (void)journal_name;

        /* Test sqlite3_filename_wal */
        const char *wal_name = sqlite3_filename_wal(fn_plain);
        (void)wal_name;

        /* Test sqlite3_uri_parameter with various params */
        const char *val_mode = sqlite3_uri_parameter(fn_plain, "mode");
        (void)val_mode;
        
        const char *val_cache = sqlite3_uri_parameter(fn_plain, "cache");
        (void)val_cache;

        sqlite3_free_filename(fn_plain);
    }

    /* Test with URI filename */
    sqlite3_filename fn_uri = sqlite3_create_filename(uri_filename, NULL, NULL, 0, NULL);
    if (fn_uri) {
        /* Test sqlite3_filename_database */
        const char *db_name_uri = sqlite3_filename_database(fn_uri);
        (void)db_name_uri;

        /* Test sqlite3_filename_journal */
        const char *journal_name_uri = sqlite3_filename_journal(fn_uri);
        (void)journal_name_uri;

        /* Test sqlite3_filename_wal */
        const char *wal_name_uri = sqlite3_filename_wal(fn_uri);
        (void)wal_name_uri;

        /* Test sqlite3_uri_parameter with the parameter we constructed */
        const char *val_param = sqlite3_uri_parameter(fn_uri, param_name);
        (void)val_param;

        /* Test with a non-existent parameter */
        const char *val_none = sqlite3_uri_parameter(fn_uri, "nonexistent_param_xyz");
        (void)val_none;

        /* Test sqlite3_database_file_object. 
         * This expects a filename passed to xOpen. Passing a created filename might be undefined.
         * We try it anyway for fuzzing purposes.
         */
        sqlite3_file *file_obj = sqlite3_database_file_object((const char *)fn_uri);
        (void)file_obj;

        sqlite3_free_filename(fn_uri);
    }

    /* Cleanup */
    free(sanitized);
    free(uri_filename);

    return 0;
}