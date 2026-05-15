// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy extension entry point functions for testing auto-extension APIs */
static void dummy_extension1(void) {
    /* Empty extension - just for testing */
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

/* Helper function to get an extension function pointer based on input data */
static void (*get_extension_ptr(const uint8_t *data, size_t size))(void) {
    if (size == 0) return NULL;
    
    switch (data[0] % 4) {
        case 0: return dummy_extension1;
        case 1: return dummy_extension2;
        case 2: return dummy_extension3;
        case 3: return NULL;
        default: return NULL;
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    /* Initialize SQLite library */
    sqlite3_initialize();
    
    /* Use the fuzzer input to determine which operations to perform */
    if (Size > 0) {
        /* Test sqlite3_cancel_auto_extension */
        if (Size > 1) {
            void (*ext_func)(void) = get_extension_ptr(Data + 1, Size - 1);
            if (ext_func) {
                sqlite3_cancel_auto_extension(ext_func);
            }
        }
        
        /* Test sqlite3_enable_load_extension */
        if (Size > 2) {
            sqlite3* db = create_test_db();
            if (db) {
                int onoff = Data[2] % 2; /* 0 or 1 */
                sqlite3_enable_load_extension(db, onoff);
                sqlite3_close(db);
            }
        }
        
        /* Test sqlite3_auto_extension */
        if (Size > 3) {
            void (*ext_func)(void) = get_extension_ptr(Data + 3, Size - 3);
            if (ext_func) {
                sqlite3_auto_extension(ext_func);
            }
        }
        
        /* Test sqlite3_reset_auto_extension */
        if (Size > 4 && (Data[4] % 2 == 0)) {
            sqlite3_reset_auto_extension();
        }
    }
    
    /* Test sqlite3_shutdown - call it based on input data */
    if (Size > 0 && (Data[0] % 3 == 0)) {
        sqlite3_shutdown();
        /* Re-initialize for subsequent operations */
        sqlite3_initialize();
    }
    
    /* Clean shutdown at the end */
    sqlite3_shutdown();
    
    return 0;
}