// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void fuzz_sqlite3_create_filename(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    // Parse input to create parameters
    size_t offset = 0;
    
    // Extract database name (non-NULL, can be empty)
    size_t db_len = data[offset] % 32;
    offset = (offset + 1) % size;
    char *zDatabase = malloc(db_len + 1);
    if (zDatabase) {
        for (size_t i = 0; i < db_len && offset < size; i++) {
            zDatabase[i] = data[offset];
            offset = (offset + 1) % size;
        }
        zDatabase[db_len] = '\0';
    }
    
    // Extract journal name (non-NULL, can be empty)
    size_t journal_len = data[offset] % 32;
    offset = (offset + 1) % size;
    char *zJournal = malloc(journal_len + 1);
    if (zJournal) {
        for (size_t i = 0; i < journal_len && offset < size; i++) {
            zJournal[i] = data[offset];
            offset = (offset + 1) % size;
        }
        zJournal[journal_len] = '\0';
    }
    
    // Extract WAL name (non-NULL, can be empty)
    size_t wal_len = data[offset] % 32;
    offset = (offset + 1) % size;
    char *zWal = malloc(wal_len + 1);
    if (zWal) {
        for (size_t i = 0; i < wal_len && offset < size; i++) {
            zWal[i] = data[offset];
            offset = (offset + 1) % size;
        }
        zWal[wal_len] = '\0';
    }
    
    // Determine number of parameters
    int nParam = data[offset] % 8;  // Limit to 8 parameters max
    offset = (offset + 1) % size;
    
    const char **azParam = NULL;
    if (nParam > 0) {
        azParam = malloc(2 * nParam * sizeof(const char*));
        if (azParam) {
            for (int i = 0; i < nParam; i++) {
                // Key (non-empty string)
                size_t key_len = (data[offset] % 31) + 1;  // At least 1 char
                offset = (offset + 1) % size;
                char *key = malloc(key_len + 1);
                if (key) {
                    for (size_t j = 0; j < key_len && offset < size; j++) {
                        key[j] = data[offset];
                        offset = (offset + 1) % size;
                    }
                    key[key_len] = '\0';
                    azParam[2*i] = key;
                }
                
                // Value (can be empty)
                size_t val_len = data[offset] % 32;
                offset = (offset + 1) % size;
                char *val = malloc(val_len + 1);
                if (val) {
                    for (size_t j = 0; j < val_len && offset < size; j++) {
                        val[j] = data[offset];
                        offset = (offset + 1) % size;
                    }
                    val[val_len] = '\0';
                    azParam[2*i + 1] = val;
                }
            }
        }
    }
    
    // Call the target function
    sqlite3_filename filename = sqlite3_create_filename(
        zDatabase ? zDatabase : "",
        zJournal ? zJournal : "",
        zWal ? zWal : "",
        nParam,
        azParam
    );
    
    // Test URI functions on the created filename
    if (filename) {
        // Test sqlite3_uri_parameter with various keys
        for (int i = 0; i < nParam && azParam; i++) {
            const char *key = azParam[2*i];
            if (key) {
                const char *val = sqlite3_uri_parameter(filename, key);
                (void)val;  // Use result to avoid optimization
            }
        }
        
        // Test sqlite3_uri_key for valid indices
        for (int i = 0; i < nParam; i++) {
            const char *key = sqlite3_uri_key(filename, i);
            (void)key;
        }
        
        // Test sqlite3_uri_boolean with default values
        for (int i = 0; i < nParam && azParam; i++) {
            const char *key = azParam[2*i];
            if (key) {
                int bool_val = sqlite3_uri_boolean(filename, key, i % 2);
                (void)bool_val;
            }
        }
        
        // Clean up
        sqlite3_free_filename(filename);
    }
    
    // Free allocated memory
    free(zDatabase);
    free(zJournal);
    free(zWal);
    if (azParam) {
        for (int i = 0; i < nParam; i++) {
            free((void*)azParam[2*i]);
            free((void*)azParam[2*i + 1]);
        }
        free(azParam);
    }
}

static void fuzz_sqlite3_db_filename(const uint8_t *data, size_t size) {
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Get filename for main database
        sqlite3_filename main_filename = sqlite3_db_filename(db, "main");
        (void)main_filename;
        
        // Try with different database names based on input
        if (size > 0) {
            char db_name[32];
            size_t name_len = data[0] % 31 + 1;
            for (size_t i = 0; i < name_len && i < size; i++) {
                db_name[i] = data[i % size];
            }
            db_name[name_len] = '\0';
            
            sqlite3_filename other_filename = sqlite3_db_filename(db, db_name);
            (void)other_filename;
        }
        
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_87(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Seed random behavior based on input
    unsigned int seed = 0;
    for (size_t i = 0; i < size && i < 4; i++) {
        seed = (seed << 8) | data[i];
    }
    srand(seed);
    
    // Fuzz sqlite3_create_filename and related URI functions
    fuzz_sqlite3_create_filename(data, size);
    
    // Fuzz sqlite3_db_filename
    fuzz_sqlite3_db_filename(data, size);
    
    return 0;
}