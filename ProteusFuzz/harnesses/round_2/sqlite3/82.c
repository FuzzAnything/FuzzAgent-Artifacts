// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    // Create a temporary in-memory database to avoid file I/O issues
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Fuzz sqlite3_extended_result_codes
    // Use the first byte to determine enable/disable (0 or 1)
    if (Data[0] & 0x01) {
        rc = sqlite3_extended_result_codes(db, 1);
    } else {
        rc = sqlite3_extended_result_codes(db, 0);
    }

    // Fuzz sqlite3_is_interrupted
    // This is safe to call on any valid db
    (void)sqlite3_is_interrupted(db);

    // Fuzz sqlite3_errcode and sqlite3_extended_errcode
    // These are safe to call even if no error has occurred yet
    (void)sqlite3_errcode(db);
    (void)sqlite3_extended_errcode(db);

    // Fuzz sqlite3_wal_checkpoint_v2
    // We need to try to trigger some state where WAL might be active or not
    // First, try to create a table and enable WAL
    const char *sql_enable_wal = "PRAGMA journal_mode=WAL;";
    rc = sqlite3_exec(db, sql_enable_wal, NULL, NULL, NULL);
    
    // Prepare output variables for checkpoint
    int log_frames = 0;
    int ckpt_frames = 0;
    
    // Use input data to select checkpoint mode
    int mode = SQLITE_CHECKPOINT_PASSIVE;
    if (Size > 1) {
        int mode_choice = Data[1] % 5;
        switch (mode_choice) {
            case 0: mode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: mode = SQLITE_CHECKPOINT_FULL; break;
            case 2: mode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: mode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: mode = SQLITE_CHECKPOINT_NOOP; break;
        }
    }

    // Use input data to select database name (NULL or "main")
    const char *zDb = NULL;
    if (Size > 2 && Data[2] & 0x01) {
        zDb = "main";
    }

    rc = sqlite3_wal_checkpoint_v2(db, zDb, mode, &log_frames, &ckpt_frames);
    
    // Check error codes after checkpoint
    (void)sqlite3_errcode(db);
    (void)sqlite3_extended_errcode(db);

    // Try to insert some data to potentially trigger errors if input is malformed
    // We construct a simple SQL statement from the fuzz input if it looks like text
    if (Size > 10) {
        char *sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            // Only attempt if it starts with a valid SQL keyword to avoid immediate syntax errors
            // This is a heuristic to increase coverage of valid paths
            if (strncmp(sql, "INSERT", 6) == 0 || 
                strncmp(sql, "CREATE", 6) == 0 || 
                strncmp(sql, "UPDATE", 6) == 0 ||
                strncmp(sql, "DELETE", 6) == 0) {
                
                sqlite3_exec(db, sql, NULL, NULL, NULL);
                
                // Check error codes after execution
                (void)sqlite3_errcode(db);
                (void)sqlite3_extended_errcode(db);
            }
            free(sql);
        }
    }

    // Fuzz sqlite3_close_v2
    // This should handle any pending statements gracefully
    rc = sqlite3_close_v2(db);
    
    // After close, db is invalid. 
    // sqlite3_is_interrupted is documented to return 0 for invalid db, 
    // but in some builds/configurations it may dereference NULL.
    // To be safe and avoid ASAN errors, we do not call it with NULL.
    // The function handles NULL gracefully in standard builds, but 
    // avoiding it ensures compatibility with all builds.

    return 0;
}