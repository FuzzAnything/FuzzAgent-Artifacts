// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_uri_int64 at sqlite3.c:175770:26 in sqlite3.h
// sqlite3_filename_journal at sqlite3.c:175797:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    // Create a dummy database file with URI parameters
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create a valid sqlite3_filename using sqlite3_create_filename
    // We'll use the dummy file as the database path and add some URI parameters
    const char *database = "./dummy_file";
    const char *journal = NULL;
    const char *wal = NULL;
    
    // Create some URI parameters from the fuzz data
    // Use first 32 bytes for parameter keys/values (if available)
    const char *param_keys[4] = {0};
    const char *param_values[4] = {0};
    char param_buf[4][64];
    
    int num_params = 0;
    if (Size >= 32) {
        // Create up to 4 parameters
        for (int i = 0; i < 4 && i * 8 < (int)Size; i++) {
            int key_len = Data[i * 8] % 32 + 1;
            int val_len = Data[i * 8 + 1] % 32 + 1;
            
            if (key_len > 31) key_len = 31;
            if (val_len > 31) val_len = 31;
            
            // Create key
            for (int j = 0; j < key_len; j++) {
                param_buf[i][j] = 'a' + (Data[(i * 8 + 2 + j) % Size] % 26);
            }
            param_buf[i][key_len] = '=';
            
            // Create value
            for (int j = 0; j < val_len; j++) {
                param_buf[i][key_len + 1 + j] = 'a' + (Data[(i * 8 + 2 + key_len + j) % Size] % 26);
            }
            param_buf[i][key_len + 1 + val_len] = '\0';
            
            param_keys[i] = param_buf[i];
            num_params++;
        }
    }
    
    // Create the filename with URI parameters
    sqlite3_filename filename = sqlite3_create_filename(database, journal, wal, 
                                                        num_params, param_keys);
    
    if (filename) {
        // Test sqlite3_uri_parameter with various parameter names
        const char *test_params[] = {"mode", "cache", "immutable", "nolock", 
                                     "psow", "vfs", "page_size", "journal_mode"};
        
        for (size_t i = 0; i < sizeof(test_params)/sizeof(test_params[0]); i++) {
            const char *param = sqlite3_uri_parameter(filename, test_params[i]);
            (void)param; // Use result to avoid compiler warnings
        }
        
        // Test sqlite3_uri_key for various indices
        for (int i = -2; i < 10; i++) {
            const char *key = sqlite3_uri_key(filename, i);
            (void)key;
        }
        
        // Test sqlite3_filename_wal
        const char *wal_name = sqlite3_filename_wal(filename);
        (void)wal_name;
        
        // Test sqlite3_uri_int64 with various parameters
        for (size_t i = 0; i < sizeof(test_params)/sizeof(test_params[0]); i++) {
            sqlite3_int64 int_val = sqlite3_uri_int64(filename, test_params[i], 
                                                     (sqlite3_int64)(Data[0] * 256 + Data[1]));
            (void)int_val;
        }
        
        // Test sqlite3_filename_journal
        const char *journal_name = sqlite3_filename_journal(filename);
        (void)journal_name;
        
        // Free the filename
        sqlite3_free_filename(filename);
    }
    
    // Also test with NULL filename
    sqlite3_uri_parameter(NULL, "test");
    sqlite3_uri_key(NULL, 0);
    sqlite3_filename_wal(NULL);
    sqlite3_uri_int64(NULL, "test", 0);
    sqlite3_filename_journal(NULL);
    
    // Test with another valid filename from sqlite3_db_filename
    sqlite3 *db = NULL;
    if (sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL) == SQLITE_OK) {
        sqlite3_filename db_filename = (sqlite3_filename)sqlite3_db_filename(db, "main");
        if (db_filename) {
            sqlite3_uri_parameter(db_filename, "cache");
            sqlite3_uri_key(db_filename, 0);
            sqlite3_filename_wal(db_filename);
            sqlite3_uri_int64(db_filename, "page_size", 4096);
            sqlite3_filename_journal(db_filename);
        }
        sqlite3_close(db);
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}

#ifdef __cplusplus
}
#endif