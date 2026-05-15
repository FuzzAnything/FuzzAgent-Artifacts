// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *filename = "./dummy_file";
    const char *sql = "SELECT 1";
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // 1. sqlite3_open
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        // If opening fails, we can still try to close if db was partially created
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // 2. sqlite3_busy_timeout
    // Use first 4 bytes of input as timeout value (if available)
    int timeout = 0;
    if (Size >= 4) {
        timeout = *(int*)Data;
    }
    sqlite3_busy_timeout(db, timeout);
    
    // 3. Prepare a statement for sqlite3_step
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        // 4. sqlite3_step
        rc = sqlite3_step(stmt);
        
        // 5. sqlite3_column_int (only if we got a row)
        if (rc == SQLITE_ROW) {
            int col_value = sqlite3_column_int(stmt, 0);
            (void)col_value; // Use value to avoid unused variable warning
        }
        
        // 6. sqlite3_finalize
        sqlite3_finalize(stmt);
    }
    
    // 7. sqlite3_sleep
    // Use next 4 bytes of input as sleep time (if available)
    int sleep_ms = 0;
    if (Size >= 8) {
        sleep_ms = *(int*)(Data + 4);
    }
    sqlite3_sleep(sleep_ms);
    
    // 8. sqlite3_close
    sqlite3_close(db);
    
    return 0;
}