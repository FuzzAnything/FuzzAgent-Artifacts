// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_filename_database at sqlite3.c:175793:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_database_file_object at sqlite3.c:49849:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    // Create dummy file for potential use
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Parse fuzz data to create parameters
    // We'll use a simple format: first byte determines configuration
    if (Size < 1) return 0;
    
    uint8_t config = Data[0];
    size_t offset = 1;
    
    // Extract database name (up to 100 bytes)
    char db_name[101] = {0};
    size_t db_len = 0;
    if (offset < Size) {
        db_len = Data[offset] % 100;
        offset++;
        if (db_len > 0 && offset + db_len <= Size) {
            memcpy(db_name, Data + offset, db_len);
            db_name[db_len] = '\0';
            offset += db_len;
        }
    }
    
    // Extract journal name (up to 100 bytes)
    char journal_name[101] = {0};
    size_t journal_len = 0;
    if (offset < Size) {
        journal_len = Data[offset] % 100;
        offset++;
        if (journal_len > 0 && offset + journal_len <= Size) {
            memcpy(journal_name, Data + offset, journal_len);
            journal_name[journal_len] = '\0';
            offset += journal_len;
        }
    }
    
    // Extract WAL name (up to 100 bytes)
    char wal_name[101] = {0};
    size_t wal_len = 0;
    if (offset < Size) {
        wal_len = Data[offset] % 100;
        offset++;
        if (wal_len > 0 && offset + wal_len <= Size) {
            memcpy(wal_name, Data + offset, wal_len);
            wal_name[wal_len] = '\0';
            offset += wal_len;
        }
    }
    
    // Extract number of parameters (0-10)
    int nParam = 0;
    if (offset < Size) {
        nParam = Data[offset] % 11;
        offset++;
    }
    
    // Allocate parameter array
    const char **azParam = NULL;
    if (nParam > 0) {
        azParam = (const char **)malloc(2 * nParam * sizeof(const char *));
        if (!azParam) return 0;
        
        for (int i = 0; i < nParam; i++) {
            // Extract key
            char key[51] = {0};
            size_t key_len = 0;
            if (offset < Size) {
                key_len = Data[offset] % 50;
                offset++;
                if (key_len > 0 && offset + key_len <= Size) {
                    memcpy(key, Data + offset, key_len);
                    key[key_len] = '\0';
                    offset += key_len;
                } else {
                    strcpy(key, "key");
                }
            } else {
                strcpy(key, "key");
            }
            
            // Extract value
            char value[51] = {0};
            size_t value_len = 0;
            if (offset < Size) {
                value_len = Data[offset] % 50;
                offset++;
                if (value_len > 0 && offset + value_len <= Size) {
                    memcpy(value, Data + offset, value_len);
                    value[value_len] = '\0';
                    offset += value_len;
                } else {
                    strcpy(value, "value");
                }
            } else {
                strcpy(value, "value");
            }
            
            // Store pointers (need to allocate since we'll free later)
            char *key_copy = strdup(key);
            char *value_copy = strdup(value);
            azParam[2*i] = key_copy;
            azParam[2*i+1] = value_copy;
        }
    }
    
    // Test sqlite3_create_filename
    sqlite3_filename filename = sqlite3_create_filename(
        db_name[0] ? db_name : "main.db",
        journal_name[0] ? journal_name : "",
        wal_name[0] ? wal_name : "",
        nParam,
        azParam
    );
    
    if (filename) {
        // Test sqlite3_uri_parameter with various parameter names
        const char *param_names[] = {"mode", "cache", "psow", "nolock", "immutable", "key", "value"};
        for (size_t i = 0; i < sizeof(param_names)/sizeof(param_names[0]); i++) {
            const char *result = sqlite3_uri_parameter(filename, param_names[i]);
            (void)result; // Ignore result, just test for crashes
        }
        
        // Test with random parameter name from fuzz data
        if (offset < Size && Size - offset > 0) {
            size_t param_len = (Size - offset) > 50 ? 50 : (Size - offset);
            char random_param[51] = {0};
            memcpy(random_param, Data + offset, param_len);
            random_param[param_len] = '\0';
            const char *result = sqlite3_uri_parameter(filename, random_param);
            (void)result;
        }
        
        // Test sqlite3_filename_database
        const char *db_result = sqlite3_filename_database(filename);
        (void)db_result;
        
        // Test sqlite3_filename_journal
        const char *journal_result = sqlite3_filename_journal(filename);
        (void)journal_result;
        
        // Test sqlite3_database_file_object (with caution)
        // Note: This function requires specific context, but we can still call it
        // to see if it crashes with our input
        if (config % 2 == 0) {
            sqlite3_file *file_obj = sqlite3_database_file_object(filename);
            (void)file_obj;
        }
        
        // Clean up
        sqlite3_free_filename(filename);
    }
    
    // Free allocated parameter strings
    if (azParam) {
        for (int i = 0; i < nParam; i++) {
            free((void *)azParam[2*i]);
            free((void *)azParam[2*i+1]);
        }
        free(azParam);
    }
    
    return 0;
}