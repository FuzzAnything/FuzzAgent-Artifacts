// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
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

static sqlite3_filename create_test_filename(const uint8_t *data, size_t size) {
    // Create a valid sqlite3_filename using the official API
    // We'll create a simple database filename with some URI parameters
    const char *zDatabase = "file:test.db";
    const char *zJournal = "test.db-journal";
    const char *zWal = "test.db-wal";
    
    // Use fuzz data to create parameter strings
    // We'll create up to 2 parameters from the fuzz data
    const char *azParam[4] = {0}; // Max 2 key-value pairs
    
    if (size > 0) {
        // Create first parameter key
        static char key1[256];
        size_t key_len = size > 255 ? 255 : size;
        memcpy(key1, data, key_len);
        key1[key_len] = '\0';
        azParam[0] = key1;
        
        // Create first parameter value (use same data for simplicity)
        static char val1[256];
        memcpy(val1, data, key_len);
        val1[key_len] = '\0';
        azParam[1] = val1;
        
        if (size > 1) {
            // Create second parameter key (shifted by 1 byte)
            static char key2[256];
            size_t key2_len = (size-1) > 255 ? 255 : (size-1);
            memcpy(key2, data + 1, key2_len);
            key2[key2_len] = '\0';
            azParam[2] = key2;
            
            // Create second parameter value
            static char val2[256];
            memcpy(val2, data + 1, key2_len);
            val2[key2_len] = '\0';
            azParam[3] = val2;
            
            return sqlite3_create_filename(zDatabase, zJournal, zWal, 2, azParam);
        } else {
            return sqlite3_create_filename(zDatabase, zJournal, zWal, 1, azParam);
        }
    }
    
    return sqlite3_create_filename(zDatabase, zJournal, zWal, 0, NULL);
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Create a valid sqlite3_filename handle
    sqlite3_filename filename = create_test_filename(Data, Size);
    if (!filename) {
        return 0;
    }
    
    // Test sqlite3_filename_journal
    const char *journal = sqlite3_filename_journal(filename);
    (void)journal; // Use result to avoid unused variable warning
    
    // Test sqlite3_filename_wal
    const char *wal = sqlite3_filename_wal(filename);
    (void)wal; // Use result to avoid unused variable warning
    
    // Test sqlite3_uri_parameter with various parameter names
    // Use fuzz data to create parameter names
    char param_name[256];
    size_t param_len = Size > 255 ? 255 : Size;
    memcpy(param_name, Data, param_len);
    param_name[param_len] = '\0';
    
    const char *param_value = sqlite3_uri_parameter(filename, param_name);
    (void)param_value; // Use result to avoid unused variable warning
    
    // Also test with some fixed parameter names
    sqlite3_uri_parameter(filename, "mode");
    sqlite3_uri_parameter(filename, "cache");
    sqlite3_uri_parameter(filename, "immutable");
    
    // Test sqlite3_uri_key for various indices
    // Use fuzz data to determine indices to test
    int max_idx = 5;
    if (Size > 0) {
        max_idx = Data[0] % 10; // Test up to 10 indices
    }
    
    for (int i = 0; i < max_idx; i++) {
        const char *key = sqlite3_uri_key(filename, i);
        (void)key; // Use result to avoid unused variable warning
    }
    
    // Test sqlite3_uri_int64
    // Use fuzz data for default value
    sqlite3_int64 default_val = 0;
    if (Size > sizeof(sqlite3_int64)) {
        memcpy(&default_val, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        default_val = Data[0];
    }
    
    sqlite3_int64 int_val = sqlite3_uri_int64(filename, param_name, default_val);
    (void)int_val; // Use result to avoid unused variable warning
    
    // Test with fixed parameter names
    sqlite3_uri_int64(filename, "cache_size", default_val);
    sqlite3_uri_int64(filename, "page_size", default_val);
    
    // Clean up
    sqlite3_free_filename(filename);
    
    return 0;
}