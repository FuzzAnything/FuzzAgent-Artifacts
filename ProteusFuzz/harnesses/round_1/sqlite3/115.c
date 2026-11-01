// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:63856:26 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:63856:26 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 filename manipulation APIs.
 * 
 * This driver attempts to exercise:
 * - sqlite3_filename_wal
 * - sqlite3_filename_journal
 * - sqlite3_filename_database
 * - sqlite3_uri_parameter
 * - sqlite3_database_file_object (with caution)
 * - sqlite3_free_filename
 *
 * Strategy:
 * 1. Parse input data as a potential database filename.
 * 2. Open a database connection to get valid filename pointers via sqlite3_db_filename.
 * 3. Invoke the target APIs on these valid pointers.
 * 4. Clean up resources properly.
 */

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *db_name = NULL;
    const char *wal_name = NULL;
    const char *journal_name = NULL;
    const char *db_base_name = NULL;
    const char *uri_val = NULL;
    sqlite3_file *file_obj = NULL;
    
    /* Skip empty inputs */
    if (Size == 0) {
        return 0;
    }

    /* Create a temporary database file name from input */
    /* We limit the size to avoid huge allocations and ensure null termination */
    size_t max_len = Size < 256 ? Size : 256;
    char *test_filename = (char *)malloc(max_len + 1);
    if (!test_filename) {
        return 0;
    }
    
    memcpy(test_filename, Data, max_len);
    test_filename[max_len] = '\0';
    
    /* Ensure the filename doesn't contain null bytes in the middle that might break parsing */
    /* Replace null bytes with a safe character if found before the end */
    for (size_t i = 0; i < max_len; i++) {
        if (test_filename[i] == '\0') {
            test_filename[i] = '_';
        }
    }

    /* Try to open a database to get a valid filename pointer from sqlite3_db_filename */
    /* We use a memory database or a temporary file to avoid side effects */
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(test_filename);
        return 0;
    }

    /* Get the database filename (will be NULL for :memory: but let's try with a file too) */
    /* To get a real filename pointer, we should open a file-based DB */
    sqlite3_close(db);
    
    /* Open a file-based database using our test filename */
    rc = sqlite3_open(test_filename, &db);
    if (rc != SQLITE_OK) {
        /* If opening fails, we can't get a valid filename pointer from sqlite3_db_filename */
        /* However, we can still try sqlite3_create_filename if available, but for now we skip */
        sqlite3_close(db);
        free(test_filename);
        return 0;
    }

    /* Get the main database filename */
    db_name = sqlite3_db_filename(db, "main");
    
    /* If we have a valid filename, test the translation APIs */
    if (db_name) {
        /* Test sqlite3_filename_wal */
        wal_name = sqlite3_filename_wal(db_name);
        (void)wal_name; /* Use result to avoid unused warning */
        
        /* Test sqlite3_filename_journal */
        journal_name = sqlite3_filename_journal(db_name);
        (void)journal_name;
        
        /* Test sqlite3_filename_database */
        db_base_name = sqlite3_filename_database(db_name);
        (void)db_base_name;
        
        /* Test sqlite3_uri_parameter with a few common parameters */
        /* We try to extract parameters that might exist in the filename */
        uri_val = sqlite3_uri_parameter(db_name, "mode");
        (void)uri_val;
        
        uri_val = sqlite3_uri_parameter(db_name, "immutable");
        (void)uri_val;
        
        uri_val = sqlite3_uri_parameter(db_name, "cache");
        (void)uri_val;
        
        /* Test sqlite3_database_file_object with the journal/wal names if they exist */
        /* This is risky and intended for VFS, but we try with NULL check */
        if (journal_name) {
            file_obj = sqlite3_database_file_object(journal_name);
            (void)file_obj;
        }
        if (wal_name) {
            file_obj = sqlite3_database_file_object(wal_name);
            (void)file_obj;
        }
        
        /* Note: sqlite3_free_filename should only be used on pointers from sqlite3_filename */
        /* sqlite3_db_filename returns pointers managed by the DB, so we do NOT free them here */
        /* We only free if we used sqlite3_create_filename, which we didn't in this path */
    }

    /* Cleanup */
    sqlite3_close(db);
    free(test_filename);
    
    return 0;
}