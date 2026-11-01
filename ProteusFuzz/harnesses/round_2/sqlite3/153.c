// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_config at sqlite3.c:186059:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 configuration and connection management APIs.
 * Target APIs:
 * - sqlite3_enable_load_extension
 * - sqlite3_db_config
 * - sqlite3_config
 * - sqlite3_reset_auto_extension
 * - sqlite3_total_changes
 * - sqlite3_close_v2
 */

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;
    int onoff;
    int op;
    int sz, cnt;
    void *lookasideBuf = NULL;

    /*
     * Initialize a database connection.
     * Use ":memory:" to avoid file I/O issues during fuzzing.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /*
     * Extract fuzzing parameters from input data.
     * We use modulo operations to ensure values are within safe ranges.
     */
    onoff = (Size > 0) ? (Data[0] % 2) : 0;
    op = (Size > 1) ? (Data[1] % 10) : 0; /* Limit op to a small range for safety */
    sz = (Size > 2) ? (Data[2] % 256) : 0;
    cnt = (Size > 3) ? (Data[3] % 256) : 0;

    /*
     * 1. Test sqlite3_enable_load_extension
     * Toggles extension loading on/off.
     */
    rc = sqlite3_enable_load_extension(db, onoff);
    (void)rc; /* Ignore return value for fuzzing state exploration */

    /*
     * 2. Test sqlite3_db_config
     * We focus on SQLITE_DBCONFIG_LOOKASIDE as per the prompt's specific requirement.
     * Ensure we allocate a valid buffer for the lookaside pointer.
     */
    if (op == 0) { /* Use 0 to represent SQLITE_DBCONFIG_LOOKASIDE logic path */
        /*
         * Fix for heap-buffer-overflow:
         * The previous logic used a fixed minimum size which was insufficient for
         * SQLite's internal lookaside structure when 'cnt' > 1.
         * SQLite expects the buffer to hold 'cnt' blocks of size 'sz'.
         * Additionally, SQLite may write metadata or pointers at the end of the buffer.
         * We must ensure the buffer is large enough to hold all requested blocks
         * plus some padding for internal bookkeeping.
         */
        
        /* Ensure sz is non-zero and reasonable */
        int adjustedSz = sz;
        if (adjustedSz == 0) adjustedSz = 16; /* Minimum block size */
        if (adjustedSz > 4096) adjustedSz = 4096; /* Cap block size */

        /* Ensure cnt is non-negative and reasonable */
        int adjustedCnt = cnt;
        if (adjustedCnt < 0) adjustedCnt = 0;
        if (adjustedCnt > 100) adjustedCnt = 100; /* Cap count to prevent OOM */

        /* 
         * Calculate total required size: (cnt * sz) + padding.
         * SQLite's setupLookaside iterates 'cnt' times, writing 'sz' bytes per block.
         * It also writes a pointer at the end of the last block.
         * We add extra padding to be safe.
         */
        size_t totalSize = (size_t)adjustedCnt * (size_t)adjustedSz;
        
        /* Add padding for internal headers/pointers (at least one pointer size + alignment) */
        size_t padding = sizeof(void*) * 2 + 64; 
        totalSize += padding;

        /* Align total size to sizeof(void*) */
        totalSize = ((totalSize + sizeof(void*) - 1) / sizeof(void*)) * sizeof(void*);

        /* Allocate the buffer */
        lookasideBuf = malloc(totalSize);
        if (lookasideBuf) {
            /*
             * Call sqlite3_db_config with:
             * op: SQLITE_DBCONFIG_LOOKASIDE
             * pBuf: lookasideBuf (valid pointer)
             * sz: adjustedSz (block size)
             * cnt: adjustedCnt (number of blocks)
             */
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, lookasideBuf, adjustedSz, adjustedCnt);
            (void)rc;
        }
    } else {
        /*
         * Try other safe opcodes if available, or just skip.
         * SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION is another safe option.
         */
        if (op == 1) {
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, onoff, NULL);
            (void)rc;
        }
    }

    /*
     * 3. Test sqlite3_config
     * This is a global configuration. It's safest to call it with SQLITE_CONFIG_SINGLETHREAD
     * or similar "anytime" options if possible, but the prompt notes it's not threadsafe.
     * We will try a few safe options.
     */
    int configOp = (Size > 4) ? (Data[4] % 5) : 0;
    switch (configOp) {
        case 0:
            /* SQLITE_CONFIG_SINGLETHREAD */
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
        case 1:
            /* SQLITE_CONFIG_MULTITHREAD */
            rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
            break;
        case 2:
            /* SQLITE_CONFIG_LOG */
            rc = sqlite3_config(SQLITE_CONFIG_LOG, NULL, NULL);
            break;
        case 3:
            /* SQLITE_CONFIG_GETMALLOC */
            rc = sqlite3_config(SQLITE_CONFIG_GETMALLOC, NULL, NULL, NULL, NULL);
            break;
        default:
            /* Try a generic call that might fail safely */
            rc = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
            break;
    }
    (void)rc;

    /*
     * 4. Test sqlite3_reset_auto_extension
     * Clears auto-extensions. Safe to call anytime.
     */
    sqlite3_reset_auto_extension();

    /*
     * 5. Test sqlite3_total_changes
     * Returns total changes. Safe to call on a valid connection.
     */
    int changes = sqlite3_total_changes(db);
    (void)changes;

    /*
     * 6. Test sqlite3_close_v2
     * Safely close the connection.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    /*
     * Cleanup
     */
    if (lookasideBuf) {
        free(lookasideBuf);
    }

    return 0;
}