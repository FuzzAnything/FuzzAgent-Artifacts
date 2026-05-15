// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:172233:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    // Create dummy file with fuzzer data
    write_dummy_file(Data, Size);
    
    // Test sqlite3_open with different filename strategies
    uint8_t open_mode = Data[0] % 4;
    const char *filename = NULL;
    
    switch (open_mode) {
        case 0:
            filename = "./dummy_file";
            break;
        case 1:
            filename = ":memory:";
            break;
        case 2:
            filename = "";
            break;
        case 3:
            // Use a simple string from data (null-terminated if possible)
            if (Size > 1) {
                char *temp_filename = (char*)malloc(Size + 1);
                if (temp_filename) {
                    memcpy(temp_filename, Data, Size);
                    temp_filename[Size] = '\0';
                    rc = sqlite3_open(temp_filename, &db);
                    free(temp_filename);
                    if (rc == SQLITE_OK && db) {
                        sqlite3_close(db);
                        db = NULL;
                    }
                }
            }
            filename = "./dummy_file";
            break;
    }
    
    // Open database
    rc = sqlite3_open(filename, &db);
    
    if (rc == SQLITE_OK && db) {
        int cur_val = 0, hiwtr_val = 0;
        
        // Test sqlite3_db_status with different operations
        uint8_t status_op = (Size > 1 ? Data[1] : 0) % 10;
        int reset_flag = (Size > 2 ? Data[2] : 0) & 1;
        
        // Try various DB status operations
        int status_ops[] = {
            SQLITE_DBSTATUS_LOOKASIDE_USED,
            SQLITE_DBSTATUS_CACHE_USED,
            SQLITE_DBSTATUS_SCHEMA_USED,
            SQLITE_DBSTATUS_STMT_USED,
            SQLITE_DBSTATUS_LOOKASIDE_HIT,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
            SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
            SQLITE_DBSTATUS_CACHE_HIT,
            SQLITE_DBSTATUS_CACHE_MISS,
            SQLITE_DBSTATUS_CACHE_WRITE
        };
        
        if (status_op < sizeof(status_ops)/sizeof(status_ops[0])) {
            sqlite3_db_status(db, status_ops[status_op], &cur_val, &hiwtr_val, reset_flag);
        }
        
        // Test sqlite3_db_config with different operations
        uint8_t config_op = (Size > 3 ? Data[3] : 0) % 5;
        
        switch (config_op) {
            case 0:
                // SQLITE_DBCONFIG_ENABLE_FKEY
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 
                                 (Size > 4 ? Data[4] : 0) & 1, NULL);
                break;
            case 1:
                // SQLITE_DBCONFIG_ENABLE_TRIGGER
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER,
                                 (Size > 4 ? Data[4] : 0) & 1, NULL);
                break;
            case 2:
                // SQLITE_DBCONFIG_ENABLE_VIEW
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW,
                                 (Size > 4 ? Data[4] : 0) & 1, NULL);
                break;
            case 3:
                // SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE
                sqlite3_db_config(db, SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE,
                                 (Size > 4 ? Data[4] : 0) & 1, NULL);
                break;
            case 4:
                // SQLITE_DBCONFIG_ENABLE_QPSG
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_QPSG,
                                 (Size > 4 ? Data[4] : 0) & 1, NULL);
                break;
        }
        
        // Test sqlite3_db_release_memory
        sqlite3_db_release_memory(db);
        
        // Test both close functions
        uint8_t close_method = (Size > 5 ? Data[5] : 0) & 1;
        
        if (close_method == 0) {
            // Use sqlite3_close
            sqlite3_close(db);
        } else {
            // Use sqlite3_close_v2
            sqlite3_close_v2(db);
        }
    } else {
        // If open failed, still try to close if db was partially created
        if (db) {
            sqlite3_close(db);
        }
    }
    
    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}