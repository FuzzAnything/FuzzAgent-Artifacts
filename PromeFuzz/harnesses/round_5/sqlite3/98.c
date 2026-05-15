// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Dummy VFS implementation for testing */
static int dummy_xOpen(sqlite3_vfs *pVfs, sqlite3_filename zName, sqlite3_file *pFile,
                       int flags, int *pOutFlags) {
    return SQLITE_IOERR;
}

static int dummy_xDelete(sqlite3_vfs *pVfs, const char *zName, int syncDir) {
    return SQLITE_OK;
}

static int dummy_xAccess(sqlite3_vfs *pVfs, const char *zName, int flags, int *pResOut) {
    if (pResOut) *pResOut = 0;
    return SQLITE_OK;
}

static int dummy_xFullPathname(sqlite3_vfs *pVfs, const char *zName, int nOut, char *zOut) {
    if (zOut && nOut > 0) {
        strncpy(zOut, zName, nOut - 1);
        zOut[nOut - 1] = '\0';
    }
    return SQLITE_OK;
}

static void *dummy_xDlOpen(sqlite3_vfs *pVfs, const char *zFilename) {
    return NULL;
}

static void dummy_xDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg) {
    if (zErrMsg && nByte > 0) zErrMsg[0] = '\0';
}

static void (*dummy_xDlSym(sqlite3_vfs *pVfs, void *pHandle, const char *zSymbol))(void) {
    return NULL;
}

static void dummy_xDlClose(sqlite3_vfs *pVfs, void *pHandle) {
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

static int dummy_xGetLastError(sqlite3_vfs *pVfs, int nErr, char *zErr) {
    return 0;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *pTime) {
    if (pTime) *pTime = 0;
    return SQLITE_OK;
}

static int dummy_xSetSystemCall(sqlite3_vfs *pVfs, const char *zName, sqlite3_syscall_ptr pFunc) {
    return SQLITE_OK;
}

static sqlite3_syscall_ptr dummy_xGetSystemCall(sqlite3_vfs *pVfs, const char *zName) {
    return NULL;
}

static const char *dummy_xNextSystemCall(sqlite3_vfs *pVfs, const char *zName) {
    return NULL;
}

static sqlite3_vfs dummy_vfs = {
    3,                           /* iVersion */
    sizeof(sqlite3_file),        /* szOsFile */
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

/* Helper function to write data to dummy file */
static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Use first byte to control behavior */
    uint8_t control = Data[0];
    size_t data_offset = 1;
    size_t remaining = Size > 1 ? Size - 1 : 0;
    
    /* Test sqlite3_vfs_register */
    int make_default = control & 1;
    sqlite3_vfs_register(&dummy_vfs, make_default);
    
    /* Test sqlite3_vfs_find */
    const char *vfs_name = (control & 2) ? "dummy_vfs" : NULL;
    sqlite3_vfs *found_vfs = sqlite3_vfs_find(vfs_name);
    
    /* Test sqlite3_sleep */
    int sleep_ms = 0;
    if (remaining >= sizeof(int)) {
        memcpy(&sleep_ms, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
        remaining = Size > data_offset ? Size - data_offset : 0;
    } else if (remaining > 0) {
        sleep_ms = Data[data_offset];
        data_offset++;
        remaining = Size > data_offset ? Size - data_offset : 0;
    }
    sqlite3_sleep(sleep_ms);
    
    /* Test sqlite3_open and sqlite3_open_v2 */
    sqlite3 *db1 = NULL, *db2 = NULL;
    const char *filename = "./dummy_file";
    
    /* Write remaining data to dummy file for database operations */
    if (remaining > 0) {
        write_to_dummy_file(Data + data_offset, remaining);
    } else {
        /* Write at least something */
        uint8_t dummy = 0;
        write_to_dummy_file(&dummy, 1);
    }
    
    /* Test sqlite3_open */
    int rc1 = sqlite3_open(filename, &db1);
    if (rc1 == SQLITE_OK && db1) {
        /* Test sqlite3_file_control */
        int opcode = 0;
        void *arg = NULL;
        
        if (remaining >= sizeof(int)) {
            memcpy(&opcode, Data + data_offset, sizeof(int));
        }
        
        sqlite3_file_control(db1, "main", opcode, arg);
        sqlite3_file_control(db1, NULL, opcode, arg);
        sqlite3_file_control(db1, "temp", opcode, arg);
        
        sqlite3_close(db1);
    }
    
    /* Test sqlite3_open_v2 */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    const char *use_vfs = (control & 4) ? "dummy_vfs" : NULL;
    
    int rc2 = sqlite3_open_v2(filename, &db2, flags, use_vfs);
    if (rc2 == SQLITE_OK && db2) {
        /* Test sqlite3_file_control with different database names */
        int opcode2 = 0;
        if (remaining >= sizeof(int)) {
            memcpy(&opcode2, Data + data_offset, sizeof(int));
        }
        
        sqlite3_file_control(db2, "main", opcode2, NULL);
        sqlite3_file_control(db2, NULL, opcode2, NULL);
        
        sqlite3_close(db2);
    }
    
    /* Clean up dummy file */
    remove("./dummy_file");
    
    /* Unregister dummy VFS to avoid affecting subsequent runs */
    sqlite3_vfs_unregister(&dummy_vfs);
    
    return 0;
}