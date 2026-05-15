// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy extension functions for testing auto-extension APIs */
static void dummy_extension1(void) {
    /* Empty extension function */
}

static void dummy_extension2(void) {
    /* Another empty extension function */
}

static void dummy_extension3(void) {
    /* Yet another empty extension function */
}

/* Helper function to create a dummy database connection */
static sqlite3* create_dummy_db(void) {
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/* Helper function to close database if not NULL */
static void close_dummy_db(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    /* Use the input data to determine which APIs to test and with what parameters */
    if (Size == 0) {
        return 0;
    }

    /* Extract parameters from fuzzer input */
    uint8_t api_selector = Data[0] % 6;  /* Which API to test (0-5) */
    uint8_t param1 = (Size > 1) ? Data[1] : 0;
    uint8_t param2 = (Size > 2) ? Data[2] : 0;
    
    /* Initialize SQLite if needed */
    sqlite3_initialize();

    switch (api_selector) {
        case 0: /* sqlite3_cancel_auto_extension */
            {
                /* Select which dummy extension to try to cancel */
                void (*ext_func)(void) = NULL;
                switch (param1 % 3) {
                    case 0: ext_func = dummy_extension1; break;
                    case 1: ext_func = dummy_extension2; break;
                    case 2: ext_func = dummy_extension3; break;
                }
                
                /* First register it, then try to cancel */
                sqlite3_auto_extension(ext_func);
                int result = sqlite3_cancel_auto_extension(ext_func);
                (void)result; /* Use result to avoid unused variable warning */
            }
            break;
            
        case 1: /* sqlite3_shutdown */
            sqlite3_shutdown();
            /* Re-initialize for subsequent tests */
            sqlite3_initialize();
            break;
            
        case 2: /* sqlite3_reset_auto_extension */
            /* Register some extensions first */
            sqlite3_auto_extension(dummy_extension1);
            sqlite3_auto_extension(dummy_extension2);
            sqlite3_reset_auto_extension();
            break;
            
        case 3: /* sqlite3_enable_load_extension */
            {
                sqlite3* db = create_dummy_db();
                if (db) {
                    /* Use param1 to determine on/off state (0 or 1) */
                    int onoff = param1 % 2;
                    int rc = sqlite3_enable_load_extension(db, onoff);
                    (void)rc; /* Use rc to avoid unused variable warning */
                    close_dummy_db(db);
                }
            }
            break;
            
        case 4: /* sqlite3_initialize */
            /* Initialize multiple times to test idempotency */
            for (int i = 0; i < (param1 % 4) + 1; i++) {
                sqlite3_initialize();
            }
            break;
            
        case 5: /* sqlite3_auto_extension */
            {
                /* Register different extensions based on input */
                switch (param1 % 4) {
                    case 0:
                        sqlite3_auto_extension(dummy_extension1);
                        break;
                    case 1:
                        sqlite3_auto_extension(dummy_extension2);
                        break;
                    case 2:
                        sqlite3_auto_extension(dummy_extension3);
                        break;
                    case 3:
                        /* Try registering the same extension multiple times */
                        sqlite3_auto_extension(dummy_extension1);
                        sqlite3_auto_extension(dummy_extension1);
                        sqlite3_auto_extension(dummy_extension1);
                        break;
                }
            }
            break;
    }

    /* Clean up any registered extensions */
    sqlite3_reset_auto_extension();
    
    /* Final shutdown */
    sqlite3_shutdown();

    return 0;
}