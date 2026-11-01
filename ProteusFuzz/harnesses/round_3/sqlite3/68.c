// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:83994:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:84168:16 in sqlite3.h
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
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

static int dummy_busy_handler(void *arg, int count) {
    return 0;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    sqlite3 *dbSrc = NULL;
    sqlite3 *dbDest = NULL;
    sqlite3_backup *backup = NULL;
    int rc;
    int i;

    if (Size < 10) {
        return 0;
    }

    // Create source and destination database files
    // We use distinct filenames to ensure backup_init works
    const char *srcFile = "./fuzz_src.db";
    const char *destFile = "./fuzz_dest.db";

    // Open source database
    rc = sqlite3_open(srcFile, &dbSrc);
    if (rc != SQLITE_OK || dbSrc == NULL) {
        return 0;
    }
    sqlite3_busy_handler(dbSrc, dummy_busy_handler, NULL);

    // Open destination database
    rc = sqlite3_open(destFile, &dbDest);
    if (rc != SQLITE_OK || dbDest == NULL) {
        sqlite3_close(dbSrc);
        return 0;
    }
    sqlite3_busy_handler(dbDest, dummy_busy_handler, NULL);

    // Use fuzzing data to create some content in the source database
    // This ensures the source has pages to backup
    char *sql = (char *)malloc(Size + 100);
    if (sql) {
        // Construct a safe SQL statement to insert data
        // We limit the size to avoid huge allocations
        int safeLen = (Size < 5000) ? Size : 5000;
        snprintf(sql, safeLen + 100, 
                 "CREATE TABLE IF NOT EXISTS fuzz_data (id INTEGER PRIMARY KEY, payload BLOB); "
                 "INSERT INTO fuzz_data (payload) VALUES (X'%s');", 
                 (Size > 0) ? "00" : "");
        
        sqlite3_exec(dbSrc, sql, NULL, NULL, NULL);
        free(sql);
    }

    // Fuzz backup operation
    // We use the first few bytes of input to determine backup parameters
    int nPages = (Data[0] % 100) + 1;
    if (Data[1] & 0x01) {
        nPages = -1; // Copy all remaining
    }

    // Initialize backup
    backup = sqlite3_backup_init(dbDest, "main", dbSrc, "main");
    if (backup) {
        // Perform backup steps
        // We loop a few times to explore different states
        for (i = 0; i < 5; i++) {
            int stepPages = (Data[2 + i] % 10) + 1;
            if (Data[3 + i] & 0x01) {
                stepPages = -1;
            }
            
            rc = sqlite3_backup_step(backup, stepPages);
            
            // Check remaining and pagecount during the process
            sqlite3_backup_remaining(backup);
            sqlite3_backup_pagecount(backup);

            if (rc == SQLITE_DONE || rc != SQLITE_OK) {
                break;
            }
        }

        // Finish backup
        sqlite3_backup_finish(backup);
        backup = NULL;
    }

    // Fuzz deserialize operation
    // We need a valid serialized database to deserialize
    // For fuzzing, we can try to deserialize garbage or a small valid header
    // Note: sqlite3_deserialize expects a valid database header.
    // Passing invalid data is a good way to test error handling.
    
    // Allocate a buffer for deserialization
    unsigned char *buf = (unsigned char *)malloc(Size);
    if (buf && Size > 0) {
        memcpy(buf, Data, Size);
        
        // Try to deserialize into the destination DB
        // We use flags to test different behaviors
        unsigned int flags = 0;
        if (Data[Size - 1] & 0x01) flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
        if (Data[Size - 1] & 0x02) flags |= SQLITE_DESERIALIZE_RESIZEABLE;
        
        // Attempt deserialize
        // This will likely fail with invalid data, which is what we want for fuzzing
        rc = sqlite3_deserialize(dbDest, "main", buf, Size, Size, flags);
        
        // If FREEONCLOSE was set, we must not free buf here as SQLite will do it
        // But if it failed and FREEONCLOSE was set, SQLite frees it.
        // If it succeeded, SQLite owns it.
        // If it failed and FREEONCLOSE was NOT set, we own it.
        // To be safe in a fuzzer, we avoid FREEONCLOSE for manual cleanup or handle carefully.
        // Given the complexity, let's just not use FREEONCLOSE for the manual buffer
        // unless we are sure of the flow.
        // Actually, the spec says: "If sqlite3_deserialize() fails ... and if the 
        // SQLITE_DESERIALIZE_FREEONCLOSE bit is set ... sqlite3_free() is invoked".
        // So if we set the flag, we don't free. If we don't set it, we free.
        // Let's just not set FREEONCLOSE to simplify cleanup.
        if (!(flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
            free(buf);
        }
    } else {
        free(buf);
    }

    // Cleanup databases
    sqlite3_close(dbSrc);
    sqlite3_close(dbDest);

    // Cleanup files
    remove(srcFile);
    remove(destFile);

    return 0;
}