// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16_v3 at sqlite3.c:132612:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16 at sqlite3.c:132588:16 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "sqlite3.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    write_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        cleanup_dummy_file();
        return 0;
    }
    
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    
    unsigned int prepFlags = Data[0] % 4;
    
    int nByte;
    if (Size > (size_t)INT_MAX) {
        nByte = -1;
    } else {
        nByte = (int)Size;
        if (nByte > 0) {
            nByte = nByte & ~1;
        }
    }
    
    if (nByte == 0) {
        sqlite3_close(db);
        cleanup_dummy_file();
        return 0;
    }
    
    rc = sqlite3_prepare16_v3(db, (const void*)Data, nByte, prepFlags, &stmt, &pzTail);
    if (rc == SQLITE_OK && stmt != NULL) {
        int data_count = sqlite3_data_count(stmt);
        (void)data_count;
        
        rc = sqlite3_clear_bindings(stmt);
        (void)rc;
        
        int status_op = Data[1] % 10;
        int resetFlg = Data[0] % 2;
        int status = sqlite3_stmt_status(stmt, status_op, resetFlg);
        (void)status;
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    rc = sqlite3_prepare16_v2(db, (const void*)Data, nByte, &stmt, &pzTail);
    if (rc == SQLITE_OK && stmt != NULL) {
        int data_count = sqlite3_data_count(stmt);
        (void)data_count;
        
        rc = sqlite3_clear_bindings(stmt);
        (void)rc;
        
        int status_op = Data[1] % 10;
        int resetFlg = Data[0] % 2;
        int status = sqlite3_stmt_status(stmt, status_op, resetFlg);
        (void)status;
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    rc = sqlite3_prepare16(db, (const void*)Data, nByte, &stmt, &pzTail);
    if (rc == SQLITE_OK && stmt != NULL) {
        int data_count = sqlite3_data_count(stmt);
        (void)data_count;
        
        rc = sqlite3_clear_bindings(stmt);
        (void)rc;
        
        int status_op = Data[1] % 10;
        int resetFlg = Data[0] % 2;
        int status = sqlite3_stmt_status(stmt, status_op, resetFlg);
        (void)status;
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    sqlite3_close(db);
    cleanup_dummy_file();
    
    return 0;
}