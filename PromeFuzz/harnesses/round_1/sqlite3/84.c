// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
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

static int dummy_xOpen(sqlite3_vfs *pVfs, sqlite3_filename zName, sqlite3_file *pFile,
                       int flags, int *pOutFlags) {
    return SQLITE_IOERR;
}

static int dummy_xDelete(sqlite3_vfs *pVfs, const char *zName, int syncDir) {
    return SQLITE_IOERR;
}

static int dummy_xAccess(sqlite3_vfs *pVfs, const char *zName, int flags, int *pResOut) {
    if (pResOut) *pResOut = 0;
    return SQLITE_IOERR;
}

static int dummy_xFullPathname(sqlite3_vfs *pVfs, const char *zName, int nOut, char *zOut) {
    if (nOut > 0) zOut[0] = '\0';
    return SQLITE_IOERR;
}

static void *dummy_xDlOpen(sqlite3_vfs *pVfs, const char *zFilename) {
    return NULL;
}

static void dummy_xDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg) {
    if (nByte > 0 && zErrMsg) zErrMsg[0] = '\0';
}

static void (*dummy_xDlSym(sqlite3_vfs *pVfs, void *p, const char *zSymbol))(void) {
    return NULL;
}

static void dummy_xDlClose(sqlite3_vfs *pVfs, void *p) {
}

static int dummy_xRandomness(sqlite3_vfs *pVfs, int nByte, char *zOut) {
    return 0;
}

static int dummy_xSleep(sqlite3_vfs *pVfs, int microseconds) {
    return microseconds;
}

static int dummy_xCurrentTime(sqlite3_vfs *pVfs, double *pTime) {
    if (pTime) *pTime = 0.0;
    return SQLITE_OK;
}

static int dummy_xGetLastError(sqlite3_vfs *pVfs, int n, char *zErr) {
    return 0;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *pTime) {
    if (pTime) *pTime = 0;
    return SQLITE_OK;
}

static int dummy_xSetSystemCall(sqlite3_vfs *pVfs, const char *zName, sqlite3_syscall_ptr pFunc) {
    return SQLITE_NOTFOUND;
}

static sqlite3_syscall_ptr dummy_xGetSystemCall(sqlite3_vfs *pVfs, const char *zName) {
    return NULL;
}

static const char *dummy_xNextSystemCall(sqlite3_vfs *pVfs, const char *zName) {
    return NULL;
}

static sqlite3_vfs g_dummy_vfs = {
    3,                           /* iVersion */
    0,                           /* szOsFile */
    1024,                        /* mxPathname */
    NULL,                        /* pNext */
    "dummy_vfs",                 /* zName */
    NULL,                        /* pAppData */
    dummy_xOpen,                 /* xOpen */
    dummy_xDelete,               /* xDelete */
    dummy_xAccess,               /* xAccess */
    dummy_xFullPathname,         /* xFullPathname */
    dummy_xDlOpen,               /* xDlOpen */
    dummy_xDlError,              /* xDlError */
    dummy_xDlSym,                /* xDlSym */
    dummy_xDlClose,              /* xDlClose */
    dummy_xRandomness,           /* xRandomness */
    dummy_xSleep,                /* xSleep */
    dummy_xCurrentTime,          /* xCurrentTime */
    dummy_xGetLastError,         /* xGetLastError */
    dummy_xCurrentTimeInt64,     /* xCurrentTimeInt64 */
    dummy_xSetSystemCall,        /* xSetSystemCall */
    dummy_xGetSystemCall,        /* xGetSystemCall */
    dummy_xNextSystemCall        /* xNextSystemCall */
};

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Use first byte to determine which functions to call */
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    /* Test sqlite3_sleep */
    if (offset < Size) {
        int sleep_ms = (int)Data[offset];
        sqlite3_sleep(sleep_ms);
        offset++;
    }
    
    /* Test sqlite3_vfs_register */
    if (offset < Size) {
        int make_default = (Data[offset] & 1) ? 1 : 0;
        sqlite3_vfs_register(&g_dummy_vfs, make_default);
        offset++;
    }
    
    /* Prepare dummy file for database operations */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (offset < Size) {
            size_t write_size = Size - offset;
            if (write_size > 1024) write_size = 1024;
            fwrite(Data + offset, 1, write_size, fp);
        }
        fclose(fp);
    }
    
    /* Test sqlite3_open and sqlite3_open_v2 */
    sqlite3 *db = NULL;
    int rc;
    
    if (selector & 0x01) {
        rc = sqlite3_open("./dummy_file", &db);
    } else {
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (selector & 0x02) flags |= SQLITE_OPEN_MEMORY;
        if (selector & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
        if (selector & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
        
        rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    }
    
    /* Test sqlite3_is_interrupted and sqlite3_busy_timeout if db opened successfully */
    if (rc == SQLITE_OK && db != NULL) {
        /* Test sqlite3_is_interrupted */
        sqlite3_is_interrupted(db);
        
        /* Test sqlite3_busy_timeout */
        if (offset < Size) {
            int timeout_ms = (int)Data[offset] * 10;
            sqlite3_busy_timeout(db, timeout_ms);
            offset++;
        }
        
        /* Close the database */
        sqlite3_close(db);
    }
    
    /* Clean up dummy file */
    remove("./dummy_file");
    
    return 0;
}