// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:173137:18 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_busy_handler(void *data, int count) {
    return 0;
}

static void dummy_trace(void *data, const char *sql) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set some dummy handlers to explore more states
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    sqlite3_trace(db, dummy_trace, NULL);
    
    // Create a dummy file for testing
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Test sqlite3_create_filename with various parameters
    const char *database = "test.db";
    const char *journal = "test.db-journal";
    const char *wal = "test.db-wal";
    
    // Create some parameter pairs from fuzzer input
    int nParam = 0;
    const char *azParam[20]; // Max 10 key-value pairs
    
    // Create buffers for parameter strings to ensure null termination
    char key_buf[10][32];
    char val_buf[10][32];
    
    // Parse fuzzer data to create parameters
    size_t offset = 0;
    while (offset < Size && nParam < 10) {
        // Key - copy up to 31 bytes and null-terminate
        size_t key_len = 0;
        if (offset < Size) {
            key_len = (Data[offset] % 31) + 1;
            if (key_len > Size - offset) key_len = Size - offset;
            if (key_len > 31) key_len = 31;
            
            memcpy(key_buf[nParam], Data + offset, key_len);
            key_buf[nParam][key_len] = '\0';
            offset += key_len;
        } else {
            strcpy(key_buf[nParam], "key");
        }
        
        // Value - copy up to 31 bytes and null-terminate
        size_t val_len = 0;
        if (offset < Size) {
            val_len = (Data[offset] % 31) + 1;
            if (val_len > Size - offset) val_len = Size - offset;
            if (val_len > 31) val_len = 31;
            
            memcpy(val_buf[nParam], Data + offset, val_len);
            val_buf[nParam][val_len] = '\0';
            offset += val_len;
        } else {
            strcpy(val_buf[nParam], "value");
        }
        
        azParam[nParam*2] = key_buf[nParam];
        azParam[nParam*2 + 1] = val_buf[nParam];
        
        nParam++;
    }
    
    // Create filename with URI parameters
    sqlite3_filename fname = sqlite3_create_filename(
        database, journal, wal, nParam, nParam > 0 ? azParam : NULL
    );
    
    if (fname) {
        // Test sqlite3_uri_key with various indices
        for (int i = -5; i < 10; i++) {
            const char *key = sqlite3_uri_key(fname, i);
            (void)key; // Use result to avoid optimization
        }
        
        // Test sqlite3_uri_parameter with various parameter names
        const char *param_names[] = {"mode", "cache", "immutable", "readonly", NULL};
        for (int i = 0; param_names[i] != NULL; i++) {
            const char *param = sqlite3_uri_parameter(fname, param_names[i]);
            (void)param;
        }
        
        // Test sqlite3_uri_boolean with various parameters and defaults
        for (int i = 0; param_names[i] != NULL; i++) {
            int bool_val = sqlite3_uri_boolean(fname, param_names[i], i % 2);
            (void)bool_val;
        }
        
        // Test with fuzzer-derived parameter names
        if (Size > 0) {
            // Use first byte as parameter name length (modulo safe range)
            size_t name_len = (Data[0] % 16) + 1;
            if (name_len > Size) name_len = Size;
            
            char param_name[17];
            memcpy(param_name, Data, name_len);
            param_name[name_len] = '\0';
            
            const char *param = sqlite3_uri_parameter(fname, param_name);
            (void)param;
            
            int bool_val = sqlite3_uri_boolean(fname, param_name, Data[0] % 2);
            (void)bool_val;
        }
        
        // Test sqlite3_db_filename with various database names
        const char *db_names[] = {"main", "temp", "test", NULL};
        for (int i = 0; db_names[i] != NULL; i++) {
            sqlite3_filename db_fname = sqlite3_db_filename(db, db_names[i]);
            (void)db_fname;
            
            // If we get a filename, test URI functions on it
            if (db_fname) {
                const char *key = sqlite3_uri_key(db_fname, 0);
                (void)key;
                
                const char *param = sqlite3_uri_parameter(db_fname, "mode");
                (void)param;
                
                int bool_val = sqlite3_uri_boolean(db_fname, "readonly", 0);
                (void)bool_val;
            }
        }
        
        // Clean up
        sqlite3_free_filename(fname);
    }
    
    // Test with NULL filename
    const char *key = sqlite3_uri_key(NULL, 0);
    (void)key;
    
    const char *param = sqlite3_uri_parameter(NULL, "mode");
    (void)param;
    
    int bool_val = sqlite3_uri_boolean(NULL, "readonly", 1);
    (void)bool_val;
    
    // Test sqlite3_db_filename with invalid database names
    if (Size > 0) {
        // Use fuzzer data to create database name
        size_t name_len = (Data[0] % 32) + 1;
        if (name_len > Size) name_len = Size;
        
        char db_name[33];
        memcpy(db_name, Data, name_len);
        db_name[name_len] = '\0';
        
        sqlite3_filename db_fname = sqlite3_db_filename(db, db_name);
        (void)db_fname;
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}