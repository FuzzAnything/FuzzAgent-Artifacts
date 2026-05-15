// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_filename at sqlite3.c:175684:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_key at sqlite3.c:175747:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_free_filename at sqlite3.c:175720:17 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_uri_parameter at sqlite3.c:175738:24 in sqlite3.h
// sqlite3_uri_boolean at sqlite3.c:175761:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static int open_test_db(sqlite3 **db) {
    return sqlite3_open(":memory:", db);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for potential file operations
    create_dummy_file(Data, Size);
    
    // Test sqlite3_create_filename and sqlite3_free_filename
    const char *database = (const char *)Data;
    const char *journal = "";
    const char *wal = "";
    
    // Limit parameter count to avoid excessive memory usage
    int nParam = Data[0] % 8;  // 0-7 parameters
    const char **azParam = NULL;
    
    if (nParam > 0 && Size > 1) {
        // Simple parameter array - in real fuzzing this would be more sophisticated
        azParam = (const char **)malloc(2 * nParam * sizeof(const char *));
        if (azParam) {
            for (int i = 0; i < nParam; i++) {
                azParam[2*i] = "key";
                azParam[2*i + 1] = "value";
            }
        }
    }
    
    sqlite3_filename filename = sqlite3_create_filename(database, journal, wal, nParam, azParam);
    
    if (filename) {
        // Test sqlite3_uri_parameter
        const char *param_value = sqlite3_uri_parameter(filename, "key");
        (void)param_value;  // Use result to avoid compiler warnings
        
        // Test sqlite3_uri_key
        for (int i = 0; i < nParam + 2; i++) {  // Test beyond bounds
            const char *key = sqlite3_uri_key(filename, i);
            (void)key;
        }
        
        // Test sqlite3_uri_boolean
        int bool_val = sqlite3_uri_boolean(filename, "key", 0);
        (void)bool_val;
        
        sqlite3_free_filename(filename);
    }
    
    if (azParam) free(azParam);
    
    // Test sqlite3_db_filename
    sqlite3 *db = NULL;
    if (open_test_db(&db) == SQLITE_OK && db) {
        sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
        (void)db_filename;
        
        // Test URI functions with database filename if available
        if (db_filename) {
            const char *test_param = sqlite3_uri_parameter(db_filename, "mode");
            (void)test_param;
            
            int test_bool = sqlite3_uri_boolean(db_filename, "cache", 1);
            (void)test_bool;
        }
        
        sqlite3_close(db);
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}