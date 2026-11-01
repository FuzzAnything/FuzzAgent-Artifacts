// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:190592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to safely close a database connection.
 * Uses sqlite3_close_v2 to handle pending statements gracefully.
 */
static void safe_close(sqlite3 *db) {
    if (db) {
        sqlite3_close_v2(db);
    }
}

/*
 * Fuzz driver entry point for LLVMFuzzer.
 * Tests sqlite3_open, sqlite3_open_v2, sqlite3_file_control,
 * sqlite3_db_name, sqlite3_db_filename, and sqlite3_close_v2.
 */
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *db_name = NULL;
    int rc;
    int op;
    int flags;
    int db_index;
    void *arg = NULL;

    /* Ensure minimum size to avoid trivial crashes on empty input */
    if (Size < 1) {
        return 0;
    }

    /*
     * Strategy:
     * 1. Use input data to determine open flags, opcodes, and indices.
     * 2. Attempt to open a database (in-memory or file-based).
     * 3. Perform file control operations.
     * 4. Query database names and filenames.
     * 5. Clean up resources.
     */

    /*
     * Determine open flags based on input byte 0.
     * We combine SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE as base.
     * Add SQLITE_OPEN_URI, SQLITE_OPEN_MEMORY, etc. based on bits.
     */
    flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Data[0] & 0x01) flags |= SQLITE_OPEN_URI;
    if (Data[0] & 0x02) flags |= SQLITE_OPEN_MEMORY;
    if (Data[0] & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
    if (Data[0] & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
    if (Data[0] & 0x10) flags |= SQLITE_OPEN_SHAREDCACHE;
    if (Data[0] & 0x20) flags |= SQLITE_OPEN_PRIVATECACHE;

    /*
     * Determine the filename.
     * If the input suggests memory mode or specific bits, use ":memory:".
     * Otherwise, use a dummy file.
     */
    if (flags & SQLITE_OPEN_MEMORY || (Data[0] & 0x40)) {
        /* Use in-memory database */
        rc = sqlite3_open_v2(":memory:", &db, flags, NULL);
    } else {
        /* Use dummy file on disk */
        /* Ensure the dummy file exists or can be created */
        FILE *f = fopen("./dummy_file", "w");
        if (f) {
            fclose(f);
        }
        rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    }

    /* If opening failed, we might still have a valid db handle if memory was allocated.
     * However, for most operations, we need a valid connection.
     * We proceed only if db is not NULL.
     */
    if (db == NULL) {
        return 0;
    }

    /*
     * 1. Test sqlite3_file_control
     * Opcodes: SQLITE_FCNTL_FILE_POINTER, SQLITE_FCNTL_JOURNAL_POINTER,
     *          SQLITE_FCNTL_VFS_POINTER, SQLITE_FCNTL_DATA_VERSION,
     *          and random values from input to test error handling.
     */
    if (Size > 1) {
        op = Data[1];
        
        /* Map some specific values to known opcodes for better coverage of success paths */
        if (op == 0) op = SQLITE_FCNTL_FILE_POINTER;
        else if (op == 1) op = SQLITE_FCNTL_JOURNAL_POINTER;
        else if (op == 2) op = SQLITE_FCNTL_VFS_POINTER;
        else if (op == 3) op = SQLITE_FCNTL_DATA_VERSION;
        /* Other values remain as random integers to test error paths */

        /* Prepare argument for opcodes that require it */
        sqlite3_file *pFile = NULL;
        sqlite3_vfs *pVfs = NULL;
        int dataVersion = 0;
        /* FIX: Use sqlite_int64 to ensure the buffer is large enough (8 bytes) 
         * for opcodes like SQLITE_FCNTL_SIZE_HINT that expect a 64-bit integer. */
        sqlite_int64 dummyInt64 = 0; 

        /* 
         * Ensure we never pass NULL as 'arg' to opcodes that might dereference it. 
         * For unknown opcodes, pass a pointer to a valid 64-bit integer on the stack 
         * to avoid stack-buffer-overflow if the internal handler reads 8 bytes.
         */
        if (op == SQLITE_FCNTL_FILE_POINTER || op == SQLITE_FCNTL_JOURNAL_POINTER) {
            arg = &pFile;
        } else if (op == SQLITE_FCNTL_VFS_POINTER) {
            arg = &pVfs;
        } else if (op == SQLITE_FCNTL_DATA_VERSION) {
            arg = &dataVersion;
        } else {
            /* For unknown opcodes, pass a pointer to a valid 64-bit integer on the stack */
            arg = &dummyInt64;
        }

        /* Call with "main" database name */
        sqlite3_file_control(db, "main", op, arg);
        
        /* Call with NULL (should refer to main) */
        sqlite3_file_control(db, NULL, op, arg);

        /* Call with invalid database name to trigger SQLITE_ERROR */
        sqlite3_file_control(db, "invalid_db_name", op, arg);
    }

    /*
     * 2. Test sqlite3_db_name
     * Iterate through a range of indices derived from input.
     */
    if (Size > 2) {
        db_index = Data[2] % 10; /* Test indices 0 to 9 */
        db_name = sqlite3_db_name(db, db_index);
        (void)db_name; /* Suppress unused variable warning */

        /* Also test with a large negative or positive index if possible */
        if (Size > 3) {
            int large_idx = (int)Data[3];
            if (large_idx > 100) large_idx = 1000;
            if (large_idx < -100) large_idx = -1000;
            db_name = sqlite3_db_name(db, large_idx);
            (void)db_name;
        }
    }

    /*
     * 3. Test sqlite3_db_filename
     * Test with "main", "temp", and invalid names.
     */
    {
        sqlite3_filename fn;
        
        fn = sqlite3_db_filename(db, "main");
        (void)fn;
        
        fn = sqlite3_db_filename(db, "temp");
        (void)fn;

        fn = sqlite3_db_filename(db, NULL);
        (void)fn;

        fn = sqlite3_db_filename(db, "invalid_db");
        (void)fn;
    }

    /*
     * 4. Test sqlite3_open (simple wrapper)
     * We already opened with open_v2, but we can try opening another connection
     * to test the simple API path, using a different file or memory.
     */
    sqlite3 *db2 = NULL;
    if (Size > 3) {
        /* Reuse dummy file or memory based on input */
        if (Data[3] & 0x01) {
            rc = sqlite3_open(":memory:", &db2);
        } else {
            rc = sqlite3_open("./dummy_file", &db2);
        }
        if (db2) {
            /* Perform a quick operation to ensure it's valid */
            /* Use a safe argument for file_control here as well */
            sqlite_int64 safe_arg64 = 0;
            sqlite3_file_control(db2, "main", SQLITE_FCNTL_DATA_VERSION, &safe_arg64);
            safe_close(db2);
        }
    }

    /*
     * 5. Cleanup
     * Close the primary database connection.
     */
    safe_close(db);

    return 0;
}