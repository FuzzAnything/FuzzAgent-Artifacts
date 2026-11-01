// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Custom VFS implementation for fuzzing purposes.
 * Most methods return errors or no-ops to prevent actual I/O.
 */
static int fuzz_vfs_xOpen(sqlite3_vfs *p, const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags) {
    (void)p;
    (void)zName;
    (void)pFile;
    (void)flags;
    (void)pOutFlags;
    return SQLITE_IOERR;
}

static int fuzz_vfs_xDelete(sqlite3_vfs *p, const char *zName, int syncDir) {
    (void)p;
    (void)zName;
    (void)syncDir;
    return SQLITE_IOERR;
}

static int fuzz_vfs_xAccess(sqlite3_vfs *p, const char *zName, int flags, int *pResOut) {
    (void)p;
    (void)zName;
    (void)flags;
    (void)pResOut;
    return SQLITE_IOERR;
}

static int fuzz_vfs_xFullPathname(sqlite3_vfs *p, const char *zName, int nOut, char *zOut) {
    (void)p;
    (void)zName;
    (void)nOut;
    (void)zOut;
    return SQLITE_IOERR;
}

static void *fuzz_vfs_xDlOpen(sqlite3_vfs *p, const char *zFilename) {
    (void)p;
    (void)zFilename;
    return NULL;
}

static void fuzz_vfs_xDlError(sqlite3_vfs *p, int nByte, char *zErrMsg) {
    (void)p;
    (void)nByte;
    (void)zErrMsg;
}

static void (*fuzz_vfs_xDlSym(sqlite3_vfs *pVfs, void *pHandle, const char *zSymbol))(void) {
    (void)pVfs;
    (void)pHandle;
    (void)zSymbol;
    return NULL;
}

static void fuzz_vfs_xDlClose(sqlite3_vfs *pVfs, void *pHandle) {
    (void)pVfs;
    (void)pHandle;
}

static int fuzz_vfs_xRandomness(sqlite3_vfs *p, int nByte, char *zOut) {
    (void)p;
    (void)nByte;
    (void)zOut;
    return 0;
}

static int fuzz_vfs_xSleep(sqlite3_vfs *p, int microseconds) {
    (void)p;
    (void)microseconds;
    return 0;
}

static int fuzz_vfs_xCurrentTime(sqlite3_vfs *p, double *pTime) {
    (void)p;
    (void)pTime;
    return SQLITE_IOERR;
}

static int fuzz_vfs_xGetLastError(sqlite3_vfs *p, int nBuf, char *zBuf) {
    (void)p;
    (void)nBuf;
    (void)zBuf;
    return SQLITE_IOERR;
}

static int fuzz_vfs_xCurrentTimeInt64(sqlite3_vfs *p, sqlite3_int64 *pTime) {
    (void)p;
    (void)pTime;
    return SQLITE_IOERR;
}

static int fuzz_vfs_xSetSystemCall(sqlite3_vfs *pVfs, const char *zName, sqlite3_syscall_ptr pFunc) {
    (void)pVfs;
    (void)zName;
    (void)pFunc;
    return SQLITE_IOERR;
}

static sqlite3_syscall_ptr fuzz_vfs_xGetSystemCall(sqlite3_vfs *p, const char *zName) {
    (void)p;
    (void)zName;
    return NULL;
}

static const char *fuzz_vfs_xNextSystemCall(sqlite3_vfs *p, const char *zName) {
    (void)p;
    (void)zName;
    return NULL;
}

/*
 * Helper to create a VFS structure from input data.
 * Returns NULL if input is too small or invalid.
 */
static sqlite3_vfs *create_fuzz_vfs(const uint8_t *Data, size_t Size) {
    if (Size < 16) return NULL;

    static sqlite3_vfs vfs_instance;
    memset(&vfs_instance, 0, sizeof(vfs_instance));

    vfs_instance.iVersion = 3;
    vfs_instance.szOsFile = sizeof(sqlite3_file);
    vfs_instance.mxPathname = 1024;
    vfs_instance.pNext = NULL;
    vfs_instance.pAppData = NULL;

    /* Use input data to influence the name, ensuring it's valid */
    static char name_buf[64];
    size_t name_len = (Size > 60) ? 60 : Size;
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';
    vfs_instance.zName = name_buf;

    /* Assign function pointers */
    vfs_instance.xOpen = fuzz_vfs_xOpen;
    vfs_instance.xDelete = fuzz_vfs_xDelete;
    vfs_instance.xAccess = fuzz_vfs_xAccess;
    vfs_instance.xFullPathname = fuzz_vfs_xFullPathname;
    vfs_instance.xDlOpen = fuzz_vfs_xDlOpen;
    vfs_instance.xDlError = fuzz_vfs_xDlError;
    vfs_instance.xDlSym = fuzz_vfs_xDlSym;
    vfs_instance.xDlClose = fuzz_vfs_xDlClose;
    vfs_instance.xRandomness = fuzz_vfs_xRandomness;
    vfs_instance.xSleep = fuzz_vfs_xSleep;
    vfs_instance.xCurrentTime = fuzz_vfs_xCurrentTime;
    vfs_instance.xGetLastError = fuzz_vfs_xGetLastError;
    vfs_instance.xCurrentTimeInt64 = fuzz_vfs_xCurrentTimeInt64;
    vfs_instance.xSetSystemCall = fuzz_vfs_xSetSystemCall;
    vfs_instance.xGetSystemCall = fuzz_vfs_xGetSystemCall;
    vfs_instance.xNextSystemCall = fuzz_vfs_xNextSystemCall;

    return &vfs_instance;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize SQLite */
    sqlite3_initialize();

    /* Create a custom VFS from input */
    sqlite3_vfs *pVfs = create_fuzz_vfs(Data, Size);
    if (!pVfs) return 0;

    /* Fuzz sqlite3_vfs_register */
    int makeDflt = (Size > 0) ? (Data[0] % 2) : 0;
    sqlite3_vfs_register(pVfs, makeDflt);

    /* Fuzz sqlite3_vfs_find with various names */
    sqlite3_vfs *found = sqlite3_vfs_find(NULL); /* Default */
    (void)found;
    
    if (Size > 10) {
        /* Try to find with a name derived from input */
        char search_name[11];
        memcpy(search_name, Data + 1, 10);
        search_name[10] = '\0';
        found = sqlite3_vfs_find(search_name);
        (void)found;
    }

    /* Fuzz sqlite3_sleep with various durations */
    int sleep_ms = 0;
    if (Size > 1) {
        sleep_ms = (int)(Data[1] % 100);
    }
    /* Also try negative and large values */
    if (Size > 2) {
        int val = (int)(Data[2] - 128); /* Can be negative */
        sqlite3_sleep(val);
    }
    sqlite3_sleep(sleep_ms);

    /* Fuzz sqlite3_randomness */
    int rand_n = 0;
    if (Size > 3) {
        rand_n = (int)(Data[3] % 256);
    }
    if (rand_n > 0) {
        char *buf = (char *)malloc(rand_n);
        if (buf) {
            sqlite3_randomness(rand_n, buf);
            free(buf);
        }
    } else {
        /* Seed with NULL */
        sqlite3_randomness(0, NULL);
    }

    /* Fuzz sqlite3_vfs_unregister */
    sqlite3_vfs_unregister(pVfs);

    /* Cleanup: Shutdown SQLite */
    sqlite3_shutdown();

    return 0;
}