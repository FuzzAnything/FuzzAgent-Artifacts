// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_db_status(sqlite3 *db, const uint8_t *data, size_t size) {
    int op;
    int current, highwater;
    int resetFlag;
    
    if (size < 3) return;
    
    // Use first byte for op code (modulo to keep within reasonable range)
    op = data[0] % 20;
    // Use second byte for reset flag
    resetFlag = data[1] % 2;
    
    // Call db_status
    sqlite3_db_status(db, op, &current, &highwater, resetFlag);
    
    // Try with different op codes based on input
    if (size > 3) {
        op = data[2] % 20;
        resetFlag = data[3] % 2;
        sqlite3_db_status(db, op, &current, &highwater, resetFlag);
    }
}

static void fuzz_db_config(sqlite3 *db, const uint8_t *data, size_t size) {
    int op;
    int value;
    
    if (size < 2) return;
    
    // Try different configuration operations
    op = data[0] % 20;  // SQLITE_DBCONFIG options are in the 1000s, but we'll modulo
    
    // Use second byte as value
    value = data[1];
    
    // Try various configurations
    switch (op % 5) {
        case 0:
            // SQLITE_DBCONFIG_ENABLE_FKEY
            sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, value % 2, NULL);
            break;
        case 1:
            // SQLITE_DBCONFIG_ENABLE_TRIGGER
            sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, value % 2, NULL);
            break;
        case 2:
            // SQLITE_DBCONFIG_LOOKASIDE
            sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, 0, 0);
            break;
        case 3:
            // SQLITE_DBCONFIG_MAINDBNAME
            if (size > 10) {
                char name[11];
                memcpy(name, data, 10);
                name[10] = '\0';
                sqlite3_db_config(db, SQLITE_DBCONFIG_MAINDBNAME, name, NULL);
            }
            break;
        default:
            // Try with just the op code
            sqlite3_db_config(db, op, NULL, NULL);
            break;
    }
}

static void fuzz_open_close(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    const char *filename = ":memory:";
    int rc;
    
    // Try to open database
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        // If opening fails, try with NULL
        sqlite3_open(NULL, &db);
    }
    
    if (db) {
        // Try to open another database with different flags if we have enough data
        if (size > 0) {
            sqlite3 *db2 = NULL;
            const char *filename2 = "./dummy_file";
            
            // Write some data to dummy file
            FILE *fp = fopen(filename2, "wb");
            if (fp) {
                if (size > 100) {
                    fwrite(data, 1, 100, fp);
                } else {
                    fwrite(data, 1, size, fp);
                }
                fclose(fp);
                
                // Try to open the file
                sqlite3_open(filename2, &db2);
                if (db2) {
                    sqlite3_close(db2);
                }
                remove(filename2);
            }
        }
        
        // Close the database
        sqlite3_close(db);
        
        // Try close_v2 on a new connection
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close_v2(db);
        }
    }
}

int LLVMFuzzerTestOneInput_120(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Initialize database connection
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        // If opening fails, try with NULL
        sqlite3_open(NULL, &db);
        if (db == NULL) {
            return 0;
        }
    }
    
    // Fuzz sqlite3_db_status
    fuzz_db_status(db, data, size);
    
    // Fuzz sqlite3_db_config
    fuzz_db_config(db, data, size);
    
    // Fuzz sqlite3_db_release_memory
    sqlite3_db_release_memory(db);
    
    // Try db_release_memory multiple times with different states
    if (size > 0) {
        for (int i = 0; i < (data[0] % 5); i++) {
            sqlite3_db_release_memory(db);
        }
    }
    
    // Close the database
    sqlite3_close(db);
    
    // Test open/close functions separately
    fuzz_open_close(data, size);
    
    // Try to open and close with various inputs
    if (size > 0) {
        // Try to use input as filename (limited to safe length)
        char temp_filename[256] = "./fuzz_";
        if (size < 200) {
            for (size_t i = 0; i < size && i < 200; i++) {
                if (data[i] >= 32 && data[i] < 127) {
                    temp_filename[7 + i] = data[i];
                } else {
                    temp_filename[7 + i] = 'a' + (data[i] % 26);
                }
            }
            temp_filename[7 + (size < 200 ? size : 200)] = '\0';
            
            sqlite3 *temp_db = NULL;
            rc = sqlite3_open(temp_filename, &temp_db);
            if (rc == SQLITE_OK && temp_db) {
                // Do some operations
                sqlite3_db_release_memory(temp_db);
                sqlite3_close(temp_db);
            }
            // Remove the temporary file if it was created
            remove(temp_filename);
        }
    }
    
    return 0;
}