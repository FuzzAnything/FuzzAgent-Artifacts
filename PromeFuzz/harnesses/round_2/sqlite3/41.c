// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    void *agg_ctx = sqlite3_aggregate_context(ctx, 16);
    
    if (argc > 0) {
        int bytes = sqlite3_value_bytes(argv[0]);
        const unsigned char *text = sqlite3_value_text(argv[0]);
        
        if (text && bytes > 0) {
            volatile char dummy = text[bytes - 1];
            (void)dummy;
        }
    }
}

static void dummy_aggregate_final(sqlite3_context *ctx) {
    void *agg_ctx = sqlite3_aggregate_context(ctx, 0);
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    rc = sqlite3_create_function(
        db,
        "dummy_agg",
        1,
        SQLITE_UTF8,
        NULL,
        NULL,
        dummy_aggregate_step,
        dummy_aggregate_final
    );
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_agg(?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK && stmt) {
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
        }
        
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}