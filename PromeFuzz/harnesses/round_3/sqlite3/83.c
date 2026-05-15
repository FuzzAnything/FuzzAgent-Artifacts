// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_83(const uint8_t *data, size_t size) {
    // Create a dummy file with fuzzer data
    create_dummy_file(data, size);
    
    // Initialize SQLite database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Test sqlite3_create_filename with various parameters
    const char *database = "./dummy_file";
    const char *journal = "./dummy_file-journal";
    const char *wal = "./dummy_file-wal";
    
    // Create parameter array from fuzzer data
    // We'll use up to 4 parameters (8 strings) from the input
    const char *params[8] = {0};
    int param_count = 0;
    
    if (size > 0) {
        // Simple parsing: split data by null bytes if present
        const uint8_t *ptr = data;
        const uint8_t *end = data + size;
        
        while (ptr < end && param_count < 8) {
            size_t len = 0;
            while (ptr + len < end && ptr[len] != 0 && len < 256) {
                len++;
            }
            
            if (len > 0) {
                char *str = malloc(len + 1);
                if (str) {
                    memcpy(str, ptr, len);
                    str[len] = 0;
                    params[param_count++] = str;
                }
            }
            
            ptr += len;
            if (ptr < end && *ptr == 0) {
                ptr++; // Skip null terminator
            }
        }
    }
    
    // Ensure even number of parameters for key-value pairs
    int nParam = (param_count / 2) & ~1;
    
    // Create filename using sqlite3_create_filename
    sqlite3_filename fname = sqlite3_create_filename(
        database, journal, wal, nParam/2, 
        nParam > 0 ? params : NULL);
    
    if (fname) {
        // Test sqlite3_uri_parameter with various parameter names
        const char *test_params[] = {"mode", "cache", "psow", "nolock", 
                                     "immutable", "readonly", NULL};
        
        for (int i = 0; test_params[i] != NULL; i++) {
            const char *val = sqlite3_uri_parameter(fname, test_params[i]);
            (void)val; // Use result to avoid optimization
        }
        
        // Test with parameter names from fuzzer data
        for (int i = 0; i < param_count && i < 4; i++) {
            const char *val = sqlite3_uri_parameter(fname, params[i]);
            (void)val;
        }
        
        // Test sqlite3_uri_boolean with various parameters and defaults
        for (int i = 0; test_params[i] != NULL; i++) {
            int bool_val = sqlite3_uri_boolean(fname, test_params[i], 
                                             (int)(data[0] & 1));
            (void)bool_val;
        }
        
        // Test sqlite3_uri_key for various indices
        for (int i = -2; i < 10; i++) {
            const char *key = sqlite3_uri_key(fname, i);
            (void)key;
        }
        
        // Free the filename
        sqlite3_free_filename(fname);
    }
    
    // Test sqlite3_db_filename with different database names
    const char *db_names[] = {"main", "temp", "test", NULL};
    
    for (int i = 0; db_names[i] != NULL; i++) {
        sqlite3_filename db_fname = sqlite3_db_filename(db, db_names[i]);
        if (db_fname) {
            // Test URI functions on database filename
            const char *param_val = sqlite3_uri_parameter(db_fname, "mode");
            (void)param_val;
            
            int bool_val = sqlite3_uri_boolean(db_fname, "cache", 0);
            (void)bool_val;
            
            const char *key = sqlite3_uri_key(db_fname, 0);
            (void)key;
        }
    }
    
    // Create another database connection with file URI
    sqlite3 *db2 = NULL;
    char uri[256];
    snprintf(uri, sizeof(uri), "file:./dummy_file?mode=ro&cache=shared");
    sqlite3_open_v2(uri, &db2, SQLITE_OPEN_READONLY, NULL);
    
    if (db2) {
        sqlite3_filename db2_fname = sqlite3_db_filename(db2, "main");
        if (db2_fname) {
            const char *mode = sqlite3_uri_parameter(db2_fname, "mode");
            (void)mode;
            
            int cache = sqlite3_uri_boolean(db2_fname, "cache", 1);
            (void)cache;
            
            for (int i = 0; i < 5; i++) {
                const char *key = sqlite3_uri_key(db2_fname, i);
                (void)key;
            }
        }
        sqlite3_close(db2);
    }
    
    // Test edge cases
    // NULL filename
    const char *null_param = sqlite3_uri_parameter(NULL, "mode");
    (void)null_param;
    
    int null_bool = sqlite3_uri_boolean(NULL, "cache", 42);
    (void)null_bool;
    
    const char *null_key = sqlite3_uri_key(NULL, 0);
    (void)null_key;
    
    // Clean up allocated parameter strings
    for (int i = 0; i < param_count; i++) {
        if (params[i]) {
            free((void*)params[i]);
        }
    }
    
    // Close database and cleanup
    sqlite3_close(db);
    cleanup_dummy_file();
    
    return 0;
}