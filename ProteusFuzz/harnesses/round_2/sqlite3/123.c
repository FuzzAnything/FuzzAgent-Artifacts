// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:190541:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:190551:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:190491:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:190505:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:190464:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:190428:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static sqlite3_filename safe_create_filename(const uint8_t *data, size_t size) {
    if (size < 1) return NULL;
    
    // Ensure null termination
    char *db_buf = malloc(size + 1);
    if (!db_buf) return NULL;
    memcpy(db_buf, data, size);
    db_buf[size] = '\0';
    
    // Basic sanitization: replace nulls in middle with underscores to ensure valid string
    for (size_t i = 0; i < size; i++) {
        if (db_buf[i] == '\0') db_buf[i] = '_';
    }
    
    // Create filename with empty journal and wal as required
    // sqlite3_create_filename copies the string, so we can free db_buf immediately after
    sqlite3_filename fn = sqlite3_create_filename(db_buf, "", "", 0, NULL);
    free(db_buf);
    return fn;
}

static sqlite3_filename create_filename_with_params(const uint8_t *data, size_t size) {
    if (size < 1) return NULL;
    
    char *db_buf = malloc(size + 1);
    if (!db_buf) return NULL;
    memcpy(db_buf, data, size);
    db_buf[size] = '\0';
    
    for (size_t i = 0; i < size; i++) {
        if (db_buf[i] == '\0') db_buf[i] = '_';
    }
    
    // Try to create some parameters from data
    int max_params = size / 8; // rough estimate
    if (max_params > 10) max_params = 10; // limit
    
    if (max_params > 0 && size >= 16) {
        char **params = malloc(sizeof(char*) * (max_params * 2));
        if (params) {
            int valid_params = 0;
            for (int i = 0; i < max_params; i++) {
                size_t offset = i * 8;
                if (offset + 8 > size) break;
                
                char key[9] = {0};
                char val[9] = {0};
                
                memcpy(key, data + offset, 4);
                key[4] = '\0';
                memcpy(val, data + offset + 4, 4);
                val[4] = '\0';
                
                for (int j = 0; j < 4; j++) {
                    if (key[j] == '\0') key[j] = 'x';
                    if (val[j] == '\0') val[j] = 'y';
                }
                
                params[i*2] = strdup(key);
                params[i*2+1] = strdup(val);
                
                if (!params[i*2] || !params[i*2+1]) {
                    for (int k = 0; k < i*2; k++) free(params[k]);
                    free(params);
                    params = NULL;
                    break;
                }
                valid_params = i + 1;
            }
            
            if (params) {
                // sqlite3_create_filename copies the strings, so we can free db_buf before calling
                sqlite3_filename fn = sqlite3_create_filename(db_buf, "", "", valid_params, (const char**)params);
                free(db_buf); // Safe to free now
                
                // Free the parameter strings
                for (int i = 0; i < valid_params * 2; i++) {
                    if (params[i]) free(params[i]);
                }
                free(params);
                return fn;
            }
        }
    }
    
    // Fallback to no parameters
    sqlite3_filename fn = sqlite3_create_filename(db_buf, "", "", 0, NULL);
    free(db_buf); // Safe to free now
    return fn;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write dummy file for potential file-based operations
    write_dummy_file(Data, Size);
    
    // Test with basic filename creation
    sqlite3_filename fn1 = safe_create_filename(Data, Size);
    if (fn1) {
        // Test sqlite3_filename_journal
        const char *journal = sqlite3_filename_journal(fn1);
        (void)journal;
        
        // Test sqlite3_filename_wal
        const char *wal = sqlite3_filename_wal(fn1);
        (void)wal;
        
        // Test sqlite3_uri_key with various indices
        for (int i = -1; i < 5; i++) {
            const char *key = sqlite3_uri_key(fn1, i);
            (void)key;
        }
        
        // Test sqlite3_uri_boolean with various parameters
        const char *test_params[] = {"mode", "immutable", "cache", "temp_store", "uri", "invalid_param"};
        for (int i = 0; i < 6; i++) {
            int result = sqlite3_uri_boolean(fn1, test_params[i], 0);
            (void)result;
            result = sqlite3_uri_boolean(fn1, test_params[i], 1);
            (void)result;
        }
        
        sqlite3_free_filename(fn1);
    }
    
    // Test with parameters if possible
    sqlite3_filename fn2 = create_filename_with_params(Data, Size);
    if (fn2) {
        const char *journal = sqlite3_filename_journal(fn2);
        (void)journal;
        
        const char *wal = sqlite3_filename_wal(fn2);
        (void)wal;
        
        for (int i = -1; i < 10; i++) {
            const char *key = sqlite3_uri_key(fn2, i);
            (void)key;
        }
        
        const char *test_params[] = {"mode", "immutable", "cache", "temp_store", "uri", "invalid_param"};
        for (int i = 0; i < 6; i++) {
            int result = sqlite3_uri_boolean(fn2, test_params[i], 0);
            (void)result;
        }
        
        sqlite3_free_filename(fn2);
    }
    
    // Test with NULL input to ensure robustness
    const char *null_journal = sqlite3_filename_journal(NULL);
    (void)null_journal;
    
    const char *null_wal = sqlite3_filename_wal(NULL);
    (void)null_wal;
    
    const char *null_key = sqlite3_uri_key(NULL, 0);
    (void)null_key;
    
    int null_bool = sqlite3_uri_boolean(NULL, "test", 0);
    (void)null_bool;
    
    // Test free with NULL
    sqlite3_free_filename(NULL);
    
    return 0;
}