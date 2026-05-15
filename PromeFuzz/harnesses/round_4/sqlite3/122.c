// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
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

static void dummy_io_methods(void) {
    // Dummy function to satisfy linker if needed
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    // Create a dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test 1: Create a valid filename using sqlite3_create_filename
    const char *zDatabase = "test.db";
    const char *zJournal = "test.db-journal";
    const char *zWal = "test.db-wal";
    
    // Create URI parameters from fuzzer data
    int nParam = 0;
    const char **azParam = NULL;
    
    if (Size > 10) {
        nParam = 1;
        azParam = (const char **)malloc(2 * sizeof(const char *));
        if (azParam) {
            char *key = (char *)malloc(6);
            char *value = (char *)malloc(6);
            if (key && value) {
                memcpy(key, "mode", 5);
                memcpy(value, Data, Size > 5 ? 5 : Size);
                value[5] = '\0';
                azParam[0] = key;
                azParam[1] = value;
            } else {
                free(key);
                free(value);
                nParam = 0;
                free(azParam);
                azParam = NULL;
            }
        }
    }
    
    // Create the filename structure
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    // Test the functions if we got a valid filename
    if (filename) {
        // Test sqlite3_filename_database
        const char *db_name = sqlite3_filename_database(filename);
        
        // Test sqlite3_filename_journal
        const char *journal_name = sqlite3_filename_journal(filename);
        
        // Test sqlite3_uri_parameter with various parameter names
        const char *param1 = sqlite3_uri_parameter(filename, "mode");
        const char *param2 = sqlite3_uri_parameter(filename, "cache");
        const char *param3 = sqlite3_uri_parameter(filename, "immutable");
        
        // Test sqlite3_database_file_object ONLY with journal_name from sqlite3_filename
        // This is the fix: use journal_name which comes from the sqlite3_filename structure
        // instead of the raw string zJournal
        if (journal_name) {
            sqlite3_file *file_obj = sqlite3_database_file_object(journal_name);
            // Note: file_obj will likely be NULL since we're not in a real VFS context,
            // but we're calling it to fuzz the function
        }
        
        // Free the filename
        sqlite3_free_filename(filename);
        
        // Free allocated parameter strings
        if (azParam) {
            free((void *)azParam[0]);
            free((void *)azParam[1]);
            free(azParam);
        }
    }
    
    // Test 2: Test with NULL and invalid inputs
    sqlite3_filename_database(NULL);
    sqlite3_filename_journal(NULL);
    sqlite3_uri_parameter(NULL, "mode");
    sqlite3_free_filename(NULL);
    sqlite3_database_file_object(NULL);
    
    // Test 3: Create another filename with empty strings
    sqlite3_filename filename2 = sqlite3_create_filename("", "", "", 0, NULL);
    if (filename2) {
        sqlite3_filename_database(filename2);
        sqlite3_filename_journal(filename2);
        sqlite3_uri_parameter(filename2, "mode");
        sqlite3_free_filename(filename2);
    }
    
    // Test 4: Create filename with more parameters from fuzzer data
    if (Size > 20) {
        // Use fuzzer data to create parameter array
        const char *params[4];
        params[0] = "key1";
        params[1] = "value1";
        params[2] = "key2";
        params[3] = "value2";
        
        sqlite3_filename filename3 = sqlite3_create_filename("db3", "db3-journal", "db3-wal", 2, params);
        if (filename3) {
            sqlite3_filename_database(filename3);
            sqlite3_filename_journal(filename3);
            
            // Test sqlite3_uri_parameter with keys that exist and don't exist
            sqlite3_uri_parameter(filename3, "key1");
            sqlite3_uri_parameter(filename3, "key2");
            sqlite3_uri_parameter(filename3, "nonexistent");
            
            // Test sqlite3_database_file_object with journal from sqlite3_filename
            const char *journal_name3 = sqlite3_filename_journal(filename3);
            if (journal_name3) {
                sqlite3_file *file_obj3 = sqlite3_database_file_object(journal_name3);
            }
            
            sqlite3_free_filename(filename3);
        }
    }
    
    // Test 5: Create a filename and test all three extraction functions
    if (Size > 5) {
        sqlite3_filename filename4 = sqlite3_create_filename("main.db", "main.db-journal", "main.db-wal", 0, NULL);
        if (filename4) {
            const char *db4 = sqlite3_filename_database(filename4);
            const char *journal4 = sqlite3_filename_journal(filename4);
            // sqlite3_filename_wal is also available but not in our target list
            
            if (journal4) {
                sqlite3_file *file_obj4 = sqlite3_database_file_object(journal4);
            }
            
            sqlite3_free_filename(filename4);
        }
    }
    
    return 0;
}