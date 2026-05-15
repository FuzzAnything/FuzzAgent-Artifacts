// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_filename_functions(sqlite3_filename fname) {
    // Test the filename extraction functions
    const char *db_name = sqlite3_filename_database(fname);
    const char *wal_name = sqlite3_filename_wal(fname);
    const char *journal_name = sqlite3_filename_journal(fname);
    
    // Use the results to avoid compiler optimizations
    (void)db_name;
    (void)wal_name;
    (void)journal_name;
}

static void fuzz_uri_parameter(sqlite3_filename fname, const char *param) {
    // Test URI parameter extraction
    const char *value = sqlite3_uri_parameter(fname, param);
    (void)value;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some minimal data
    if (Size < 4) {
        return 0;
    }
    
    // Create database, journal, and WAL names from fuzzer input
    // Split the input into three parts for the three filename components
    size_t third = Size / 3;
    size_t db_size = third;
    size_t journal_size = third;
    size_t wal_size = Size - 2 * third;
    
    // Allocate and copy the strings, ensuring null termination
    char *db_name = (char *)malloc(db_size + 1);
    char *journal_name = (char *)malloc(journal_size + 1);
    char *wal_name = (char *)malloc(wal_size + 1);
    
    if (!db_name || !journal_name || !wal_name) {
        free(db_name);
        free(journal_name);
        free(wal_name);
        return 0;
    }
    
    memcpy(db_name, Data, db_size);
    memcpy(journal_name, Data + db_size, journal_size);
    memcpy(wal_name, Data + db_size + journal_size, wal_size);
    
    db_name[db_size] = '\0';
    journal_name[journal_size] = '\0';
    wal_name[wal_size] = '\0';
    
    // Create URI parameters from the fuzzer input
    // Use a simple approach: create one key-value pair using the entire input
    const char *param_key = "fuzz_param";
    const char *param_value = (const char *)Data;
    
    // Create an array of parameter pointers (key1, value1, key2, value2, ...)
    const char *params[2];
    params[0] = param_key;
    params[1] = param_value;
    
    // Create a valid sqlite3_filename using the API
    sqlite3_filename fname = sqlite3_create_filename(
        db_name,      // Database name
        journal_name, // Journal name  
        wal_name,     // WAL name
        1,            // Number of parameter pairs
        params        // Parameter array
    );
    
    if (fname) {
        // Test all the filename functions
        fuzz_filename_functions(fname);
        
        // Test URI parameter extraction with various parameter names
        fuzz_uri_parameter(fname, "fuzz_param");
        fuzz_uri_parameter(fname, "mode");
        fuzz_uri_parameter(fname, "cache");
        fuzz_uri_parameter(fname, "immutable");
        
        // Test with empty parameter name
        fuzz_uri_parameter(fname, "");
        
        // Test with NULL parameter name
        fuzz_uri_parameter(fname, NULL);
        
        // Free the filename
        sqlite3_free_filename(fname);
    }
    
    // Test with NULL filename for functions that might handle it
    sqlite3_free_filename(NULL);
    
    // Test filename functions with NULL (though this may be undefined)
    // We'll comment these out as they could cause crashes
    // fuzz_filename_functions(NULL);
    // fuzz_uri_parameter(NULL, "test");
    
    // Create another filename with empty strings and no parameters
    sqlite3_filename fname2 = sqlite3_create_filename("", "", "", 0, NULL);
    if (fname2) {
        fuzz_filename_functions(fname2);
        fuzz_uri_parameter(fname2, "test");
        sqlite3_free_filename(fname2);
    }
    
    // Create a filename with database name only
    sqlite3_filename fname3 = sqlite3_create_filename(db_name, "", "", 0, NULL);
    if (fname3) {
        fuzz_filename_functions(fname3);
        sqlite3_free_filename(fname3);
    }
    
    // Clean up
    free(db_name);
    free(journal_name);
    free(wal_name);
    
    return 0;
}