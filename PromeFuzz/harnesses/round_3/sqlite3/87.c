// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "sqlite3.h"

static int dummy_xOpen(sqlite3_vfs* vfs, const char* zName, sqlite3_file* file, int flags, int* pOutFlags) {
    return SQLITE_IOERR;
}

static int dummy_xDelete(sqlite3_vfs* vfs, const char* zName, int syncDir) {
    return SQLITE_IOERR;
}

static int dummy_xAccess(sqlite3_vfs* vfs, const char* zName, int flags, int* pResOut) {
    if (pResOut) *pResOut = 0;
    return SQLITE_OK;
}

static int dummy_xFullPathname(sqlite3_vfs* vfs, const char* zName, int nOut, char* zOut) {
    if (zOut && nOut > 0) {
        strncpy(zOut, zName, nOut - 1);
        zOut[nOut - 1] = '\0';
    }
    return SQLITE_OK;
}

static void* dummy_xDlOpen(sqlite3_vfs* vfs, const char* zFilename) {
    return NULL;
}

static void dummy_xDlError(sqlite3_vfs* vfs, int nByte, char* zErrMsg) {
    if (zErrMsg && nByte > 0) zErrMsg[0] = '\0';
}

static void (*dummy_xDlSym(sqlite3_vfs* vfs, void* handle, const char* zSymbol))(void) {
    return NULL;
}

static void dummy_xDlClose(sqlite3_vfs* vfs, void* handle) {}

static int dummy_xRandomness(sqlite3_vfs* vfs, int nByte, char* zOut) {
    return 0;
}

static int dummy_xSleep(sqlite3_vfs* vfs, int microseconds) {
    return microseconds;
}

static int dummy_xCurrentTime(sqlite3_vfs* vfs, double* time) {
    if (time) *time = 2440587.5; // 1970-01-01 00:00:00
    return SQLITE_OK;
}

static int dummy_xGetLastError(sqlite3_vfs* vfs, int nBuf, char* zBuf) {
    return 0;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs* vfs, sqlite3_int64* time) {
    if (time) *time = 210866760000000; // 1970-01-01 00:00:00 in milliseconds
    return SQLITE_OK;
}

static sqlite3_vfs dummy_vfs = {
    3,                           /* iVersion */
    sizeof(sqlite3_file),        /* szOsFile */
    1024,                        /* mxPathname */
    NULL,                        /* pNext */
    "dummy",                     /* zName */
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
    NULL,                        /* xSetSystemCall */
    NULL,                        /* xGetSystemCall */
    NULL                         /* xNextSystemCall */
};

static void create_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, f);
        } else {
            const char* default_data = "SQLite format 3";
            fwrite(default_data, 1, strlen(default_data), f);
        }
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_87(const uint8_t* data, size_t size) {
    if (size < 1) return 0;
    
    create_dummy_file(data, size);
    
    sqlite3* db = NULL;
    sqlite3* db2 = NULL;
    int rc;
    void* dummy_ptr = NULL;
    
    unsigned int seed = time(NULL) ^ (size << 16) ^ data[0];
    srand(seed);
    
    int op = data[0] % 256;
    int sleep_ms = (size > 1) ? (data[1] % 1000) : 0;
    const char* db_name = (size > 2 && (data[2] % 2)) ? "main" : NULL;
    
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_file_control(db, db_name, op, &dummy_ptr);
        
        sqlite3_sleep(sleep_ms);
        
        sqlite3_close(db);
    }
    
    const char* vfs_name = (size > 3 && (data[3] % 2)) ? "dummy" : NULL;
    sqlite3_vfs* vfs = sqlite3_vfs_find(vfs_name);
    
    if (vfs) {
        int make_default = (size > 4) ? (data[4] % 2) : 0;
        sqlite3_vfs_register(vfs, make_default);
    }
    
    sqlite3_vfs_register(&dummy_vfs, 0);
    
    int flags = 0;
    if (size > 5) {
        if (data[5] & 0x01) flags |= SQLITE_OPEN_READONLY;
        if (data[5] & 0x02) flags |= SQLITE_OPEN_READWRITE;
        if (data[5] & 0x04) flags |= SQLITE_OPEN_CREATE;
        if (data[5] & 0x08) flags |= SQLITE_OPEN_URI;
        if (data[5] & 0x10) flags |= SQLITE_OPEN_MEMORY;
    } else {
        flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
    
    const char* zVfs = (size > 6 && (data[6] % 2)) ? "dummy" : NULL;
    rc = sqlite3_open_v2("./dummy_file", &db2, flags, zVfs);
    if (rc == SQLITE_OK && db2) {
        sqlite3_close(db2);
    }
    
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_file_control(db, "temp", op, &dummy_ptr);
        sqlite3_close(db);
    }
    
    cleanup_dummy_file();
    
    return 0;
}