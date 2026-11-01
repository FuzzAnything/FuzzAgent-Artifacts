// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API sequence:
 * sqlite3_open -> sqlite3_db_name -> sqlite3_file_control -> sqlite3_file_control -> sqlite3_close -> sqlite3_open
 */

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *dbName = NULL;
    int op1, op2;
    int rc;
    void *arg1 = NULL;
    void *arg2 = NULL;
    int dbIndex = 0;

    /*
     * Step 1: Prepare environment
     * Open the database. Use "./dummy_file" as specified.
     */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        /* If opening fails, close if handle exists (e.g., OOM handled by sqlite3_open) */
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    /*
     * Step 2: Invoke sqlite3_db_name
     * Determine index N from input data.
     */
    if (Size > 0) {
        dbIndex = (int)Data[0] % 5; // 0 to 4
    } else {
        dbIndex = 0;
    }

    dbName = sqlite3_db_name(db, dbIndex);

    /*
     * Step 3: Invoke sqlite3_file_control (First call)
     */
    if (Size > 1) {
        op1 = (int)Data[1];
    } else {
        op1 = SQLITE_FCNTL_FILE_POINTER;
    }

    sqlite3_file *dummyFile = NULL;
    arg1 = (void *)&dummyFile;

    rc = sqlite3_file_control(db, dbName, op1, arg1);

    /*
     * Step 4: Invoke sqlite3_file_control (Second call)
     */
    if (Size > 2) {
        op2 = (int)Data[2];
    } else {
        op2 = SQLITE_FCNTL_VFS_POINTER;
    }

    sqlite3_vfs *dummyVfs = NULL;
    arg2 = (void *)&dummyVfs;

    rc = sqlite3_file_control(db, dbName, op2, arg2);
    
    /* 
     * Fix for LeakSanitizer: 
     * SQLITE_FCNTL_VFS_POINTER returns a string via sqlite3_mprintf which must be freed.
     * We cast the pointer to char* and free it if it was successfully allocated.
     */
    if (rc == SQLITE_OK && arg2 != NULL && op2 == SQLITE_FCNTL_VFS_POINTER) {
        char *vfsName = *(char **)arg2;
        if (vfsName != NULL) {
            sqlite3_free(vfsName);
        }
    }

    /*
     * Step 5: Invoke sqlite3_close
     */
    rc = sqlite3_close(db);
    db = NULL;

    /*
     * Step 6: Invoke sqlite3_open again
     */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_close(db);
        db = NULL;
    }

    return 0;
}