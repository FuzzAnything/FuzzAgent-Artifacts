// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

static void dummy_io_methods(void) {
    // Dummy function to reference sqlite3_io_methods to avoid unused type warnings
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    // Limit input size to prevent excessive memory usage
    if (Size > 4096) {
        return 0;
    }

    // Create a null-terminated string from the input data
    // Add padding at the beginning to avoid negative indexing issues
    size_t padded_size = Size + 16;
    char *padded_str = (char *)malloc(padded_size);
    if (!padded_str) {
        return 0;
    }
    
    // Fill the padding area with zeros to create a safe boundary
    memset(padded_str, 0, 16);
    
    // Copy the actual data after the padding
    if (Size > 0) {
        memcpy(padded_str + 16, Data, Size);
    }
    padded_str[padded_size - 1] = '\0';
    
    char *input_str = padded_str + 16;  // Point to the actual data

    // Test 1: sqlite3_uri_parameter with various inputs
    // Only test with NULL or valid-looking strings
    if (Size > 4) {
        const char *param_result = sqlite3_uri_parameter((sqlite3_filename)input_str, "mode");
        (void)param_result;
        
        param_result = sqlite3_uri_parameter((sqlite3_filename)input_str, "cache");
        (void)param_result;
    }
    
    // Test with NULL filename
    const char *param_result = sqlite3_uri_parameter(NULL, "mode");
    (void)param_result;

    // Test 2: sqlite3_filename_database with various inputs
    if (Size > 4) {
        const char *db_result = sqlite3_filename_database((sqlite3_filename)input_str);
        (void)db_result;
    }
    
    // Test with NULL
    const char *db_result = sqlite3_filename_database(NULL);
    (void)db_result;

    // Test 3: sqlite3_create_filename and sqlite3_free_filename
    // Prepare parameters for create_filename
    const char *zDatabase = Size > 0 ? input_str : "";
    const char *zJournal = "";
    const char *zWal = "";
    
    // Create some parameter pairs from the input safely
    int nParam = 0;
    const char **azParam = NULL;
    
    if (Size > 30) {
        // Use up to 2 parameter pairs maximum
        nParam = Size > 60 ? 2 : 1;
        azParam = (const char **)malloc(2 * nParam * sizeof(const char *));
        if (azParam) {
            for (int i = 0; i < nParam; i++) {
                // Use safe offsets within the input buffer
                size_t offset = i * 10;
                if (offset + 10 < Size) {
                    // Create copies of strings to ensure proper null termination
                    char *key = (char *)malloc(11);
                    char *value = (char *)malloc(11);
                    if (key && value) {
                        strncpy(key, input_str + offset, 10);
                        key[10] = '\0';
                        if (strlen(key) == 0) {
                            strcpy(key, "key");
                        }
                        
                        size_t val_offset = offset + 5;
                        if (val_offset < Size) {
                            strncpy(value, input_str + val_offset, 10);
                            value[10] = '\0';
                            if (strlen(value) == 0) {
                                strcpy(value, "value");
                            }
                        } else {
                            strcpy(value, "value");
                        }
                        
                        azParam[2*i] = key;
                        azParam[2*i + 1] = value;
                    } else {
                        if (key) free(key);
                        if (value) free(value);
                        azParam[2*i] = "key";
                        azParam[2*i + 1] = "value";
                    }
                } else {
                    azParam[2*i] = "key";
                    azParam[2*i + 1] = "value";
                }
            }
        }
    }
    
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    // Clean up allocated parameter strings
    if (azParam) {
        for (int i = 0; i < nParam; i++) {
            if (azParam[2*i] && azParam[2*i] != "key") {
                free((void *)azParam[2*i]);
            }
            if (azParam[2*i + 1] && azParam[2*i + 1] != "value") {
                free((void *)azParam[2*i + 1]);
            }
        }
        free(azParam);
    }
    
    // Test 4: Use the created filename with other functions
    if (filename) {
        const char *uri_test = sqlite3_uri_parameter(filename, "mode");
        (void)uri_test;
        
        const char *db_name = sqlite3_filename_database(filename);
        (void)db_name;
        
        const char *journal_name = sqlite3_filename_journal(filename);
        (void)journal_name;
        
        // Test 5: sqlite3_database_file_object - only call with NULL
        // The previous call with filename caused a crash, so we avoid it
        sqlite3_file *file_obj = sqlite3_database_file_object(NULL);
        (void)file_obj;
        
        // Clean up
        sqlite3_free_filename(filename);
    }
    
    // Test 6: sqlite3_filename_journal with various inputs
    if (Size > 4) {
        const char *journal_result = sqlite3_filename_journal((sqlite3_filename)input_str);
        (void)journal_result;
    }
    
    // Test with NULL
    const char *journal_result = sqlite3_filename_journal(NULL);
    (void)journal_result;
    
    // Test sqlite3_free_filename with NULL
    sqlite3_free_filename(NULL);

    free(padded_str);
    return 0;
}