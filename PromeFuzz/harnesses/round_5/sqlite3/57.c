// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy extension entry points for testing auto-extension functions */
static void dummy_extension1(void) {
    /* This function will be cast to the proper signature when called by SQLite */
}

static void dummy_extension2(void) {
    /* Another dummy extension */
}

static void dummy_extension3(void) {
    /* Third dummy extension */
}

/* Helper function to create a database connection */
static sqlite3* create_test_db(void) {
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    /* Use the input data to influence behavior */
    int mode = 0;
    if (Size > 0) {
        mode = Data[0] % 8; /* 8 different modes */
    }
    
    /* Initialize SQLite if needed */
    sqlite3_initialize();
    
    switch (mode) {
        case 0:
            /* Test sqlite3_reset_auto_extension */
            sqlite3_reset_auto_extension();
            break;
            
        case 1:
            /* Test sqlite3_cancel_auto_extension with various entry points */
            {
                void (*extensions[])(void) = {dummy_extension1, dummy_extension2, dummy_extension3, NULL};
                for (int i = 0; extensions[i] != NULL; i++) {
                    sqlite3_cancel_auto_extension(extensions[i]);
                }
            }
            break;
            
        case 2:
            /* Test sqlite3_shutdown and re-initialize */
            sqlite3_shutdown();
            sqlite3_initialize();
            break;
            
        case 3:
            /* Test sqlite3_auto_extension with dummy extensions */
            sqlite3_auto_extension(dummy_extension1);
            sqlite3_auto_extension(dummy_extension2);
            sqlite3_auto_extension(dummy_extension3);
            break;
            
        case 4:
            /* Test sqlite3_enable_load_extension */
            {
                sqlite3* db = create_test_db();
                if (db) {
                    /* Toggle extension loading based on input data */
                    int enable = (Size > 1) ? (Data[1] % 2) : 0;
                    sqlite3_enable_load_extension(db, enable);
                    sqlite3_close(db);
                }
            }
            break;
            
        case 5:
            /* Test combination: register extensions then reset */
            sqlite3_auto_extension(dummy_extension1);
            sqlite3_auto_extension(dummy_extension2);
            sqlite3_reset_auto_extension();
            break;
            
        case 6:
            /* Test combination: register, cancel, then reset */
            sqlite3_auto_extension(dummy_extension1);
            sqlite3_cancel_auto_extension(dummy_extension1);
            sqlite3_reset_auto_extension();
            break;
            
        case 7:
            /* Test full lifecycle: initialize, use extensions, shutdown */
            {
                sqlite3* db = create_test_db();
                if (db) {
                    sqlite3_auto_extension(dummy_extension1);
                    sqlite3_enable_load_extension(db, 1);
                    sqlite3_close(db);
                }
                sqlite3_shutdown();
                sqlite3_initialize();
            }
            break;
    }
    
    /* Clean up */
    sqlite3_shutdown();
    
    return 0;
}