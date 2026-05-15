// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static int dummy_xOpen(sqlite3_vfs* vfs, sqlite3_filename zName, sqlite3_file* file, int flags, int* pOutFlags) {
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
    if (nOut > 0) {
        strncpy(zOut, zName, nOut - 1);
        zOut[nOut - 1] = '\0';
    }
    return SQLITE_OK;
}

static void* dummy_xDlOpen(sqlite3_vfs* vfs, const char* zFilename) {
    return NULL;
}

static void dummy_xDlError(sqlite3_vfs* vfs, int nByte, char* zErrMsg) {
    if (nByte > 0 && zErrMsg) {
        zErrMsg[0] = '\0';
    }
}

static void (*dummy_xDlSym(sqlite3_vfs* vfs, void* handle, const char* zSymbol))(void) {
    return NULL;
}

static void dummy_xDlClose(sqlite3_vfs* vfs, void* handle) {
}

static int dummy_xRandomness(sqlite3_vfs* vfs, int nByte, char* zOut) {
    return 0;
}

static int dummy_xSleep(sqlite3_vfs* vfs, int microseconds) {
    return 0;
}

static int dummy_xCurrentTime(sqlite3_vfs* vfs, double* pTime) {
    *pTime = 0.0;
    return SQLITE_OK;
}

static int dummy_xGetLastError(sqlite3_vfs* vfs, int nErr, char* zErr) {
    return 0;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs* vfs, sqlite3_int64* pTime) {
    *pTime = 0;
    return SQLITE_OK;
}

static int dummy_xSetSystemCall(sqlite3_vfs* vfs, const char* zName, sqlite3_syscall_ptr pFunc) {
    return SQLITE_NOTFOUND;
}

static sqlite3_syscall_ptr dummy_xGetSystemCall(sqlite3_vfs* vfs, const char* zName) {
    return NULL;
}

static const char* dummy_xNextSystemCall(sqlite3_vfs* vfs, const char* zName) {
    return NULL;
}

static sqlite3_vfs dummy_vfs = {
    3,                    /* iVersion */
    0,                    /* szOsFile - will be set dynamically */
    1024,                 /* mxPathname */
    NULL,                 /* pNext */
    "dummy_vfs",          /* zName */
    NULL,                 /* pAppData */
    dummy_xOpen,
    dummy_xDelete,
    dummy_xAccess,
    dummy_xFullPathname,
    dummy_xDlOpen,
    dummy_xDlError,
    dummy_xDlSym,
    dummy_xDlClose,
    dummy_xRandomness,
    dummy_xSleep,
    dummy_xCurrentTime,
    dummy_xGetLastError,
    dummy_xCurrentTimeInt64,
    dummy_xSetSystemCall,
    dummy_xGetSystemCall,
    dummy_xNextSystemCall
};

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // 1. Test sqlite3_sleep
    int sleep_ms = (int)Data[0];
    if (Size > 1) {
        sleep_ms = (sleep_ms << 8) | Data[1];
    }
    sqlite3_sleep(sleep_ms);
    
    // 2. Test sqlite3_vfs_register
    static int vfs_registered = 0;
    if (!vfs_registered) {
        int make_default = (Size > 2) ? (Data[2] & 1) : 0;
        sqlite3_vfs_register(&dummy_vfs, make_default);
        vfs_registered = 1;
    }
    
    // 3. Test sqlite3_open and sqlite3_is_interrupted
    sqlite3* db = NULL;
    const char* filename = "./dummy_file";
    
    // Write some data to the file if we have enough input
    FILE* fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 10) {
            size_t write_size = Size > 1024 ? 1024 : Size;
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }
    
    // Try different open modes based on input
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 3) {
        uint8_t flags_byte = Data[3];
        if (flags_byte & 0x01) open_flags |= SQLITE_OPEN_READONLY;
        if (flags_byte & 0x02) open_flags |= SQLITE_OPEN_MEMORY;
        if (flags_byte & 0x04) open_flags |= SQLITE_OPEN_NOMUTEX;
        if (flags_byte & 0x08) open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    
    // Try sqlite3_open
    int rc = sqlite3_open(filename, &db);
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_is_interrupted
        sqlite3_is_interrupted(db);
        
        // Test sqlite3_busy_timeout
        int timeout_ms = 0;
        if (Size > 4) {
            timeout_ms = (int)Data[4];
            if (Size > 5) {
                timeout_ms = (timeout_ms << 8) | Data[5];
            }
        }
        sqlite3_busy_timeout(db, timeout_ms);
        
        sqlite3_close(db);
    }
    
    // Try sqlite3_open_v2 with different VFS
    const char* vfs_name = NULL;
    if (Size > 6 && (Data[6] & 1)) {
        vfs_name = "dummy_vfs";
    }
    
    rc = sqlite3_open_v2(filename, &db, open_flags, vfs_name);
    if (rc == SQLITE_OK && db) {
        sqlite3_is_interrupted(db);
        
        // Test busy timeout again with different value
        int timeout_ms2 = 0;
        if (Size > 7) {
            timeout_ms2 = (int)Data[7];
            if (Size > 8) {
                timeout_ms2 = (timeout_ms2 << 8) | Data[8];
            }
        }
        sqlite3_busy_timeout(db, timeout_ms2);
        
        sqlite3_close(db);
    }
    
    // Clean up
    remove(filename);
    
    return 0;
}