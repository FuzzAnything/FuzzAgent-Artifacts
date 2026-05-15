// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
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

/* Dummy VFS structure for sqlite3_vfs_register */
static sqlite3_vfs dummy_vfs = {
    3,                    /* iVersion */
    0,                    /* szOsFile */
    1024,                 /* mxPathname */
    NULL,                 /* pNext */
    "dummy_vfs",          /* zName */
    NULL,                 /* pAppData */
    NULL,                 /* xOpen */
    NULL,                 /* xDelete */
    NULL,                 /* xAccess */
    NULL,                 /* xFullPathname */
    NULL,                 /* xDlOpen */
    NULL,                 /* xDlError */
    NULL,                 /* xDlSym */
    NULL,                 /* xDlClose */
    NULL,                 /* xRandomness */
    NULL,                 /* xSleep */
    NULL,                 /* xCurrentTime */
    NULL,                 /* xGetLastError */
    NULL,                 /* xCurrentTimeInt64 */
    NULL,                 /* xSetSystemCall */
    NULL,                 /* xGetSystemCall */
    NULL                  /* xNextSystemCall */
};

/* Helper function to create dummy file with fuzzer data */
static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, fp);
        } else {
            const char *default_data = "test";
            fwrite(default_data, 1, 4, fp);
        }
        fclose(fp);
    }
}

/* Helper function to cleanup dummy file */
static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    
    /* Create dummy file for database operations */
    create_dummy_file(Data, Size);
    
    /* Test sqlite3_open with dummy file */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db != NULL) {
        /* Test sqlite3_busy_timeout with fuzzer-derived value */
        int timeout = 0;
        if (Size > 0) {
            /* Use first byte to determine timeout value */
            timeout = (int)(Data[0]) * 10; /* Scale to get meaningful values */
        }
        sqlite3_busy_timeout(db, timeout);
        
        /* Test sqlite3_is_interrupted */
        sqlite3_is_interrupted(db);
        
        /* Close the database */
        sqlite3_close(db);
    }
    
    /* Test sqlite3_open_v2 with various flags */
    if (Size >= 2) {
        int flags = 0;
        /* Use second byte to determine flags */
        uint8_t flag_byte = Data[1];
        
        if (flag_byte & 0x01) flags |= SQLITE_OPEN_READWRITE;
        if (flag_byte & 0x02) flags |= SQLITE_OPEN_CREATE;
        if (flag_byte & 0x04) flags |= SQLITE_OPEN_MEMORY;
        if (flag_byte & 0x08) flags |= SQLITE_OPEN_NOMUTEX;
        if (flag_byte & 0x10) flags |= SQLITE_OPEN_FULLMUTEX;
        
        rc = sqlite3_open_v2("./dummy_file", &db2, flags, NULL);
        if (rc == SQLITE_OK && db2 != NULL) {
            /* Test sqlite3_busy_timeout with another fuzzer-derived value */
            int timeout2 = 0;
            if (Size > 2) {
                timeout2 = (int)(Data[2]) * 10;
            }
            sqlite3_busy_timeout(db2, timeout2);
            
            /* Test sqlite3_is_interrupted again */
            sqlite3_is_interrupted(db2);
            
            sqlite3_close(db2);
        }
    }
    
    /* Test sqlite3_sleep with fuzzer-derived value */
    int sleep_ms = 0;
    if (Size > 3) {
        sleep_ms = (int)(Data[3]) * 10; /* Scale to get meaningful sleep times */
    }
    sqlite3_sleep(sleep_ms);
    
    /* Test sqlite3_vfs_register */
    int make_default = 0;
    if (Size > 4) {
        make_default = Data[4] & 0x01; /* Use fifth byte to decide if default */
    }
    sqlite3_vfs_register(&dummy_vfs, make_default);
    
    /* Try to register again with different makeDflt value */
    sqlite3_vfs_register(&dummy_vfs, !make_default);
    
    /* Cleanup dummy file */
    cleanup_dummy_file();
    
    return 0;
}