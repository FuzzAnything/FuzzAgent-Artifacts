// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_uri_parameter(sqlite3_filename z, const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Create a parameter name from fuzzer data
    size_t param_len = Size > 100 ? 100 : Size;
    char *param = malloc(param_len + 1);
    if (!param) return;
    
    for (size_t i = 0; i < param_len; i++) {
        param[i] = Data[i] ? Data[i] : 'a';
    }
    param[param_len] = '\0';
    
    const char *result = sqlite3_uri_parameter(z, param);
    (void)result; // Use result to avoid unused variable warning
    
    free(param);
}

static void test_filename_functions(sqlite3_filename z) {
    // Test sqlite3_filename_wal
    const char *wal_name = sqlite3_filename_wal(z);
    (void)wal_name;
    
    // Test sqlite3_filename_database
    const char *db_name = sqlite3_filename_database(z);
    (void)db_name;
    
    // Test sqlite3_filename_journal
    const char *journal_name = sqlite3_filename_journal(z);
    (void)journal_name;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_filename filename = NULL;
    
    // Create a dummy database file
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        }
        fclose(fp);
    }
    
    // Open an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        remove(dummy_file);
        return 0;
    }
    
    // Attach the dummy file as a database
    char attach_sql[256];
    snprintf(attach_sql, sizeof(attach_sql), "ATTACH DATABASE '%s' AS testdb", dummy_file);
    sqlite3_exec(db, attach_sql, NULL, NULL, NULL);
    
    // Get filename from the attached database
    filename = sqlite3_db_filename(db, "testdb");
    
    if (filename) {
        // Test URI parameter function with fuzzer data
        test_uri_parameter(filename, Data, Size);
        
        // Test other filename functions
        test_filename_functions(filename);
        
        // Test with NULL filename
        test_uri_parameter(NULL, Data, Size);
        (void)sqlite3_filename_wal(NULL);
        (void)sqlite3_filename_database(NULL);
        (void)sqlite3_filename_journal(NULL);
    }
    
    // Test with main database filename
    sqlite3_filename main_filename = sqlite3_db_filename(db, "main");
    if (main_filename) {
        test_uri_parameter(main_filename, Data, Size);
        test_filename_functions(main_filename);
    }
    
    // Clean up
    sqlite3_close(db);
    remove(dummy_file);
    
    return 0;
}