// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    const char *db_name;
    void *arg = NULL;
    
    /* Write fuzzed data to dummy file */
    write_dummy_file(Data, Size);
    
    /* First sqlite3_open */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* sqlite3_db_name */
    if (Size > 0) {
        int n = Data[0] % 3; /* 0=main, 1=temp, 2=out of range */
        db_name = sqlite3_db_name(db, n);
        /* db_name may be NULL, that's okay */
    }
    
    /* First sqlite3_file_control */
    if (Size > 1) {
        int op = Data[1] % 256; /* Use fuzzed byte for opcode */
        const char *zDbName = NULL;
        if (Size > 2) {
            if (Data[2] % 2 == 0) {
                zDbName = "main";
            } else if (Data[2] % 3 == 0) {
                zDbName = "temp";
            }
            /* else NULL */
        }
        sqlite3_file_control(db, zDbName, op, &arg);
    }
    
    /* Second sqlite3_file_control with different parameters */
    if (Size > 3) {
        int op = Data[3] % 256;
        const char *zDbName = NULL;
        if (Size > 4) {
            if (Data[4] % 2 == 0) {
                zDbName = "main";
            } else if (Data[4] % 3 == 0) {
                zDbName = "temp";
            }
            /* else NULL */
        }
        sqlite3_file_control(db, zDbName, op, NULL);
    }
    
    /* sqlite3_close */
    sqlite3_close(db);
    
    /* Second sqlite3_open with potentially different flags */
    db = NULL;
    if (Size > 5) {
        rc = sqlite3_open("./dummy_file", &db);
        if (rc == SQLITE_OK && db) {
            /* Try to get db_name again with different index */
            if (Size > 6) {
                int n = Data[6] % 4;
                db_name = sqlite3_db_name(db, n);
            }
            sqlite3_close(db);
        }
    }
    
    return 0;
}