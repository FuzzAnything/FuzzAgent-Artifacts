// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:190608:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:190608:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:190608:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target APIs:
 * 1. sqlite3_open
 * 2. sqlite3_wal_checkpoint_v2
 * 3. sqlite3_file_control
 * 4. sqlite3_db_readonly
 * 5. sqlite3_db_name
 * 6. sqlite3_wal_checkpoint
 */

/*
 * Helper to safely close a database connection if it is valid.
 */
static void safe_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Helper to create a temporary file with fuzzing data if needed.
 * Returns 0 on success, -1 on failure.
 */
static int create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return -1;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int pnLog = 0, pnCkpt = 0;
    const char *dbName;
    int dbIndex;

    /*
     * Step 1: Prepare environment.
     * We will try to open a database using the fuzzing data as the filename.
     * To avoid issues with null bytes or invalid paths, we can use a fixed filename
     * or construct a safe path. Here we use "./dummy_file" and write data to it.
     */
    if (Size > 0) {
        create_dummy_file(Data, Size);
    }

    /*
     * Try to open the database.
     * We use "./dummy_file" as the filename to ensure a valid path.
     * We also try ":memory:" to test in-memory databases.
     */
    const char *filename = "./dummy_file";
    if (Size > 10 && Data[0] == 'm' && Data[1] == 'e' && Data[2] == 'm') {
        /* Heuristic to switch to memory mode if data starts with "mem" */
        filename = ":memory:";
    }

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        /*
         * If opening fails, we might still have a valid db handle (unless memory failed).
         * We can still try to run some checks, but many operations will fail.
         * We proceed to cleanup at the end.
         */
    }

    if (db) {
        /*
         * Step 2 & 3: Explore program states by invoking target functions diversely.
         */

        /*
         * 1. sqlite3_wal_checkpoint_v2
         * Try different modes and database names.
         */
        int modes[] = {
            SQLITE_CHECKPOINT_PASSIVE,
            SQLITE_CHECKPOINT_FULL,
            SQLITE_CHECKPOINT_RESTART,
            SQLITE_CHECKPOINT_TRUNCATE,
            SQLITE_CHECKPOINT_NOOP
        };
        int num_modes = sizeof(modes) / sizeof(modes[0]);

        for (int i = 0; i < num_modes; i++) {
            /* Try with NULL (all databases) */
            sqlite3_wal_checkpoint_v2(db, NULL, modes[i], &pnLog, &pnCkpt);
            /* Try with "main" */
            sqlite3_wal_checkpoint_v2(db, "main", modes[i], &pnLog, &pnCkpt);
            /* Try with a random string from input data if available */
            if (Size > 5) {
                char zDb[6] = {0};
                memcpy(zDb, Data, 5);
                sqlite3_wal_checkpoint_v2(db, zDb, modes[i], &pnLog, &pnCkpt);
            }
        }

        /*
         * 2. sqlite3_file_control
         * Try various opcodes.
         * FIX: For opcodes that require an output pointer (like SQLITE_FCNTL_FILE_POINTER,
         * SQLITE_FCNTL_VFS_POINTER, SQLITE_FCNTL_JOURNAL_POINTER), we must provide
         * a valid pointer instead of NULL. We use local dummy variables for this.
         */
        int opcodes[] = {
            SQLITE_FCNTL_FILE_POINTER,
            SQLITE_FCNTL_JOURNAL_POINTER,
            SQLITE_FCNTL_VFS_POINTER,
            SQLITE_FCNTL_DATA_VERSION
        };
        int num_opcodes = sizeof(opcodes) / sizeof(opcodes[0]);

        sqlite3_file *dummyFilePtr = NULL;
        sqlite3_vfs *dummyVfsPtr = NULL;
        unsigned int dummyDataVersion = 0;

        for (int i = 0; i < num_opcodes; i++) {
            void *argPtr = NULL;

            /* Select the appropriate dummy pointer based on the opcode */
            if (opcodes[i] == SQLITE_FCNTL_FILE_POINTER || opcodes[i] == SQLITE_FCNTL_JOURNAL_POINTER) {
                argPtr = &dummyFilePtr;
            } else if (opcodes[i] == SQLITE_FCNTL_VFS_POINTER) {
                argPtr = &dummyVfsPtr;
            } else if (opcodes[i] == SQLITE_FCNTL_DATA_VERSION) {
                argPtr = &dummyDataVersion;
            }

            /* Call with NULL db name */
            sqlite3_file_control(db, NULL, opcodes[i], argPtr);
            /* Call with "main" db name */
            sqlite3_file_control(db, "main", opcodes[i], argPtr);
            if (Size > 5) {
                char zDb[6] = {0};
                memcpy(zDb, Data, 5);
                sqlite3_file_control(db, zDb, opcodes[i], argPtr);
            }
        }

        /*
         * 3. sqlite3_db_readonly
         * Check if databases are read-only.
         */
        sqlite3_db_readonly(db, NULL);
        sqlite3_db_readonly(db, "main");
        if (Size > 5) {
            char zDb[6] = {0};
            memcpy(zDb, Data, 5);
            sqlite3_db_readonly(db, zDb);
        }

        /*
         * 4. sqlite3_db_name
         * Try to get schema names for different indices.
         */
        for (int i = -1; i <= 10; i++) {
            dbName = sqlite3_db_name(db, i);
            /* dbName might be NULL, which is valid. We don't dereference it here. */
        }

        /*
         * 5. sqlite3_wal_checkpoint
         * Legacy wrapper.
         */
        sqlite3_wal_checkpoint(db, NULL);
        sqlite3_wal_checkpoint(db, "main");
        if (Size > 5) {
            char zDb[6] = {0};
            memcpy(zDb, Data, 5);
            sqlite3_wal_checkpoint(db, zDb);
        }

        /*
         * Additional exploration: Try to execute a simple SQL statement to ensure the DB is usable.
         * This can help trigger more code paths related to the database state.
         */
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    /*
     * Step 2: Cleanup.
     * Always close the database connection if it was opened.
     */
    safe_close(db);

    return 0;
}