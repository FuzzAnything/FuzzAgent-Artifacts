// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:190514:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:190537:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:190482:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 URI and filename manipulation functions.
 * Target functions:
 * - sqlite3_uri_int64
 * - sqlite3_filename_wal
 * - sqlite3_free_filename
 * - sqlite3_filename_journal
 * - sqlite3_filename_database
 * - sqlite3_uri_parameter
 *
 * Note: The sqlite3_uri_* and sqlite3_filename_* functions expect filenames
 * that are either:
 * 1. Passed by the SQLite core to a VFS xOpen method
 * 2. Returned by sqlite3_db_filename()
 * 3. Created by sqlite3_create_filename()
 *
 * Fix: sqlite3_create_filename calls strlen on all arguments. Passing NULL
 * causes a crash. We must pass empty strings "" instead of NULL for optional
 * components to ensure strlen() operates on valid memory.
 */

static void test_uri_int64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Create a null-terminated string from fuzz data
    char *raw_buf = (char *)malloc(Size + 1);
    if (!raw_buf) return;
    
    memcpy(raw_buf, Data, Size);
    raw_buf[Size] = '\0';
    
    // Use sqlite3_create_filename to create a valid filename structure.
    // Pass "" instead of NULL for zJournal and zWal to prevent strlen(NULL) crash.
    sqlite3_filename zFilename = sqlite3_create_filename(raw_buf, "", "", 0, NULL);
    
    if (zFilename) {
        const char *params[] = {"mode", "cache", "timeout", "custom", "test"};
        sqlite3_int64 defaults[] = {0, -1, 100, -1000, 9223372036854775807LL};
        
        for (int i = 0; i < 5; i++) {
            sqlite3_int64 result = sqlite3_uri_int64(zFilename, params[i], defaults[i]);
            (void)result;
        }
        
        // Free the filename created by sqlite3_create_filename
        sqlite3_free_filename(zFilename);
    }
    
    free(raw_buf);
}

static void test_filename_functions(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Create a null-terminated string from fuzz data
    char *raw_buf = (char *)malloc(Size + 1);
    if (!raw_buf) return;
    
    memcpy(raw_buf, Data, Size);
    raw_buf[Size] = '\0';
    
    // Create a proper filename using sqlite3_create_filename.
    // Pass "" instead of NULL for zJournal and zWal.
    sqlite3_filename zFilename = sqlite3_create_filename(raw_buf, "", "", 0, NULL);
    
    if (zFilename) {
        // Test sqlite3_filename_database
        const char *db_name = sqlite3_filename_database(zFilename);
        (void)db_name;
        
        // Test sqlite3_filename_journal
        const char *journal_name = sqlite3_filename_journal(zFilename);
        (void)journal_name;
        
        // Test sqlite3_filename_wal
        const char *wal_name = sqlite3_filename_wal(zFilename);
        (void)wal_name;
        
        // Test sqlite3_uri_parameter with various parameter names
        const char *param_names[] = {"mode", "cache", "timeout", "custom", "test", "vfs", "immutable"};
        for (int i = 0; i < 7; i++) {
            const char *param_value = sqlite3_uri_parameter(zFilename, param_names[i]);
            (void)param_value;
        }
        
        // Free the filename created by sqlite3_create_filename
        sqlite3_free_filename(zFilename);
    }
    
    free(raw_buf);
}

static void test_with_database_connection(const uint8_t *Data, size_t Size) {
    if (Size < 10) return; // Need minimum size for a reasonable filename
    
    // Create a null-terminated string from fuzz data
    char *filename = (char *)malloc(Size + 1);
    if (!filename) return;
    
    memcpy(filename, Data, Size);
    filename[Size] = '\0';
    
    // Ensure filename doesn't start with special characters that might cause issues
    if (filename[0] == '/' || filename[0] == '.' || filename[0] == '\\') {
        filename[0] = 't';
    }
    
    sqlite3 *db = NULL;
    
    // Try to open database in memory mode or with the filename
    int rc = sqlite3_open(filename, &db);
    
    if (rc == SQLITE_OK && db) {
        // Get the database filename
        const char *db_filename = sqlite3_db_filename(db, "main");
        
        if (db_filename) {
            // Test filename functions with the actual database filename
            const char *db_name = sqlite3_filename_database(db_filename);
            (void)db_name;
            
            const char *journal_name = sqlite3_filename_journal(db_filename);
            (void)journal_name;
            
            const char *wal_name = sqlite3_filename_wal(db_filename);
            (void)wal_name;
            
            // Test URI parameters
            const char *param_names[] = {"mode", "cache", "timeout", "custom", "test"};
            for (int i = 0; i < 5; i++) {
                const char *param_value = sqlite3_uri_parameter(db_filename, param_names[i]);
                (void)param_value;
                
                sqlite3_int64 int_val = sqlite3_uri_int64(db_filename, param_names[i], -1);
                (void)int_val;
            }
        }
        
        sqlite3_close(db);
    }
    
    free(filename);
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test basic URI integer parsing with properly created filename
    test_uri_int64(Data, Size);
    
    // Test filename manipulation functions with properly created filename
    test_filename_functions(Data, Size);
    
    // Test with actual database connection
    test_with_database_connection(Data, Size);
    
    return 0;
}