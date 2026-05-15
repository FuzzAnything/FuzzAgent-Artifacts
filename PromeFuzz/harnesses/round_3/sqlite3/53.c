// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char* generate_string(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) {
        return "";
    }
    
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    
    size_t len = *offset - start;
    char* str = malloc(len + 1);
    if (!str) {
        return "";
    }
    
    memcpy(str, data + start, len);
    str[len] = 0;
    
    if (*offset < size) {
        (*offset)++; // Skip null terminator
    }
    
    return str;
}

int LLVMFuzzerTestOneInput_53(const uint8_t* Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Generate strings for database, journal, and WAL filenames
    const char* zDatabase = generate_string(Data, Size, &offset);
    const char* zJournal = generate_string(Data, Size, &offset);
    const char* zWal = generate_string(Data, Size, &offset);
    
    // Generate number of parameters (0-10)
    int nParam = Data[offset % Size] % 11;
    offset = (offset + 1) % Size;
    
    // Generate parameter key-value pairs
    const char** azParam = NULL;
    if (nParam > 0) {
        azParam = malloc(2 * nParam * sizeof(const char*));
        if (azParam) {
            for (int i = 0; i < 2 * nParam; i++) {
                azParam[i] = generate_string(Data, Size, &offset);
            }
        } else {
            nParam = 0;
        }
    }
    
    // Create a valid sqlite3_filename using sqlite3_create_filename
    sqlite3_filename fname = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    if (fname) {
        // Test sqlite3_uri_parameter with various parameter names
        const char* test_params[] = {"mode", "cache", "immutable", "psow", "nolock", "vfs"};
        for (size_t i = 0; i < sizeof(test_params) / sizeof(test_params[0]); i++) {
            const char* result = sqlite3_uri_parameter(fname, test_params[i]);
            (void)result; // Use result to avoid unused variable warning
        }
        
        // Test with a parameter from our generated list
        if (nParam > 0 && azParam) {
            for (int i = 0; i < nParam; i++) {
                const char* key = azParam[2 * i];
                const char* result = sqlite3_uri_parameter(fname, key);
                (void)result;
            }
        }
        
        // Test sqlite3_filename_wal
        const char* wal_result = sqlite3_filename_wal(fname);
        (void)wal_result;
        
        // Test sqlite3_filename_database
        const char* db_result = sqlite3_filename_database(fname);
        (void)db_result;
        
        // Test sqlite3_filename_journal
        const char* journal_result = sqlite3_filename_journal(fname);
        (void)journal_result;
        
        // Test with NULL filename for sqlite3_uri_parameter
        const char* null_result = sqlite3_uri_parameter(NULL, "mode");
        (void)null_result;
        
        // Free the filename
        sqlite3_free_filename(fname);
    }
    
    // Test sqlite3_free_filename with NULL
    sqlite3_free_filename(NULL);
    
    // Free allocated strings
    free((void*)zDatabase);
    free((void*)zJournal);
    free((void*)zWal);
    
    if (azParam) {
        for (int i = 0; i < 2 * nParam; i++) {
            free((void*)azParam[i]);
        }
        free(azParam);
    }
    
    return 0;
}