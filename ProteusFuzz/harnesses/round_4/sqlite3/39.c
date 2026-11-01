// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_compileoption_used at sqlite3.c:190765:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * 1. sqlite3_compileoption_used
 * 2. sqlite3_open_v2
 * 3. sqlite3_close
 *
 * Fixed: Removed the second sqlite3_close(db) call that caused use-after-free.
 * After the first close, the db pointer is invalid (freed) and should not be used again.
 */

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    
    /* Ensure we have enough data to work with */
    if (Size == 0) {
        return 0;
    }
    
    /* Step 1: Call sqlite3_compileoption_used with various inputs */
    /* Try different option names based on input data */
    char option_name[64];
    size_t opt_len = Size < 63 ? Size : 63;
    
    /* Create a safe option name from input data */
    memcpy(option_name, Data, opt_len);
    option_name[opt_len] = '\0';
    
    /* Test with the generated option name */
    sqlite3_compileoption_used(option_name);
    
    /* Test with NULL (should be safe) */
    sqlite3_compileoption_used(NULL);
    
    /* Test with common option names */
    sqlite3_compileoption_used("SQLITE_ENABLE_FTS3");
    sqlite3_compileoption_used("SQLITE_ENABLE_FTS4");
    sqlite3_compileoption_used("SQLITE_ENABLE_FTS5");
    sqlite3_compileoption_used("SQLITE_ENABLE_JSON1");
    sqlite3_compileoption_used("SQLITE_ENABLE_RTREE");
    
    /* Step 2: Call sqlite3_open_v2 with various flags */
    /* Use input data to determine flags */
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] % 100; /* Limit flags to reasonable range */
    }
    
    /* Ensure we have at least one valid flag combination */
    /* Common flags: SQLITE_OPEN_READONLY, SQLITE_OPEN_READWRITE, SQLITE_OPEN_CREATE */
    if (flags == 0) {
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    } else if (flags < 10) {
        flags = SQLITE_OPEN_READONLY;
    } else if (flags < 20) {
        flags = SQLITE_OPEN_READWRITE;
    } else if (flags < 30) {
        flags = SQLITE_OPEN_CREATE;
    } else {
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
    
    /* Try to open the database */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    
    /* Handle open result */
    if (rc != SQLITE_OK) {
        /* If open failed, db might be NULL or invalid */
        /* Still try to close it (safe with NULL) */
        sqlite3_close(db);
        return 0;
    }
    
    /* Step 3: Call sqlite3_close */
    /* Close the database connection */
    rc = sqlite3_close(db);
    
    /* Handle close result */
    /* SQLITE_OK means success, SQLITE_BUSY means resources still open */
    /* Both are acceptable in fuzzing context */
    
    /* Additional test: Try closing NULL (should be safe) */
    sqlite3_close(NULL);
    
    /* NOTE: Do NOT call sqlite3_close(db) again here.
     * The previous call to sqlite3_close(db) has freed the memory.
     * Calling it again would cause a use-after-free error.
     */
    
    /* Create a dummy file with some data if needed */
    FILE *f = fopen(filename, "w");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
    
    return 0;
}