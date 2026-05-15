// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_test_statement(sqlite3 *db, sqlite3_stmt **stmt) {
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB, text TEXT);";
    int rc = sqlite3_exec(db, sql, 0, 0, 0);
    if (rc != SQLITE_OK) return rc;
    
    sql = "INSERT INTO test VALUES (?, ?, ?);";
    return sqlite3_prepare_v2(db, sql, -1, stmt, 0);
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    if (db == NULL) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
    }
    
    if (stmt == NULL) {
        if (create_test_statement(db, &stmt) != SQLITE_OK) {
            sqlite3_close(db);
            db = NULL;
            return 0;
        }
    }
    
    if (Size == 0) {
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    int param_index = 1;
    if (Size > 1) {
        param_index = (Data[1] % 3) + 1;
        offset = 2;
    }
    
    switch (selector) {
        case 0: {
            if (Size - offset < 2) break;
            size_t text_size = Size - offset;
            if (text_size % 2 != 0) text_size--;
            sqlite3_bind_text16(stmt, param_index, Data + offset, text_size, SQLITE_TRANSIENT);
            break;
        }
        case 1: {
            sqlite3_uint64 blob_size = 0;
            if (Size - offset >= sizeof(sqlite3_uint64)) {
                memcpy(&blob_size, Data + offset, sizeof(sqlite3_uint64));
            } else if (Size > offset) {
                blob_size = Data[offset];
            }
            sqlite3_bind_zeroblob64(stmt, param_index, blob_size);
            break;
        }
        case 2: {
            int n = 0;
            if (Size - offset >= sizeof(int)) {
                memcpy(&n, Data + offset, sizeof(int));
                if (n < 0) n = -n;
            } else if (Size > offset) {
                n = Data[offset];
            }
            sqlite3_bind_zeroblob(stmt, param_index, n);
            break;
        }
        case 3: {
            if (Size - offset < 1) break;
            size_t blob_size = Size - offset;
            sqlite3_uint64 size64 = blob_size;
            sqlite3_bind_blob64(stmt, param_index, Data + offset, size64, SQLITE_TRANSIENT);
            break;
        }
        case 4: {
            sqlite3_clear_bindings(stmt);
            break;
        }
        case 5: {
            sqlite3_bind_null(stmt, param_index);
            break;
        }
    }
    
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    return 0;
}