// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:13243:25 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
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
    if (zErrMsg && nByte > 0) {
        strncpy(zErrMsg, "dummy", nByte - 1);
        zErrMsg[nByte - 1] = '\0';
    }
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
    return 0;
}

static int dummy_xCurrentTime(sqlite3_vfs *pVfs, double *pTime) {
    if (pTime) *pTime = 0.0;
    return SQLITE_OK;
}

static int dummy_xGetLastError(sqlite3_vfs *pVfs, int nBuf, char *zBuf) {
    return 0;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *pTime) {
    if (pTime) *pTime = 0;
    return SQLITE_OK;
}

static sqlite3_vfs dummy_vfs = {
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
    NULL,                        /* xSetSystemCall */
    NULL,                        /* xGetSystemCall */
    NULL                         /* xNextSystemCall */
};

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Use first byte to determine which functions to test */
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    /* Test sqlite3_vfs_register */
    if (selector & 0x01) {
        int makeDflt = (Size > offset) ? (Data[offset] & 0x01) : 0;
        offset = (offset + 1) % Size;
        sqlite3_vfs_register(&dummy_vfs, makeDflt);
    }
    
    /* Test sqlite3_sleep */
    if (selector & 0x02) {
        int sleep_ms = 0;
        if (Size - offset >= 4) {
            sleep_ms = *(int*)(Data + offset);
            offset = (offset + 4) % Size;
        } else if (Size - offset >= 1) {
            sleep_ms = Data[offset];
            offset = (offset + 1) % Size;
        }
        sqlite3_sleep(sleep_ms);
    }
    
    /* Test sqlite3_open */
    if (selector & 0x04) {
        sqlite3 *db = NULL;
        const char *filename = NULL;
        
        if (Size - offset > 0) {
            /* Use input data as filename, but limit to reasonable length */
            size_t name_len = (Size - offset > 255) ? 255 : (Size - offset);
            char *temp_name = malloc(name_len + 1);
            if (temp_name) {
                memcpy(temp_name, Data + offset, name_len);
                temp_name[name_len] = '\0';
                filename = temp_name;
                offset = (offset + name_len) % Size;
            }
        }
        
        if (!filename) {
            filename = ":memory:";
        }
        
        int rc = sqlite3_open(filename, &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
        
        if (filename && filename != ":memory:") {
            free((void*)filename);
        }
    }
    
    /* Test sqlite3_vfs_unregister */
    if (selector & 0x08) {
        sqlite3_vfs_unregister(&dummy_vfs);
    }
    
    /* Test sqlite3_vfs_find */
    if (selector & 0x10) {
        const char *vfs_name = NULL;
        
        if (Size - offset > 0) {
            size_t name_len = (Size - offset > 255) ? 255 : (Size - offset);
            char *temp_name = malloc(name_len + 1);
            if (temp_name) {
                memcpy(temp_name, Data + offset, name_len);
                temp_name[name_len] = '\0';
                vfs_name = temp_name;
                offset = (offset + name_len) % Size;
            }
        }
        
        sqlite3_vfs *found_vfs = sqlite3_vfs_find(vfs_name);
        (void)found_vfs; /* Ignore result */
        
        if (vfs_name) {
            free((void*)vfs_name);
        }
    }
    
    /* Test sqlite3_open_v2 */
    if (selector & 0x20) {
        sqlite3 *db = NULL;
        const char *filename = NULL;
        int flags = 0;
        const char *vfs_name = NULL;
        
        /* Extract filename */
        if (Size - offset > 0) {
            size_t name_len = (Size - offset > 127) ? 127 : (Size - offset);
            char *temp_name = malloc(name_len + 1);
            if (temp_name) {
                memcpy(temp_name, Data + offset, name_len);
                temp_name[name_len] = '\0';
                filename = temp_name;
                offset = (offset + name_len) % Size;
            }
        }
        
        /* Extract flags */
        if (Size - offset >= 4) {
            flags = *(int*)(Data + offset);
            offset = (offset + 4) % Size;
        } else if (Size - offset >= 1) {
            flags = Data[offset];
            offset = (offset + 1) % Size;
        }
        
        /* Extract VFS name */
        if (Size - offset > 0) {
            size_t vfs_len = (Size - offset > 127) ? 127 : (Size - offset);
            char *temp_vfs = malloc(vfs_len + 1);
            if (temp_vfs) {
                memcpy(temp_vfs, Data + offset, vfs_len);
                temp_vfs[vfs_len] = '\0';
                vfs_name = temp_vfs;
                offset = (offset + vfs_len) % Size;
            }
        }
        
        if (!filename) {
            filename = ":memory:";
        }
        
        int rc = sqlite3_open_v2(filename, &db, flags, vfs_name);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
        
        if (filename && filename != ":memory:") {
            free((void*)filename);
        }
        if (vfs_name) {
            free((void*)vfs_name);
        }
    }
    
    return 0;
}