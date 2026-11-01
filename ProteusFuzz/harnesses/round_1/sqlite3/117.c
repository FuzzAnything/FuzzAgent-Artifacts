// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_stricmp at sqlite3.c:36338:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function for collation comparison.
 * This is passed to sqlite3_create_collation and related functions.
 */
static int collation_compare_callback(void *pArg, int nLen1, const void *pStr1, int nLen2, const void *pStr2) {
    (void)pArg;
    if (nLen1 == 0 && nLen2 == 0) return 0;
    if (nLen1 == 0) return -1;
    if (nLen2 == 0) return 1;
    /* Perform a simple byte-wise comparison for fuzzing purposes */
    int cmp = memcmp(pStr1, pStr2, (nLen1 < nLen2) ? nLen1 : nLen2);
    if (cmp != 0) return cmp;
    return nLen1 - nLen2;
}

/*
 * Static destructor for collation context.
 */
static void collation_destroy_callback(void *pArg) {
    if (pArg) {
        free(pArg);
    }
}

/*
 * Static callback for collation_needed.
 * Registers a default collation when needed.
 */
static void collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    if (db && zName) {
        /* Register a simple collation for the requested name */
        sqlite3_create_collation(db, zName, eTextRep, NULL, collation_compare_callback);
    }
}

/*
 * Fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /*
     * 1. Prepare the environment.
     * Open an in-memory database to avoid file system dependencies.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have some data to work with */
    if (Size == 0) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * 2. Fuzz sqlite3_stricmp
     * We use the input data as two potential strings.
     * We handle NULL cases and various lengths.
     * CRITICAL FIX: Ensure strings are null-terminated before passing to sqlite3_stricmp.
     */
    
    /* Create a safe, null-terminated buffer for the first string */
    char safe_str1[256];
    size_t len1 = (Size < 255) ? Size : 255;
    memcpy(safe_str1, Data, len1);
    safe_str1[len1] = '\0';

    const char *str1 = safe_str1;
    const char *str2 = NULL;

    /* Case A: Both non-NULL (if data is long enough for two strings) */
    if (Size >= 4) {
        /* Find a null terminator in the original data to split strings safely */
        size_t first_null = strnlen((const char *)Data, Size);
        
        if (first_null < Size) {
            /* We found a null terminator, try to use the rest as str2 */
            size_t remaining = Size - first_null - 1;
            if (remaining > 0) {
                char safe_str2[256];
                size_t len2 = (remaining < 255) ? remaining : 255;
                memcpy(safe_str2, Data + first_null + 1, len2);
                safe_str2[len2] = '\0';
                str2 = safe_str2;
                
                int cmp = sqlite3_stricmp(str1, str2);
                (void)cmp; /* Suppress unused variable warning */
            } else {
                /* No valid second string, compare with NULL */
                int cmp = sqlite3_stricmp(str1, NULL);
                (void)cmp;
            }
        } else {
            /* No null terminator in the buffer, compare with NULL */
            int cmp = sqlite3_stricmp(str1, NULL);
            (void)cmp;
        }
    } else {
        /* Short data: compare with NULL */
        int cmp = sqlite3_stricmp(str1, NULL);
        (void)cmp;
    }

    /* Case B: Compare NULL with NULL */
    int cmp_null = sqlite3_stricmp(NULL, NULL);
    (void)cmp_null;

    /* Case C: Compare NULL with non-NULL */
    int cmp_null_str = sqlite3_stricmp(NULL, str1);
    (void)cmp_null_str;

    /*
     * 3. Fuzz sqlite3_create_collation
     * We use the input data as the collation name.
     */
    const char *coll_name = safe_str1; /* Use the already safe string */

    /* Register with SQLITE_UTF8 */
    rc = sqlite3_create_collation(db, coll_name, SQLITE_UTF8, NULL, collation_compare_callback);
    (void)rc;

    /* Register with SQLITE_UTF16LE */
    rc = sqlite3_create_collation(db, coll_name, SQLITE_UTF16LE, NULL, collation_compare_callback);
    (void)rc;

    /* Register with SQLITE_UTF16BE */
    rc = sqlite3_create_collation(db, coll_name, SQLITE_UTF16BE, NULL, collation_compare_callback);
    (void)rc;

    /*
     * 4. Fuzz sqlite3_create_collation16
     * We need a UTF-16 name. We'll construct a simple one from the input.
     */
    if (Size >= 2) {
        wchar_t w_name[256];
        size_t w_len = (Size / 2) < 255 ? (Size / 2) : 255;
        /* Simple conversion: take every 2 bytes as a UTF-16 char */
        for (size_t i = 0; i < w_len; i++) {
            w_name[i] = (wchar_t)(Data[i * 2] | (Data[i * 2 + 1] << 8));
        }
        w_name[w_len] = 0;

        rc = sqlite3_create_collation16(db, w_name, SQLITE_UTF16LE, NULL, collation_compare_callback);
        (void)rc;
    }

    /*
     * 5. Fuzz sqlite3_create_collation_v2
     * Similar to create_collation but with a destructor.
     */
    void *pArg = malloc(10);
    if (pArg) {
        memset(pArg, 0, 10);
        rc = sqlite3_create_collation_v2(db, coll_name, SQLITE_UTF8, pArg, collation_compare_callback, collation_destroy_callback);
        (void)rc;
    }

    /*
     * 6. Fuzz sqlite3_collation_needed
     * Register a callback that will be invoked if an unknown collation is used.
     */
    void *user_data = (void *)(uintptr_t)Size; /* Use Size as dummy user data */
    rc = sqlite3_collation_needed(db, user_data, collation_needed_callback);
    (void)rc;

    /*
     * 7. Explore more states by triggering the collation_needed callback.
     * We execute a query that uses an undefined collation.
     */
    const char *sql = "SELECT 'test' COLLATE FOO;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /*
     * 8. Fuzz sqlite3_vtab_collation
     * This function requires a valid sqlite3_index_info structure from xBestIndex.
     * Since we cannot easily trigger xBestIndex without a full virtual table implementation,
     * we skip direct fuzzing of this function in isolation.
     */

    /*
     * 9. Cleanup.
     * Close the database connection.
     */
    sqlite3_close(db);

    return 0;
}