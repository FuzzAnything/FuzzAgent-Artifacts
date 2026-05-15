// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
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

static void create_and_test_filename(const uint8_t *data, size_t size) {
    // Parse input to get database, journal, WAL strings and parameters
    // Format: [db][\0][journal][\0][wal][\0][nParam][key1][\0][val1][\0]...
    const uint8_t *ptr = data;
    const uint8_t *end = data + size;
    
    // Extract database name
    const char *zDatabase = (const char *)ptr;
    while (ptr < end && *ptr != 0) ptr++;
    if (ptr >= end) return;
    ptr++; // Skip null terminator
    
    // Extract journal name
    const char *zJournal = (const char *)ptr;
    while (ptr < end && *ptr != 0) ptr++;
    if (ptr >= end) return;
    ptr++; // Skip null terminator
    
    // Extract WAL name
    const char *zWal = (const char *)ptr;
    while (ptr < end && *ptr != 0) ptr++;
    if (ptr >= end) return;
    ptr++; // Skip null terminator
    
    // Extract number of parameters (1 byte)
    if (ptr >= end) return;
    int nParam = *ptr % 16; // Limit to reasonable number
    ptr++;
    
    // Prepare parameters array
    const char **azParam = NULL;
    if (nParam > 0) {
        azParam = (const char **)malloc(2 * nParam * sizeof(const char *));
        if (!azParam) return;
        
        for (int i = 0; i < nParam * 2; i++) {
            if (ptr >= end) {
                // Not enough data for all parameters
                free(azParam);
                return;
            }
            azParam[i] = (const char *)ptr;
            while (ptr < end && *ptr != 0) ptr++;
            if (ptr >= end) {
                free(azParam);
                return;
            }
            ptr++; // Skip null terminator
        }
    }
    
    // Create filename
    sqlite3_filename filename = sqlite3_create_filename(
        zDatabase, zJournal, zWal, nParam, azParam);
    
    if (filename) {
        // Test the various functions on the created filename
        const char *db = sqlite3_filename_database(filename);
        const char *journal = sqlite3_filename_journal(filename);
        const char *wal = sqlite3_filename_wal(filename);
        
        // Test URI parameter extraction with some sample keys
        const char *param1 = sqlite3_uri_parameter(filename, "key1");
        const char *param2 = sqlite3_uri_parameter(filename, "key2");
        const char *param3 = sqlite3_uri_parameter(filename, "mode");
        
        // Free the filename
        sqlite3_free_filename(filename);
    }
    
    if (azParam) {
        free(azParam);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        // Need at least minimal data
        return 0;
    }
    
    // Test the main functionality
    create_and_test_filename(Data, Size);
    
    return 0;
}