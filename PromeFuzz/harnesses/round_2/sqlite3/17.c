// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static int prepare_and_execute_statement(sqlite3* db, const uint8_t* data, size_t size) {
    sqlite3_stmt* stmt = NULL;
    const char* sql = "SELECT 1;";
    int rc;
    
    /* Prepare a simple statement using the fuzzer data to influence preparation */
    if (size > 0) {
        /* Use first byte to decide between different SQL statements */
        switch (data[0] % 4) {
            case 0: sql = "SELECT 1;"; break;
            case 1: sql = "SELECT 2, 3;"; break;
            case 2: sql = "SELECT COUNT(*) FROM sqlite_master;"; break;
            case 3: sql = "PRAGMA journal_mode;"; break;
        }
    }
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    /* Execute the statement */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* Call sqlite3_column_int on the first column if it exists */
        if (sqlite3_column_count(stmt) > 0) {
            sqlite3_column_int(stmt, 0);
        }
    }
    
    /* Finalize the statement */
    rc = sqlite3_finalize(stmt);
    return rc;
}

int LLVMFuzzerTestOneInput_17(const uint8_t* Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    const char* filename = "./dummy_file";
    
    /* Write fuzzer data to dummy file if needed */
    if (Size > 0) {
        FILE* fp = fopen(filename, "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
        }
    }
    
    /* 1. sqlite3_open */
    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If opening fails, try with in-memory database as fallback */
        if (db) sqlite3_close(db);
        rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK || db == NULL) {
            return 0;
        }
    }
    
    /* 2. sqlite3_busy_timeout */
    if (Size > 0) {
        /* Use first byte to set timeout (0-255 ms) */
        sqlite3_busy_timeout(db, (int)(Data[0]));
    } else {
        sqlite3_busy_timeout(db, 100);
    }
    
    /* 3. sqlite3_step and 4. sqlite3_column_int (via helper function) */
    /* 5. sqlite3_finalize (also in helper) */
    prepare_and_execute_statement(db, Data, Size);
    
    /* 6. sqlite3_sleep */
    if (Size > 1) {
        /* Use second byte for sleep time (0-255 ms) */
        sqlite3_sleep((int)(Data[1]));
    } else {
        sqlite3_sleep(1);
    }
    
    /* 7. sqlite3_close */
    sqlite3_close(db);
    
    /* Clean up dummy file */
    remove(filename);
    
    return 0;
}