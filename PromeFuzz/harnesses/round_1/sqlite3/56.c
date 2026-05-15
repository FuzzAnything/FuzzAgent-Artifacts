// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_uri_parameter(sqlite3_filename filename) {
    // Test with various parameter names
    const char* params[] = {"mode", "cache", "immutable", "psow", "nolock", 
                           "vfs", "journal_mode", "page_size", "temp_store"};
    
    for (size_t i = 0; i < sizeof(params)/sizeof(params[0]); i++) {
        const char* result = sqlite3_uri_parameter(filename, params[i]);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test with NULL filename
    sqlite3_uri_parameter(NULL, "mode");
    
    // Test with empty parameter
    sqlite3_uri_parameter(filename, "");
}

static void fuzz_filename_functions(sqlite3_filename filename) {
    // Test all filename extraction functions
    const char* db = sqlite3_filename_database(filename);
    const char* journal = sqlite3_filename_journal(filename);
    const char* wal = sqlite3_filename_wal(filename);
    
    (void)db;
    (void)journal;
    (void)wal;
    
    // Test with NULL input
    sqlite3_filename_database(NULL);
    sqlite3_filename_journal(NULL);
    sqlite3_filename_wal(NULL);
}

static sqlite3_filename create_fuzz_filename(const uint8_t *data, size_t size) {
    // Use fixed strings for database, journal, and WAL names
    const char* database = "test.db";
    const char* journal = "test.db-journal";
    const char* wal = "test.db-wal";
    
    // Create URI parameters from fuzzer input
    // We'll create up to 3 key-value pairs from the input data
    int nParam = 0;
    const char* params[6] = {0}; // 2 * max 3 parameters
    
    // Use static buffers for parameter values to avoid stack-use-after-scope
    static char mode_val[2];
    static char cache_val[3];
    static char imm_val[2];
    
    // Try to parse some parameters from the input
    if (size >= 2) {
        // First parameter: mode (first byte as string)
        const char* mode_key = "mode";
        mode_val[0] = (data[0] % 26) + 'a'; // Convert to letter
        mode_val[1] = '\0';
        params[0] = mode_key;
        params[1] = mode_val;
        nParam++;
        
        if (size >= 4) {
            // Second parameter: cache size
            const char* cache_key = "cache";
            cache_val[0] = '0' + (data[1] % 10);
            cache_val[1] = '0' + (data[2] % 10);
            cache_val[2] = '\0';
            params[2] = cache_key;
            params[3] = cache_val;
            nParam++;
            
            if (size >= 6) {
                // Third parameter: immutable flag
                const char* imm_key = "immutable";
                imm_val[0] = (data[3] & 1) ? '1' : '0';
                imm_val[1] = '\0';
                params[4] = imm_key;
                params[5] = imm_val;
                nParam++;
            }
        }
    }
    
    // Create the filename
    return sqlite3_create_filename(database, journal, wal, nParam, params);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *data, size_t size) {
    // Create a filename using the fuzzer input
    sqlite3_filename filename = create_fuzz_filename(data, size);
    
    if (filename) {
        // Test URI parameter extraction
        fuzz_uri_parameter(filename);
        
        // Test filename extraction functions
        fuzz_filename_functions(filename);
        
        // Free the filename
        sqlite3_free_filename(filename);
    }
    
    // Test free_filename with NULL
    sqlite3_free_filename(NULL);
    
    // Test with minimal input to create empty parameters
    if (size == 0) {
        sqlite3_filename empty_filename = sqlite3_create_filename("", "", "", 0, NULL);
        if (empty_filename) {
            fuzz_uri_parameter(empty_filename);
            fuzz_filename_functions(empty_filename);
            sqlite3_free_filename(empty_filename);
        }
    }
    
    return 0;
}