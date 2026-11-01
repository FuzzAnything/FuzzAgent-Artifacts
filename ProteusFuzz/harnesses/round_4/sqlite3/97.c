// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:189742:16 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_vfs_register at sqlite3.c:27296:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
// sqlite3_vfs_unregister at sqlite3.c:27324:16 in sqlite3.h
// sqlite3_vfs_find at sqlite3.c:27250:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 VFS and utility functions.
 * Targets:
 * 1. sqlite3_vfs_unregister
 * 2. sqlite3_vfs_find
 * 3. sqlite3_vfs_register
 * 4. sqlite3_initialize
 * 5. sqlite3_sleep
 * 6. sqlite3_randomness
 */

/* Static helper to create a dummy VFS structure for registration */
static sqlite3_vfs *create_dummy_vfs(const char *name) {
    sqlite3_vfs *vfs = (sqlite3_vfs *)malloc(sizeof(sqlite3_vfs));
    if (!vfs) return NULL;

    memset(vfs, 0, sizeof(sqlite3_vfs));
    vfs->iVersion = 3;
    vfs->szOsFile = 0;
    vfs->mxPathname = 1024;
    vfs->zName = name;
    vfs->pAppData = NULL;
    /* Function pointers are left NULL for this fuzzing scenario as we 
       are testing the registration/list management logic, not the 
       actual file I/O implementation. */
    
    return vfs;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize SQLite library. This is idempotent but good practice. */
    sqlite3_initialize();

    /* 
     * Strategy: 
     * Use the input data to drive the logic of which API to call and 
     * what parameters to use.
     */

    /* Ensure we have enough data for basic operations */
    if (Size < 1) return 0;

    int op = Data[0] % 6; /* Select one of the 6 target functions */
    
    /* Create a dummy VFS for operations that require one */
    sqlite3_vfs *dummy_vfs = NULL;
    char vfs_name[32];
    
    /* Generate a unique-ish name based on input to avoid collisions in single run,
       though in fuzzing we might hit collisions intentionally. */
    snprintf(vfs_name, sizeof(vfs_name), "fuzz_vfs_%d", Data[0]);
    
    if (op == 0 || op == 2 || op == 3) {
        dummy_vfs = create_dummy_vfs(vfs_name);
    }

    switch (op) {
        case 0: {
            /* Test sqlite3_vfs_register */
            if (dummy_vfs) {
                int makeDflt = 0;
                if (Size > 1) {
                    makeDflt = (Data[1] % 2);
                }
                sqlite3_vfs_register(dummy_vfs, makeDflt);
                
                /* Try to find it immediately */
                sqlite3_vfs *found = sqlite3_vfs_find(dummy_vfs->zName);
                (void)found;
                
                /* Unregister before freeing to prevent use-after-free in global list */
                sqlite3_vfs_unregister(dummy_vfs);
                
                /* Cleanup */
                free(dummy_vfs);
            }
            break;
        }
        case 1: {
            /* Test sqlite3_vfs_find */
            /* Try finding with NULL (default) */
            sqlite3_vfs *def = sqlite3_vfs_find(NULL);
            (void)def;
            
            /* Try finding with a name from input if available */
            if (Size > 2) {
                /* Construct a string from input, ensuring null termination */
                size_t len = (Size - 2) < 30 ? (Size - 2) : 30;
                char name_buf[32];
                memcpy(name_buf, &Data[1], len);
                name_buf[len] = '\0';
                
                sqlite3_vfs *found = sqlite3_vfs_find(name_buf);
                (void)found;
            }
            break;
        }
        case 2: {
            /* Test sqlite3_vfs_unregister */
            if (dummy_vfs) {
                /* Register first to ensure it exists in the list */
                sqlite3_vfs_register(dummy_vfs, 0);
                /* Then unregister */
                sqlite3_vfs_unregister(dummy_vfs);
                
                /* Try to unregister again (should be safe/no-op or error) */
                sqlite3_vfs_unregister(dummy_vfs);
                
                /* Cleanup */
                free(dummy_vfs);
            } else {
                /* Unregister NULL or invalid pointer behavior check */
                sqlite3_vfs_unregister(NULL);
            }
            break;
        }
        case 3: {
            /* Test sqlite3_vfs_register with potential collisions or edge cases */
            if (dummy_vfs) {
                /* Register as default */
                sqlite3_vfs_register(dummy_vfs, 1);
                /* Register again (idempotent) */
                sqlite3_vfs_register(dummy_vfs, 0);
                
                /* Unregister before freeing */
                sqlite3_vfs_unregister(dummy_vfs);
                
                /* Cleanup */
                free(dummy_vfs);
            }
            break;
        }
        case 4: {
            /* Test sqlite3_sleep */
            int ms = 0;
            if (Size > 1) {
                ms = Data[1];
                /* Ensure we don't sleep too long in fuzzing */
                if (ms > 100) ms = 100;
                if (ms < 0) ms = 0; /* Handle negative as per docs */
            }
            sqlite3_sleep(ms);
            break;
        }
        case 5: {
            /* Test sqlite3_randomness */
            int n = 0;
            void *buf = NULL;
            
            if (Size > 1) {
                n = Data[1];
                /* Limit buffer size to avoid OOM */
                if (n > 1024) n = 1024;
                if (n > 0) {
                    buf = malloc(n);
                    if (buf) {
                        sqlite3_randomness(n, buf);
                        free(buf);
                    }
                }
            }
            
            /* Test seeding with NULL buffer */
            sqlite3_randomness(0, NULL);
            break;
        }
        default:
            break;
    }

    return 0;
}