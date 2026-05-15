// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:13289:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:13317:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:172794:16 in sqlite3.h
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

/* Dummy VFS for fuzzing */
static int dummy_xOpen(sqlite3_vfs* vfs, const char *zName, sqlite3_file *pFile,
                       int flags, int *pOutFlags) {
    return SQLITE_CANTOPEN;
}

static int dummy_xDelete(sqlite3_vfs* vfs, const char *zName, int syncDir) {
    return SQLITE_OK;
}

static int dummy_xAccess(sqlite3_vfs* vfs, const char *zName, int flags, int *pResOut) {
    *pResOut = 0;
    return SQLITE_OK;
}

static int dummy_xFullPathname(sqlite3_vfs* vfs, const char *zName, int nOut, char *zOut) {
    if (nOut > 0) {
        zOut[0] = '\0';
    }
    return SQLITE_OK;
}

static void* dummy_xDlOpen(sqlite3_vfs* vfs, const char *zFilename) {
    return NULL;
}

static void dummy_xDlError(sqlite3_vfs* vfs, int nByte, char *zErrMsg) {
    if (nByte > 0) {
        zErrMsg[0] = '\0';
    }
}

static void (*dummy_xDlSym(sqlite3_vfs* vfs, void *pHandle, const char *zSymbol))(void) {
    return NULL;
}

static void dummy_xDlClose(sqlite3_vfs* vfs, void *pHandle) {
}

static int dummy_xRandomness(sqlite3_vfs* vfs, int nByte, char *zOut) {
    return 0;
}

static int dummy_xSleep(sqlite3_vfs* vfs, int microseconds) {
    return 0;
}

static int dummy_xCurrentTime(sqlite3_vfs* vfs, double *pTime) {
    *pTime = 0.0;
    return SQLITE_OK;
}

static int dummy_xGetLastError(sqlite3_vfs* vfs, int nByte, char *zErrMsg) {
    return 0;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs* vfs, sqlite3_int64 *pTime) {
    *pTime = 0;
    return SQLITE_OK;
}

static sqlite3_vfs dummy_vfs = {
    3,                    /* iVersion */
    0,                    /* szOsFile */
    1024,                 /* mxPathname */
    NULL,                 /* pNext */
    "dummy_vfs",          /* zName */
    NULL,                 /* pAppData */
    dummy_xOpen,          /* xOpen */
    dummy_xDelete,        /* xDelete */
    dummy_xAccess,        /* xAccess */
    dummy_xFullPathname,  /* xFullPathname */
    dummy_xDlOpen,        /* xDlOpen */
    dummy_xDlError,       /* xDlError */
    dummy_xDlSym,         /* xDlSym */
    dummy_xDlClose,       /* xDlClose */
    dummy_xRandomness,    /* xRandomness */
    dummy_xSleep,         /* xSleep */
    dummy_xCurrentTime,   /* xCurrentTime */
    dummy_xGetLastError,  /* xGetLastError */
    dummy_xCurrentTimeInt64, /* xCurrentTimeInt64 */
    NULL,                 /* xSetSystemCall */
    NULL,                 /* xGetSystemCall */
    NULL                  /* xNextSystemCall */
};

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    /* 1. Test sqlite3_sleep */
    if (Size >= sizeof(int)) {
        int sleep_ms;
        memcpy(&sleep_ms, Data, sizeof(int));
        sqlite3_sleep(sleep_ms);
    }

    /* 2. Test sqlite3_vfs_register */
    int makeDflt = 0;
    if (Size > 0) {
        makeDflt = Data[0] & 1;
    }
    sqlite3_vfs_register(&dummy_vfs, makeDflt);
    sqlite3_vfs_unregister(&dummy_vfs);

    /* 3. Test sqlite3_is_interrupted */
    sqlite3 *db1 = NULL;
    if (sqlite3_open(":memory:", &db1) == SQLITE_OK) {
        sqlite3_is_interrupted(db1);
        cleanup_database(db1);
    }

    /* 4. Test sqlite3_open */
    sqlite3 *db2 = NULL;
    /* Use the first min(Size, 255) bytes as filename */
    char filename[256] = "./dummy_file";
    if (Size > 0) {
        size_t len = Size < 255 ? Size : 255;
        memcpy(filename, Data, len);
        filename[len] = '\0';
    }
    
    /* Write dummy data to file if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size < 1024 ? Size : 1024, fp);
        }
        fclose(fp);
    }
    
    int rc_open = sqlite3_open(filename, &db2);
    if (rc_open == SQLITE_OK) {
        cleanup_database(db2);
    }

    /* 5. Test sqlite3_open_v2 */
    sqlite3 *db3 = NULL;
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size >= sizeof(int)) {
        memcpy(&flags, Data, sizeof(int));
    }
    
    /* Use different filename variation */
    char filename2[256] = ":memory:";
    if (Size > sizeof(int)) {
        size_t offset = sizeof(int);
        size_t len = (Size - offset) < 255 ? (Size - offset) : 255;
        memcpy(filename2, Data + offset, len);
        filename2[len] = '\0';
    }
    
    int rc_open_v2 = sqlite3_open_v2(filename2, &db3, flags, NULL);
    if (rc_open_v2 == SQLITE_OK) {
        /* 6. Test sqlite3_busy_timeout on the opened database */
        int timeout_ms = 0;
        if (Size >= 2 * sizeof(int)) {
            memcpy(&timeout_ms, Data + sizeof(int), sizeof(int));
        }
        sqlite3_busy_timeout(db3, timeout_ms);
        
        /* Also test sqlite3_is_interrupted on this database */
        sqlite3_is_interrupted(db3);
        
        cleanup_database(db3);
    }

    /* Clean up dummy file */
    remove("./dummy_file");

    return 0;
}