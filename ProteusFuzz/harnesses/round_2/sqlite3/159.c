// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
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
 * Fuzz driver for SQLite filename manipulation APIs.
 * Target APIs:
 * - sqlite3_filename_journal
 * - sqlite3_uri_parameter
 * - sqlite3_filename_wal
 * - sqlite3_free_filename
 * - sqlite3_filename_database
 *
 * Note: sqlite3_database_file_object is excluded as per instructions.
 */

static void write_dummy_file(const char *path, const uint8_t *data, size_t size) {
    FILE *f = fopen(path, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure null termination */
    char *input_buf = (char *)malloc(Size + 1);
    if (!input_buf) return 0;
    memcpy(input_buf, Data, Size);
    input_buf[Size] = '\0';

    sqlite3_filename zFilename = NULL;
    const char *journal_name = NULL;
    const char *wal_name = NULL;
    const char *db_name = NULL;
    const char *param_value = NULL;

    /* 
     * sqlite3_create_filename signature:
     * sqlite3_filename sqlite3_create_filename(
     *   const char *zDatabase,
     *   const char *zJournal,
     *   const char *zWal,
     *   int nParam,
     *   const char **azParam
     * );
     * We pass NULL for optional components and 0 for parameters to create a basic filename structure.
     * CRITICAL FIX: sqlite3_create_filename calls strlen on its arguments. 
     * Passing NULL triggers a SEGV. We must pass an empty string "" instead of NULL.
     */

    /* Strategy 1: Direct fuzzed input as database filename */
    zFilename = sqlite3_create_filename(input_buf, "", "", 0, NULL);
    if (zFilename) {
        journal_name = sqlite3_filename_journal(zFilename);
        wal_name = sqlite3_filename_wal(zFilename);
        db_name = sqlite3_filename_database(zFilename);
        
        /* Test URI parameters with various keys */
        const char *params[] = {"mode", "immutable", "journal_mode", "cache", "temp_store", "uri", "vfs", "locking_mode", "mmap_size", "nolock", "readonly", "shared_cache"};
        for (int i = 0; i < 12; i++) {
            param_value = sqlite3_uri_parameter(zFilename, params[i]);
            (void)param_value; 
        }
        
        /* Try extracting a parameter name from the input itself */
        if (Size > 5) {
            char key[64] = {0};
            size_t key_len = (Size < 63) ? Size : 63;
            memcpy(key, input_buf, key_len);
            param_value = sqlite3_uri_parameter(zFilename, key);
            (void)param_value;
        }

        sqlite3_free_filename(zFilename);
        zFilename = NULL;
    }

    /* Strategy 2: Construct a URI-style filename */
    if (Size > 5) {
        char uri_buf[256];
        size_t copy_len = (Size < 240) ? Size : 240;
        snprintf(uri_buf, sizeof(uri_buf), "file:%.*s?mode=ro&immutable=1", (int)copy_len, input_buf);
        
        zFilename = sqlite3_create_filename(uri_buf, "", "", 0, NULL);
        if (zFilename) {
            journal_name = sqlite3_filename_journal(zFilename);
            wal_name = sqlite3_filename_wal(zFilename);
            db_name = sqlite3_filename_database(zFilename);
            
            param_value = sqlite3_uri_parameter(zFilename, "mode");
            param_value = sqlite3_uri_parameter(zFilename, "immutable");
            param_value = sqlite3_uri_parameter(zFilename, "nonexistent_param");
            
            sqlite3_free_filename(zFilename);
            zFilename = NULL;
        }
    }

    /* Strategy 3: Simulate a journal filename */
    if (Size > 10) {
        char journal_buf[256];
        size_t copy_len = (Size < 240) ? Size : 240;
        snprintf(journal_buf, sizeof(journal_buf), "%.*s-journal", (int)copy_len, input_buf);
        
        zFilename = sqlite3_create_filename(journal_buf, "", "", 0, NULL);
        if (zFilename) {
            journal_name = sqlite3_filename_journal(zFilename);
            wal_name = sqlite3_filename_wal(zFilename);
            db_name = sqlite3_filename_database(zFilename);
            
            sqlite3_free_filename(zFilename);
            zFilename = NULL;
        }
    }

    /* Strategy 4: Simulate a WAL filename */
    if (Size > 10) {
        char wal_buf[256];
        size_t copy_len = (Size < 240) ? Size : 240;
        snprintf(wal_buf, sizeof(wal_buf), "%.*s-wal", (int)copy_len, input_buf);
        
        zFilename = sqlite3_create_filename(wal_buf, "", "", 0, NULL);
        if (zFilename) {
            journal_name = sqlite3_filename_journal(zFilename);
            wal_name = sqlite3_filename_wal(zFilename);
            db_name = sqlite3_filename_database(zFilename);
            
            sqlite3_free_filename(zFilename);
            zFilename = NULL;
        }
    }

    /* Write to dummy file to satisfy file requirement if needed */
    write_dummy_file("./dummy_file", Data, Size);

    free(input_buf);
    return 0;
}