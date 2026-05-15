// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
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

static void fuzz_uri_operations(sqlite3_filename filename, const char *param_name) {
    if (!filename) return;
    
    // Test sqlite3_uri_parameter
    const char *param_value = sqlite3_uri_parameter(filename, param_name);
    (void)param_value;
    
    // Test sqlite3_uri_int64 with different default values
    sqlite3_int64 int64_val1 = sqlite3_uri_int64(filename, param_name, 0);
    sqlite3_int64 int64_val2 = sqlite3_uri_int64(filename, param_name, 100);
    sqlite3_int64 int64_val3 = sqlite3_uri_int64(filename, param_name, -1);
    (void)int64_val1;
    (void)int64_val2;
    (void)int64_val3;
    
    // Test sqlite3_filename_wal
    const char *wal_name = sqlite3_filename_wal(filename);
    (void)wal_name;
    
    // Test sqlite3_filename_journal
    const char *journal_name = sqlite3_filename_journal(filename);
    (void)journal_name;
}

static sqlite3_filename create_test_filename(const uint8_t *data, size_t size) {
    // Create a filename with URI parameters for testing
    // Format: file:./dummy_file?param1=value1&param2=value2...
    static char buffer[4096];
    
    if (size > 2000) size = 2000;
    
    // Start with base URI
    strcpy(buffer, "file:./dummy_file?");
    
    // Add parameters from fuzzer input
    size_t offset = strlen(buffer);
    for (size_t i = 0; i < size && offset < sizeof(buffer) - 10; i++) {
        if (data[i] == 0) {
            buffer[offset++] = '=';
            buffer[offset++] = 'v';
            buffer[offset++] = 'a';
            buffer[offset++] = 'l';
        } else {
            buffer[offset++] = data[i];
        }
        
        // Add separator every few bytes
        if (i % 5 == 4 && offset < sizeof(buffer) - 5) {
            buffer[offset++] = '=';
            buffer[offset++] = 'v';
            buffer[offset++] = '&';
            buffer[offset++] = 'p';
        }
    }
    
    buffer[offset] = '\0';
    
    // Create filename using sqlite3_create_filename with empty strings instead of NULL
    return sqlite3_create_filename(buffer, "", "", 0, NULL);
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create a dummy database file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write some minimal SQLite header data
        const char header[] = "SQLite format 3";
        fwrite(header, 1, sizeof(header), fp);
        fclose(fp);
    }
    
    // Test with a real database connection first
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    
    if (rc == SQLITE_OK && db != NULL) {
        // Get database filename using sqlite3_db_filename
        sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
        
        if (db_filename) {
            // Extract a parameter name from the fuzzer data
            char param_name[256];
            size_t param_len = Size > 255 ? 255 : Size;
            memcpy(param_name, Data, param_len);
            param_name[param_len] = '\0';
            
            // Test URI operations with the real database filename
            fuzz_uri_operations(db_filename, param_name);
        }
        
        // Close the database
        sqlite3_close(db);
    }
    
    // Test with a created filename
    sqlite3_filename test_filename = create_test_filename(Data, Size);
    
    if (test_filename) {
        // Extract a parameter name from the fuzzer data (different offset)
        char param_name2[256];
        size_t offset = Size > 100 ? Size - 100 : 0;
        size_t param_len2 = Size - offset > 255 ? 255 : Size - offset;
        if (param_len2 > 0) {
            memcpy(param_name2, Data + offset, param_len2);
            param_name2[param_len2] = '\0';
            
            // Test URI operations with the created filename
            fuzz_uri_operations(test_filename, param_name2);
        }
        
        // Test with empty parameter name
        fuzz_uri_operations(test_filename, "");
        
        // Free the created filename
        sqlite3_free_filename(test_filename);
    }
    
    // Test with NULL filename
    fuzz_uri_operations(NULL, "test");
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}