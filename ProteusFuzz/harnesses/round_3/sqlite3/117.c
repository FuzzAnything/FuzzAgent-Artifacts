// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for sqlite3 VFS and initialization APIs.
 * Target APIs:
 *   sqlite3_vfs_find
 *   sqlite3_initialize
 *   sqlite3_vfs_register
 *   sqlite3_randomness
 *   sqlite3_vfs_unregister
 *   sqlite3_sleep
 */

/* Helper to safely extract a null-terminated string from fuzz data */
static int extract_string(const uint8_t *data, size_t size, char *out, size_t out_size) {
    if (size == 0) {
        out[0] = '\0';
        return 0;
    }
    size_t len = 0;
    while (len < size && len < out_size - 1) {
        if (data[len] == '\0') break;
        len++;
    }
    memcpy(out, data, len);
    out[len] = '\0';
    return (int)len;
}

/* Helper to extract an integer from fuzz data */
static int extract_int(const uint8_t *data, size_t size) {
    if (size < 4) {
        return 0;
    }
    int val;
    memcpy(&val, data, 4);
    return val;
}

/* Custom VFS implementation for testing */
static sqlite3_vfs g_customVfs = {0};

static int dummy_xOpen(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags) {
    (void)pVfs;
    (void)zName;
    (void)pFile;
    (void)flags;
    (void)pOutFlags;
    return SQLITE_CANTOPEN;
}

static int dummy_xDelete(sqlite3_vfs *pVfs, const char *zName, int syncDir) {
    (void)pVfs;
    (void)zName;
    (void)syncDir;
    return SQLITE_OK;
}

static int dummy_xAccess(sqlite3_vfs *pVfs, const char *zName, int flags, int *pResOut) {
    (void)pVfs;
    (void)zName;
    (void)flags;
    if (pResOut) *pResOut = 0;
    return SQLITE_OK;
}

static int dummy_xFullPathname(sqlite3_vfs *pVfs, const char *zName, int nOut, char *zOut) {
    (void)pVfs;
    (void)zName;
    (void)nOut;
    (void)zOut;
    return SQLITE_CANTOPEN;
}

static int dummy_xRandomness(sqlite3_vfs *pVfs, int nByte, char *zOut) {
    (void)pVfs;
    (void)nByte;
    (void)zOut;
    return 0;
}

static int dummy_xSleep(sqlite3_vfs *pVfs, int microseconds) {
    (void)pVfs;
    (void)microseconds;
    return 0;
}

static int dummy_xCurrentTime(sqlite3_vfs *pVfs, double *pTime) {
    (void)pVfs;
    if (pTime) *pTime = 0.0;
    return SQLITE_OK;
}

static int dummy_xCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *pTime) {
    (void)pVfs;
    if (pTime) *pTime = 0;
    return SQLITE_OK;
}

static void init_custom_vfs(sqlite3_vfs *pVfs) {
    memset(pVfs, 0, sizeof(*pVfs));
    pVfs->iVersion = 3;
    pVfs->szOsFile = sizeof(sqlite3_file);
    pVfs->mxPathname = 512;
    pVfs->zName = "fuzz_vfs";
    pVfs->xOpen = dummy_xOpen;
    pVfs->xDelete = dummy_xDelete;
    pVfs->xAccess = dummy_xAccess;
    pVfs->xFullPathname = dummy_xFullPathname;
    pVfs->xDlOpen = NULL;
    pVfs->xDlError = NULL;
    pVfs->xDlSym = NULL;
    pVfs->xDlClose = NULL;
    pVfs->xRandomness = dummy_xRandomness;
    pVfs->xSleep = dummy_xSleep;
    pVfs->xCurrentTime = dummy_xCurrentTime;
    pVfs->xGetLastError = NULL;
    pVfs->xCurrentTimeInt64 = dummy_xCurrentTimeInt64;
    pVfs->xSetSystemCall = NULL;
    pVfs->xGetSystemCall = NULL;
    pVfs->xNextSystemCall = NULL;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    /* Ensure minimum size for meaningful operations */
    if (Size < 1) {
        return 0;
    }

    /* Initialize SQLite */
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Prepare buffers */
    char vfs_name[256];
    char random_buf[256];
    int make_dflt = 0;
    int sleep_ms = 0;
    int n_random = 0;

    /* Extract data from fuzz input */
    size_t offset = 0;
    if (offset < Size) {
        extract_string(Data + offset, Size - offset, vfs_name, sizeof(vfs_name));
        offset += 10;
        if (offset > Size) offset = Size;
    }

    if (offset < Size) {
        make_dflt = (Data[offset] % 2);
        offset++;
    }

    if (offset < Size) {
        sleep_ms = extract_int(Data + offset, Size - offset);
        offset += 4;
        if (offset > Size) offset = Size;
    }

    if (offset < Size) {
        n_random = extract_int(Data + offset, Size - offset);
        if (n_random < 0) n_random = 0;
        if (n_random > (int)sizeof(random_buf)) n_random = (int)sizeof(random_buf);
        offset += 4;
        if (offset > Size) offset = Size;
    }

    /* Test sqlite3_vfs_find with various names */
    sqlite3_vfs *pVfs = NULL;

    /* Case 1: NULL name (default VFS) */
    pVfs = sqlite3_vfs_find(NULL);
    (void)pVfs;

    /* Case 2: Extracted name */
    pVfs = sqlite3_vfs_find(vfs_name);
    (void)pVfs;

    /* Case 3: Empty string */
    vfs_name[0] = '\0';
    pVfs = sqlite3_vfs_find(vfs_name);
    (void)pVfs;

    /* Case 4: Special characters */
    if (Size > 20) {
        char special_name[256];
        extract_string(Data + 20, Size - 20, special_name, sizeof(special_name));
        pVfs = sqlite3_vfs_find(special_name);
        (void)pVfs;
    }

    /* Initialize custom VFS and register it */
    init_custom_vfs(&g_customVfs);
    rc = sqlite3_vfs_register(&g_customVfs, make_dflt);
    if (rc == SQLITE_OK) {
        /* Verify registration */
        pVfs = sqlite3_vfs_find("fuzz_vfs");
        (void)pVfs;

        /* Test sqlite3_randomness */
        if (n_random > 0) {
            sqlite3_randomness(n_random, random_buf);
        } else {
            /* Seed the PRNG */
            sqlite3_randomness(0, NULL);
        }

        /* Test sqlite3_sleep */
        if (sleep_ms > 0 && sleep_ms < 1000) {
            int actual = sqlite3_sleep(sleep_ms);
            (void)actual;
        }

        /* Unregister the custom VFS */
        rc = sqlite3_vfs_unregister(&g_customVfs);
        (void)rc;

        /* Verify unregistration */
        pVfs = sqlite3_vfs_find("fuzz_vfs");
        if (pVfs != NULL) {
            /* This should not happen, but we handle it gracefully */
        }
    }

    /* Test sqlite3_randomness with NULL buffer (seeding) */
    sqlite3_randomness(0, NULL);

    /* Test sqlite3_randomness with valid buffer */
    if (Size > 50) {
        int n = (int)(Size - 50);
        if (n > 100) n = 100;
        sqlite3_randomness(n, random_buf);
    }

    /* Test sqlite3_sleep with edge cases */
    sqlite3_sleep(0);
    sqlite3_sleep(-1);
    sqlite3_sleep(1);

    /* Cleanup: Shutdown SQLite */
    sqlite3_shutdown();

    return 0;
}