// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int flags;
    int onoff;
    int op;
    void *pArg = NULL;
    const char *zDbName = NULL;
    const char *zVfs = NULL;
    char filename[256];

    if (Size == 0) {
        return 0;
    }

    /* Prepare filename safely */
    size_t len = (Size < sizeof(filename) - 1) ? Size : sizeof(filename) - 1;
    memcpy(filename, Data, len);
    filename[len] = '\0';

    /* 
     * Strategy:
     * 1. Try sqlite3_open with the generated filename.
     * 2. If successful, run other API calls.
     * 3. Always close the database.
     * 4. Try sqlite3_open_v2 with derived flags.
     * 5. Try in-memory and empty string databases.
     */

    /* Step 1: Try sqlite3_open */
    rc = sqlite3_open(filename, &db);
    if (rc == SQLITE_OK && db) {
        /* Step 2: Try sqlite3_total_changes */
        int changes = sqlite3_total_changes(db);
        (void)changes;

        /* Step 3: Try sqlite3_enable_load_extension */
        if (Size > 0) {
            onoff = Data[0] % 2;
            rc = sqlite3_enable_load_extension(db, onoff);
            (void)rc;
        }

        /* Step 4: Try sqlite3_file_control with safe parameters */
        if (Size > 1) {
            op = Data[1];
            /* 
             * Only call sqlite3_file_control if we have a valid argument pointer.
             * For most opcodes, pArg can be NULL, but some require specific structures.
             * To avoid crashes, we only pass NULL or a valid pointer.
             * Here, we pass NULL to test the NULL path, which is safe for many opcodes.
             */
            rc = sqlite3_file_control(db, zDbName, op, pArg);
            (void)rc;
        }

        /* Step 5: Close the database */
        rc = sqlite3_close_v2(db);
        (void)rc;
        db = NULL;
    }

    /* Step 6: Try sqlite3_open_v2 with different flags */
    if (Size > 2) {
        flags = Data[2];
        /* Ensure at least one of the required flags is set */
        flags |= (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

        /* Try to open with a specific VFS if input suggests it */
        if (Size > 3 && Data[3] != 0) {
            size_t vfsLen = (Size - 3 < 64) ? (Size - 3) : 64;
            char vfsName[65];
            memcpy(vfsName, Data + 3, vfsLen);
            vfsName[vfsLen] = '\0';
            zVfs = vfsName;
        } else {
            zVfs = NULL;
        }

        rc = sqlite3_open_v2(filename, &db, flags, zVfs);
        if (rc == SQLITE_OK && db) {
            int changes = sqlite3_total_changes(db);
            (void)changes;

            if (Size > 0) {
                onoff = Data[0] % 2;
                rc = sqlite3_enable_load_extension(db, onoff);
                (void)rc;
            }

            if (Size > 1) {
                op = Data[1];
                rc = sqlite3_file_control(db, zDbName, op, pArg);
                (void)rc;
            }

            rc = sqlite3_close_v2(db);
            (void)rc;
            db = NULL;
        }
    }

    /* Step 7: Try opening an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        int changes = sqlite3_total_changes(db);
        (void)changes;

        if (Size > 0) {
            onoff = Data[0] % 2;
            rc = sqlite3_enable_load_extension(db, onoff);
            (void)rc;
        }

        if (Size > 1) {
            op = Data[1];
            rc = sqlite3_file_control(db, zDbName, op, pArg);
            (void)rc;
        }

        rc = sqlite3_close_v2(db);
        (void)rc;
        db = NULL;
    }

    /* Step 8: Try opening an empty string database (temporary on-disk) */
    rc = sqlite3_open("", &db);
    if (rc == SQLITE_OK && db) {
        int changes = sqlite3_total_changes(db);
        (void)changes;

        if (Size > 0) {
            onoff = Data[0] % 2;
            rc = sqlite3_enable_load_extension(db, onoff);
            (void)rc;
        }

        if (Size > 1) {
            op = Data[1];
            rc = sqlite3_file_control(db, zDbName, op, pArg);
            (void)rc;
        }

        rc = sqlite3_close_v2(db);
        (void)rc;
        db = NULL;
    }

    /* Cleanup: Ensure no database handles are left open */
    if (db) {
        sqlite3_close_v2(db);
    }

    return 0;
}