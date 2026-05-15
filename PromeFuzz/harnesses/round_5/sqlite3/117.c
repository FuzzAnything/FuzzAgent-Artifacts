// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int dummy_busy_handler(void *arg, int count) {
    // Always return 0 to not retry
    return 0;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *db_name = "./dummy_file";
    
    // Remove any existing dummy file
    remove(db_name);
    
    // Open database
    rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input to generate parameters
    // We'll use the first few bytes for parameter values
    int param1 = 0, param2 = 0, param3 = 0;
    if (Size >= 1) param1 = (int)Data[0];
    if (Size >= 2) param2 = (int)Data[1];
    if (Size >= 3) param3 = (int)Data[2];
    
    // 1. Test sqlite3_busy_timeout
    sqlite3_busy_timeout(db, param1);
    
    // 2. Test sqlite3_wal_checkpoint_v2
    int pnLog = 0, pnCkpt = 0;
    const char *zDb = NULL; // Use main database
    // Use param2 % 5 to get valid mode (0-4)
    int mode = param2 % 5;
    sqlite3_wal_checkpoint_v2(db, zDb, mode, &pnLog, &pnCkpt);
    
    // 3. Test sqlite3_busy_handler
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    
    // 4. Test sqlite3_db_cacheflush
    sqlite3_db_cacheflush(db);
    
    // 5. Test sqlite3_db_status
    int cur = 0, hiwtr = 0;
    // Use param3 % 10 to get op code (0-9 are valid SQLITE_DBSTATUS values)
    int op = param3 % 10;
    int resetFlag = (param1 & 1); // Use LSB of first param as reset flag
    sqlite3_db_status(db, op, &cur, &hiwtr, resetFlag);
    
    // 6. Test sqlite3_setlk_timeout
    // Use param1 for timeout, param2 & 1 for flags
    int flags = param2 & 1;
    sqlite3_setlk_timeout(db, param1, flags);
    
    // Cleanup
    sqlite3_close(db);
    remove(db_name);
    
    return 0;
}