// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static void create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        // Write some dummy data to ensure the file exists and is writable
        const char *dummy = "SQLite dummy file content for fuzzing.";
        fwrite(dummy, 1, strlen(dummy), f);
        fclose(f);
    }
}

static void remove_dummy_file(const char *filename) {
    unlink(filename);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *db_name = "./dummy_file";
    int rc;
    int log_frames = 0;
    int ckpt_frames = 0;

    // Ensure the dummy file exists
    create_dummy_file(db_name);

    // Open the database
    rc = sqlite3_open_v2(db_name, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        // If we can't open, try to clean up and return
        remove_dummy_file(db_name);
        return 0;
    }

    // Enable WAL mode to make sqlite3_wal_checkpoint_v2 meaningful
    // We use a simple SQL command to switch to WAL mode
    char *err_msg = NULL;
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }

    // Fuzzing logic based on input data
    if (Size > 0) {
        // Use first byte to determine checkpoint mode
        int mode = Data[0] % 5; // 0 to 4 mapping to PASSIVE, FULL, RESTART, TRUNCATE, NOOP
        int eMode = SQLITE_CHECKPOINT_PASSIVE;
        switch (mode) {
            case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
            case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
        }

        // Use second byte to determine if we enable extended result codes
        int enable_extended = (Size > 1) ? (Data[1] % 2) : 0;
        sqlite3_extended_result_codes(db, enable_extended);

        // Use third byte to determine database name for checkpoint (NULL or "main")
        const char *zDb = NULL;
        if (Size > 2 && Data[2] % 2 == 0) {
            zDb = "main";
        }

        // Call sqlite3_wal_checkpoint_v2
        rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &log_frames, &ckpt_frames);

        // Check error codes
        int err_code = sqlite3_errcode(db);
        int ext_err_code = sqlite3_extended_errcode(db);
        int is_interrupted = sqlite3_is_interrupted(db);

        // If interrupted, we can stop early
        if (is_interrupted) {
            sqlite3_close_v2(db);
            remove_dummy_file(db_name);
            return 0;
        }
    }

    // Perform some additional operations to explore more states
    // Try to execute a simple query to potentially trigger errors
    sqlite3_exec(db, "SELECT 1;", NULL, NULL, NULL);
    
    // Check error state again
    int final_err_code = sqlite3_errcode(db);
    int final_ext_err_code = sqlite3_extended_errcode(db);

    // Close the database
    sqlite3_close_v2(db);

    // Cleanup
    remove_dummy_file(db_name);

    return 0;
}