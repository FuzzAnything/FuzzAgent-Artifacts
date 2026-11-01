// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 filename manipulation APIs.
 * Targets:
 * - sqlite3_filename_database
 * - sqlite3_uri_parameter
 * - sqlite3_filename_wal
 * - sqlite3_filename_journal
 * - sqlite3_free_filename
 */

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * Strategy:
     * 1. Parse the input data to extract potential components for the filename.
     *    We treat the input as a potential database path or URI.
     * 2. Call sqlite3_create_filename with the parsed components.
     * 3. Invoke target APIs on the resulting filename object.
     * 4. Clean up with sqlite3_free_filename.
     */

    sqlite3_filename zFilename = NULL;
    char *zInput = (char *)malloc(Size + 1);
    if (!zInput) return 0;
    memcpy(zInput, Data, Size);
    zInput[Size] = '\0';

    /* 
     * Prepare arguments for sqlite3_create_filename.
     * We try a few strategies to maximize code coverage.
     */
    
    /* Strategy 1: Treat entire input as the database path */
    const char *zDatabase = zInput;
    const char *zJournal = NULL;
    const char *zWal = NULL;
    int nParam = 0;
    const char **azParam = NULL;

    /* 
     * If the input looks like a URI (starts with "file:"), we might want to 
     * pass it as the database name directly, and sqlite3_create_filename 
     * will parse the query parameters internally if supported, or we can 
     * manually extract them. However, the API expects separate components 
     * for Journal and Wal if we want to construct a composite name.
     * For fuzzing, passing the raw string as zDatabase is the most robust 
     * way to test the internal parsing logic of sqlite3_create_filename.
     */

    zFilename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);

    if (zFilename) {
        /* 
         * Test sqlite3_filename_database
         */
        const char *zDb = sqlite3_filename_database(zFilename);
        if (zDb) {
            (void)strlen(zDb);
        }

        /* 
         * Test sqlite3_filename_journal
         */
        const char *zJnl = sqlite3_filename_journal(zFilename);
        if (zJnl) {
            (void)strlen(zJnl);
        }

        /* 
         * Test sqlite3_filename_wal
         */
        const char *zWalRet = sqlite3_filename_wal(zFilename);
        if (zWalRet) {
            (void)strlen(zWalRet);
        }

        /* 
         * Test sqlite3_uri_parameter
         * We try standard parameters and also use parts of the input as param names.
         */
        const char *zParamVal = NULL;
        
        zParamVal = sqlite3_uri_parameter(zFilename, "mode");
        if (zParamVal) (void)strlen(zParamVal);
        
        zParamVal = sqlite3_uri_parameter(zFilename, "immutable");
        if (zParamVal) (void)strlen(zParamVal);

        zParamVal = sqlite3_uri_parameter(zFilename, "cache");
        if (zParamVal) (void)strlen(zParamVal);

        /* Try using a substring of the input as a parameter name */
        if (Size > 0 && Size < 32) {
            char zParamBuf[33];
            memcpy(zParamBuf, Data, Size);
            zParamBuf[Size] = '\0';
            zParamVal = sqlite3_uri_parameter(zFilename, zParamBuf);
            if (zParamVal) (void)strlen(zParamVal);
        }

        /* Test sqlite3_uri_boolean */
        int bVal = sqlite3_uri_boolean(zFilename, "immutable", 0);
        (void)bVal;

        /* Test sqlite3_uri_int64 */
        int64_t iVal = sqlite3_uri_int64(zFilename, "cache", -1);
        (void)iVal;

        /* Test sqlite3_uri_key */
        for (int i = 0; i < 10; i++) {
            const char *zKey = sqlite3_uri_key(zFilename, i);
            if (zKey) {
                (void)strlen(zKey);
            }
        }

        /* Cleanup */
        sqlite3_free_filename(zFilename);
    }

    free(zInput);
    return 0;
}