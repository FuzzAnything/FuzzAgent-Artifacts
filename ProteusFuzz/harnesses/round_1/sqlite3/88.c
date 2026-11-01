// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
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
 * Static helper to create a minimal dummy VFS for fuzzing.
 * This VFS does not implement real I/O, but provides enough
 * structure to allow registration and unregistration without crashing.
 */
static sqlite3_vfs *create_dummy_vfs(const char *name) {
    sqlite3_vfs *vfs = (sqlite3_vfs *)malloc(sizeof(sqlite3_vfs));
    if (!vfs) return NULL;

    memset(vfs, 0, sizeof(sqlite3_vfs));

    vfs->iVersion = 3;
    vfs->szOsFile = sizeof(sqlite3_file);
    vfs->mxPathname = 1024;
    vfs->zName = name;
    vfs->pAppData = NULL;

    /* All function pointers are NULL to indicate no-op or unsupported */
    vfs->xOpen = NULL;
    vfs->xDelete = NULL;
    vfs->xAccess = NULL;
    vfs->xFullPathname = NULL;
    vfs->xDlOpen = NULL;
    vfs->xDlError = NULL;
    vfs->xDlSym = NULL;
    vfs->xDlClose = NULL;
    vfs->xRandomness = NULL;
    vfs->xSleep = NULL;
    vfs->xCurrentTime = NULL;
    vfs->xGetLastError = NULL;
    vfs->xCurrentTimeInt64 = NULL;
    vfs->xSetSystemCall = NULL;
    vfs->xGetSystemCall = NULL;
    vfs->xNextSystemCall = NULL;

    return vfs;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    /* Ensure minimum size to avoid trivial inputs */
    if (Size < 1) {
        return 0;
    }

    /* Initialize SQLite */
    int rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    uint8_t *data = (uint8_t *)Data;
    size_t len = Size;

    /*
     * Strategy:
     * 1. Parse a name from the input to register a custom VFS.
     * 2. Register the VFS with varying makeDflt flags.
     * 3. Attempt to find the VFS.
     * 4. Unregister the VFS.
     * 5. Test sqlite3_sleep and sqlite3_randomness.
     */

    /* Extract a potential VFS name (null-terminated string) */
    char vfs_name[256];
    size_t name_len = 0;
    while (name_len < len && name_len < sizeof(vfs_name) - 1 && data[name_len] != 0) {
        vfs_name[name_len] = (char)data[name_len];
        name_len++;
    }
    vfs_name[name_len] = '\0';

    /* If the name is empty, use a default name */
    if (name_len == 0) {
        strcpy(vfs_name, "fuzz_vfs");
    }

    /* Create a dummy VFS */
    sqlite3_vfs *custom_vfs = create_dummy_vfs(vfs_name);
    if (!custom_vfs) {
        return 0;
    }

    /* Determine makeDflt flag from input */
    int make_dflt = (len > 0 && data[0] % 2 == 0) ? 1 : 0;

    /* Register the custom VFS */
    int reg_rc = sqlite3_vfs_register(custom_vfs, make_dflt);
    (void)reg_rc; /* Ignore return value for fuzzing stability */

    /* Try to find the VFS by name */
    sqlite3_vfs *found_vfs = sqlite3_vfs_find(vfs_name);
    if (found_vfs) {
        /* Verify it matches our custom VFS */
        if (found_vfs == custom_vfs) {
            /* Success path */
        }
    }

    /* Try to find with NULL (should return default) */
    sqlite3_vfs *default_vfs = sqlite3_vfs_find(NULL);
    (void)default_vfs;

    /* Try to find a non-existent VFS */
    char non_exist_name[256];
    snprintf(non_exist_name, sizeof(non_exist_name), "%s_non_exist", vfs_name);
    sqlite3_vfs *not_found = sqlite3_vfs_find(non_exist_name);
    (void)not_found;

    /* Unregister the custom VFS */
    int unreg_rc = sqlite3_vfs_unregister(custom_vfs);
    (void)unreg_rc;

    /* Verify it's no longer found */
    sqlite3_vfs *after_unreg = sqlite3_vfs_find(vfs_name);
    (void)after_unreg;

    /* Test sqlite3_sleep with various values derived from input */
    if (len > 1) {
        int sleep_ms = (int)(data[1] % 1000);
        sqlite3_sleep(sleep_ms);
    }
    if (len > 2) {
        int sleep_ms_neg = (int)(data[2] % 1000) - 500; /* Can be negative */
        sqlite3_sleep(sleep_ms_neg);
    }

    /* Test sqlite3_randomness */
    char rand_buf[64];
    if (len > 3) {
        int rand_len = (int)(data[3] % 64);
        if (rand_len > 0) {
            sqlite3_randomness(rand_len, rand_buf);
        }
    }
    /* Seed randomness (N <= 0 or NULL) */
    sqlite3_randomness(-1, NULL);

    /* Cleanup */
    free(custom_vfs);

    /* Shutdown SQLite (optional, but good for state reset) */
    sqlite3_shutdown();

    return 0;
}