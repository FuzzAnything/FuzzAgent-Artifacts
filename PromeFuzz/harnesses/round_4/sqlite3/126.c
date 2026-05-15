// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    // Create a dummy file for any file operations if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Parse input to create parameters for sqlite3_create_filename
    // We'll use a simple format: first three null-terminated strings for
    // database, journal, and WAL filenames, followed by key-value pairs
    const uint8_t *ptr = Data;
    const uint8_t *end = Data + Size;
    
    // Extract database filename (first null-terminated string)
    const char *zDatabase = "";
    if (ptr < end) {
        zDatabase = (const char *)ptr;
        while (ptr < end && *ptr != 0) ptr++;
        if (ptr < end) ptr++; // Skip null terminator
    }
    
    // Extract journal filename (second null-terminated string)
    const char *zJournal = "";
    if (ptr < end) {
        zJournal = (const char *)ptr;
        while (ptr < end && *ptr != 0) ptr++;
        if (ptr < end) ptr++; // Skip null terminator
    }
    
    // Extract WAL filename (third null-terminated string)
    const char *zWal = "";
    if (ptr < end) {
        zWal = (const char *)ptr;
        while (ptr < end && *ptr != 0) ptr++;
        if (ptr < end) ptr++; // Skip null terminator
    }
    
    // Count remaining strings for URI parameters (key-value pairs)
    int nParam = 0;
    const char **azParam = NULL;
    const uint8_t *param_start = ptr;
    
    // First pass: count valid key-value pairs
    while (ptr < end) {
        // Key must be non-empty
        const uint8_t *key_start = ptr;
        while (ptr < end && *ptr != 0) ptr++;
        if (ptr == end || ptr == key_start) break; // No key or no null terminator
        ptr++; // Skip null terminator
        
        // Value
        if (ptr >= end) break;
        const uint8_t *val_start = ptr;
        while (ptr < end && *ptr != 0) ptr++;
        if (ptr == end) break; // No null terminator for value
        ptr++; // Skip null terminator
        
        nParam++;
    }
    
    // Allocate and fill parameter array
    if (nParam > 0) {
        azParam = (const char **)malloc(2 * nParam * sizeof(const char *));
        if (azParam) {
            ptr = param_start;
            int idx = 0;
            while (ptr < end && idx < nParam) {
                // Key
                azParam[2 * idx] = (const char *)ptr;
                while (ptr < end && *ptr != 0) ptr++;
                if (ptr >= end) break;
                ptr++;
                
                // Value
                azParam[2 * idx + 1] = (const char *)ptr;
                while (ptr < end && *ptr != 0) ptr++;
                if (ptr >= end) break;
                ptr++;
                
                idx++;
            }
        }
    }
    
    // Create a valid sqlite3_filename using sqlite3_create_filename
    sqlite3_filename filename = sqlite3_create_filename(zDatabase, zJournal, zWal, nParam, azParam);
    
    // Test sqlite3_filename_database
    if (filename) {
        const char *db_name = sqlite3_filename_database(filename);
        (void)db_name; // Use result to avoid unused variable warning
    }
    
    // Test sqlite3_filename_journal
    if (filename) {
        const char *journal_name = sqlite3_filename_journal(filename);
        (void)journal_name;
    }
    
    // Test sqlite3_uri_parameter with various parameter names
    if (filename) {
        const char *param_val = sqlite3_uri_parameter(filename, "mode");
        (void)param_val;
        param_val = sqlite3_uri_parameter(filename, "cache");
        (void)param_val;
        param_val = sqlite3_uri_parameter(filename, "immutable");
        (void)param_val;
        
        // Also test with a parameter from our input if available
        if (nParam > 0 && azParam) {
            param_val = sqlite3_uri_parameter(filename, azParam[0]);
            (void)param_val;
        }
    }
    
    // Test sqlite3_database_file_object (with journal filename)
    // Note: This expects a journal filename passed to xOpen with specific flags
    // We'll call it but behavior may be undefined. Only call if string is not empty.
    if (zJournal && *zJournal) {
        sqlite3_file *file_obj = sqlite3_database_file_object(zJournal);
        (void)file_obj;
    }
    
    // Test sqlite3_database_file_object (with WAL filename)
    if (zWal && *zWal) {
        sqlite3_file *file_obj = sqlite3_database_file_object(zWal);
        (void)file_obj;
    }
    
    // Clean up
    if (azParam) {
        free(azParam);
    }
    
    // Test sqlite3_free_filename
    sqlite3_free_filename(filename);
    
    // Also test with NULL
    sqlite3_free_filename(NULL);
    
    // Additional test: Create another filename with empty strings
    sqlite3_filename filename2 = sqlite3_create_filename("", "", "", 0, NULL);
    if (filename2) {
        const char *db_name2 = sqlite3_filename_database(filename2);
        (void)db_name2;
        const char *journal_name2 = sqlite3_filename_journal(filename2);
        (void)journal_name2;
        sqlite3_free_filename(filename2);
    }
    
    // Test with invalid inputs for the other functions
    // Only test with NULL for functions that are documented to handle NULL
    const char *invalid_result = sqlite3_filename_database(NULL);
    (void)invalid_result;
    invalid_result = sqlite3_filename_journal(NULL);
    (void)invalid_result;
    invalid_result = sqlite3_uri_parameter(NULL, "mode");
    (void)invalid_result;
    
    // Do NOT test sqlite3_database_file_object with NULL as it causes segfault
    // The documentation says behavior is undefined for invalid inputs
    
    return 0;
}