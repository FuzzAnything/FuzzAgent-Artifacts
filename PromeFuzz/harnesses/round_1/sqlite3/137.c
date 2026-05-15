// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzz_uri_functions(sqlite3_filename filename, const uint8_t *param_data, size_t param_size) {
    if (filename == NULL) return;
    
    // Extract a parameter name from the fuzzer input
    // Use up to 100 bytes for parameter name, or less if input is smaller
    char param_name[101] = {0};
    size_t param_name_len = param_size < 100 ? param_size : 100;
    if (param_name_len > 0) {
        memcpy(param_name, param_data, param_name_len);
        param_name[param_name_len] = '\0';
    } else {
        // Default parameter name if no input
        strcpy(param_name, "cache");
    }
    
    // Test sqlite3_uri_parameter
    const char *param_value = sqlite3_uri_parameter(filename, param_name);
    (void)param_value; // Use result to avoid unused variable warning
    
    // Test sqlite3_uri_int64 with various default values
    sqlite3_int64 int_val = sqlite3_uri_int64(filename, param_name, 0);
    (void)int_val;
    int_val = sqlite3_uri_int64(filename, param_name, 42);
    (void)int_val;
    int_val = sqlite3_uri_int64(filename, param_name, -1);
    (void)int_val;
    
    // Test sqlite3_filename_wal
    const char *wal_name = sqlite3_filename_wal(filename);
    (void)wal_name;
    
    // Test sqlite3_filename_journal
    const char *journal_name = sqlite3_filename_journal(filename);
    (void)journal_name;
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    static int initialized = 0;
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    // Create a dummy file for database operations
    FILE *dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        // Write some initial data to make it look like a valid SQLite database
        // SQLite header: "SQLite format 3\0"
        const char header[] = "SQLite format 3\0";
        fwrite(header, 1, sizeof(header), dummy);
        fclose(dummy);
    }
    
    // Create a database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        // Test sqlite3_db_filename with different database names
        sqlite3_filename main_filename = sqlite3_db_filename(db, "main");
        sqlite3_filename temp_filename = sqlite3_db_filename(db, "temp");
        
        // Test URI functions with the filename from the database
        if (main_filename != NULL) {
            fuzz_uri_functions(main_filename, Data, Size);
        }
        
        // Test with temp database filename
        if (temp_filename != NULL) {
            fuzz_uri_functions(temp_filename, Data, Size);
        }
        
        // Test with NULL filename
        fuzz_uri_functions(NULL, Data, Size);
        
        // Close the database
        sqlite3_close(db);
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}