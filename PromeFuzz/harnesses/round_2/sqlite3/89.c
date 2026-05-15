// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_auto_extension at sqlite3.c:127635:16 in sqlite3.h
// sqlite3_cancel_auto_extension at sqlite3.c:127685:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:127713:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:127586:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

/* Dummy extension entry point for testing auto-extension functions */
static int dummy_extension_init(sqlite3 *db, const char **pzErrMsg, const struct sqlite3_api_routines *pThunk) {
    (void)db;
    (void)pzErrMsg;
    (void)pThunk;
    return SQLITE_OK;
}

static void dummy_entry_point(void) {
    /* This is the wrapper function that will be cast to void(*)(void) */
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    /* Initialize SQLite library */
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        /* If initialization fails, we can't proceed with most operations */
        return 0;
    }

    /* Create a database connection */
    sqlite3 *db = NULL;
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        /* If we can't open a database, we can still test some functions */
        sqlite3_shutdown();
        return 0;
    }

    /* Use fuzzer input to determine which functions to call and with what parameters */
    if (Size > 0) {
        /* Use first byte to determine which operations to perform */
        uint8_t op_selector = Data[0];
        
        /* Test sqlite3_enable_load_extension with different on/off values */
        if (Size > 1) {
            int onoff = Data[1] & 1;  /* Use second byte as on/off value */
            sqlite3_enable_load_extension(db, onoff);
        }
        
        /* Test sqlite3_auto_extension with dummy function */
        if ((op_selector & 0x01) && Size > 2) {
            /* Register auto-extension multiple times (duplicates should be ignored) */
            for (int i = 0; i < (Data[2] % 5) + 1; i++) {
                sqlite3_auto_extension(dummy_entry_point);
            }
        }
        
        /* Test sqlite3_cancel_auto_extension */
        if ((op_selector & 0x02) && Size > 3) {
            /* Try to cancel the auto-extension */
            sqlite3_cancel_auto_extension(dummy_entry_point);
        }
        
        /* Test sqlite3_reset_auto_extension */
        if ((op_selector & 0x04) && Size > 4) {
            /* Reset all auto-extensions */
            sqlite3_reset_auto_extension();
        }
        
        /* Test multiple initialize/shutdown cycles */
        if ((op_selector & 0x08) && Size > 5) {
            int cycles = Data[5] % 10;
            for (int i = 0; i < cycles; i++) {
                sqlite3_shutdown();
                sqlite3_initialize();
            }
        }
        
        /* Test enable_load_extension with different db connections */
        if ((op_selector & 0x10) && Size > 6) {
            sqlite3 *db2 = NULL;
            if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
                int onoff2 = Data[6] & 1;
                sqlite3_enable_load_extension(db2, onoff2);
                sqlite3_close(db2);
            }
        }
    }

    /* Clean up */
    sqlite3_close(db);
    
    /* Final shutdown */
    sqlite3_shutdown();
    
    return 0;
}