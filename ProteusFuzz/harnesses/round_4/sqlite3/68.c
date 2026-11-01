// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_os_init at sqlite3.c:47794:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 VFS and initialization APIs.
 * Target APIs:
 * - sqlite3_vfs_register
 * - sqlite3_os_init
 * - sqlite3_initialize
 * - sqlite3_shutdown
 * - sqlite3_sleep
 * - sqlite3_randomness
 */

/* Static storage for VFS to prevent use-after-free when registered */
static sqlite3_vfs g_dummy_vfs;
static int g_vfs_initialized = 0;

static int dummy_xOpen(sqlite3_vfs *p, const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags) {
    (void)p;
    (void)zName;
    (void)flags;
    (void)pOutFlags;
    if (pFile) {
        pFile->pMethods = NULL;
    }
    return SQLITE_OK;
}

static int dummy_xDelete(sqlite3_vfs *p, const char *zName, int syncDir) {
    (void)p;
    (void)zName;
    (void)syncDir;
    return SQLITE_OK;
}

static int dummy_xAccess(sqlite3_vfs *p, const char *zName, int flags, int *pResOut) {
    (void)p;
    (void)zName;
    (void)flags;
    if (pResOut) *pResOut = 0;
    return SQLITE_OK;
}

static int dummy_xFullPathname(sqlite3_vfs *p, const char *zName, int nOut, char *zOut) {
    (void)p;
    (void)zName;
    (void)nOut;
    (void)zOut;
    return SQLITE_OK;
}

static void *dummy_xDlOpen(sqlite3_vfs *p, const char *zFilename) {
    (void)p;
    (void)zFilename;
    return NULL;
}

static void dummy_xDlError(sqlite3_vfs *p, int nByte, char *zErrMsg) {
    (void)p;
    (void)nByte;
    (void)zErrMsg;
}

static void (*dummy_xDlSym_impl(sqlite3_vfs *pVfs, void *pHandle, const char *zSymbol))(void) {
    (void)pVfs;
    (void)pHandle;
    (void)zSymbol;
    return NULL;
}

static void dummy_xDlClose(sqlite3_vfs *pVfs, void *pHandle) {
    (void)pVfs;
    (void)pHandle;
}

static int dummy_xRandomness(sqlite3_vfs *p, int nByte, char *zOut) {
    (void)p;
    (void)nByte;
    (void)zOut;
    return 0;
}

static int dummy_xSleep(sqlite3_vfs *p, int microseconds) {
    (void)p;
    (void)microseconds;
    return 0;
}

static int dummy_xCurrentTime(sqlite3_vfs *p, double *pTime) {
    (void)p;
    (void)pTime;
    return SQLITE_OK;
}

static int dummy_xGetLastError(sqlite3_vfs *p, int nBuf, char *zBuf) {
    (void)p;
    (void)nBuf;
    (void)zBuf;
    return SQLITE_OK;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs *p, sqlite3_int64 *piTime) {
    (void)p;
    (void)piTime;
    return SQLITE_OK;
}

static int dummy_xSetSystemCall(sqlite3_vfs *pVfs, const char *zName, sqlite3_syscall_ptr pFunc) {
    (void)pVfs;
    (void)zName;
    (void)pFunc;
    return SQLITE_OK;
}

static sqlite3_syscall_ptr dummy_xGetSystemCall(sqlite3_vfs *p, const char *zName) {
    (void)p;
    (void)zName;
    return NULL;
}

static const char *dummy_xNextSystemCall(sqlite3_vfs *p, const char *zName) {
    (void)p;
    (void)zName;
    return NULL;
}

static void init_dummy_vfs(const char *name) {
    memset(&g_dummy_vfs, 0, sizeof(g_dummy_vfs));
    g_dummy_vfs.iVersion = 3;
    g_dummy_vfs.szOsFile = sizeof(sqlite3_file);
    g_dummy_vfs.mxPathname = 1024;
    g_dummy_vfs.zName = name;
    g_dummy_vfs.pAppData = NULL;

    g_dummy_vfs.xOpen = dummy_xOpen;
    g_dummy_vfs.xDelete = dummy_xDelete;
    g_dummy_vfs.xAccess = dummy_xAccess;
    g_dummy_vfs.xFullPathname = dummy_xFullPathname;
    g_dummy_vfs.xDlOpen = dummy_xDlOpen;
    g_dummy_vfs.xDlError = dummy_xDlError;
    g_dummy_vfs.xDlSym = dummy_xDlSym_impl;
    g_dummy_vfs.xDlClose = dummy_xDlClose;
    g_dummy_vfs.xRandomness = dummy_xRandomness;
    g_dummy_vfs.xSleep = dummy_xSleep;
    g_dummy_vfs.xCurrentTime = dummy_xCurrentTime;
    g_dummy_vfs.xGetLastError = dummy_xGetLastError;
    g_dummy_vfs.xCurrentTimeInt64 = dummy_xCurrentTimeInt64;
    g_dummy_vfs.xSetSystemCall = dummy_xSetSystemCall;
    g_dummy_vfs.xGetSystemCall = dummy_xGetSystemCall;
    g_dummy_vfs.xNextSystemCall = dummy_xNextSystemCall;
    g_vfs_initialized = 1;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure we have some data to work with */
    int op = Data[0] % 6;

    /* Initialize SQLite once per fuzz run to avoid state leakage issues */
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Call sqlite3_os_init explicitly as per target API */
    rc = sqlite3_os_init();
    (void)rc;

    switch (op) {
        case 0: {
            /* Test sqlite3_vfs_register */
            static char vfs_name[32];
            /* Safe access: check size before accessing Data[1] */
            int val = 0;
            if (Size > 1) {
                val = (int)(Data[1] % 1000);
            }
            snprintf(vfs_name, sizeof(vfs_name), "fuzz_vfs_%d", val);
            
            /* Initialize the static VFS structure */
            init_dummy_vfs(vfs_name);
            
            int makeDflt = 0;
            if (Size > 1) {
                makeDflt = (Data[1] & 0x01);
            }
            rc = sqlite3_vfs_register(&g_dummy_vfs, makeDflt);
            (void)rc;
            break;
        }
        case 1: {
            /* Test sqlite3_shutdown and re-initialize */
            rc = sqlite3_shutdown();
            (void)rc;
            rc = sqlite3_initialize();
            (void)rc;
            break;
        }
        case 2: {
            /* Test sqlite3_sleep */
            int ms = 0;
            if (Size > 1) {
                ms = (Data[1] % 100);
            }
            /* Also test negative values which should be treated as 0 in newer versions */
            if (Size > 2 && (Data[2] & 0x01)) {
                ms = -ms;
            }
            rc = sqlite3_sleep(ms);
            (void)rc;
            break;
        }
        case 3: {
            /* Test sqlite3_randomness with seeding (NULL buffer) */
            sqlite3_randomness(0, NULL);
            break;
        }
        case 4: {
            /* Test sqlite3_randomness with generation */
            int n = 1;
            if (Size > 1) {
                n = (Data[1] % 1024) + 1;
            }
            unsigned char *buf = (unsigned char *)malloc(n);
            if (buf) {
                sqlite3_randomness(n, buf);
                free(buf);
            }
            break;
        }
        case 5: {
            /* Mixed operations */
            static char vfs_name_mixed[32];
            int val_mixed = 0;
            if (Size > 1) {
                val_mixed = (int)(Data[1] % 1000);
            }
            snprintf(vfs_name_mixed, sizeof(vfs_name_mixed), "fuzz_vfs_mixed_%d", val_mixed);
            
            init_dummy_vfs(vfs_name_mixed);
            
            rc = sqlite3_vfs_register(&g_dummy_vfs, 0);
            (void)rc;
            
            sqlite3_randomness(16, NULL);
            sqlite3_sleep(1);
            break;
        }
    }

    /* Cleanup: shutdown to reset state for next run */
    rc = sqlite3_shutdown();
    (void)rc;

    return 0;
}