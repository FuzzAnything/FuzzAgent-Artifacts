// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
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

static int trace_callback(unsigned mask, void* context, void* p1, void* p2) {
    (void)mask;
    (void)context;
    (void)p1;
    (void)p2;
    return 0;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    if (Size < 1) {
        return 0;
    }
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    uint8_t enable_extended = Data[0] & 1;
    sqlite3_extended_result_codes(db, enable_extended);
    
    uint8_t trace_mask = Data[0] & 0x0F;
    if (Size > 1) {
        trace_mask = Data[1] & 0x0F;
    }
    sqlite3_trace_v2(db, trace_mask, trace_callback, NULL);
    
    const char *sql = "CREATE TABLE IF NOT EXISTS t1(a INTEGER, b TEXT, c BLOB, d REAL, e NULL);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    if (Size > 10) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
            
            sql = "CREATE VIRTUAL TABLE IF NOT EXISTS t2 USING fts5(content);";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
            if (rc == SQLITE_OK) {
                sqlite3_step(stmt);
                sqlite3_reset(stmt);
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
            
            remove("./dummy_file");
        }
    }
    
    if (Size > 20) {
        const char *select_sql = "SELECT * FROM t1;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail);
        if (rc == SQLITE_OK) {
            int step_result = sqlite3_step(stmt);
            if (step_result == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count && i < 5; i++) {
                    int col_type = sqlite3_column_type(stmt, i);
                    (void)col_type;
                }
            }
            sqlite3_reset(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    if (Size > 30) {
        const char *insert_sql = "INSERT INTO t1 VALUES(?, ?, ?, ?, ?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, &tail);
        if (rc == SQLITE_OK) {
            int param_count = sqlite3_bind_parameter_count(stmt);
            for (int i = 1; i <= param_count && i <= 5; i++) {
                switch (i % 5) {
                    case 1: sqlite3_bind_int(stmt, i, (int)Data[i % Size]); break;
                    case 2: sqlite3_bind_text(stmt, i, "test", 4, SQLITE_STATIC); break;
                    case 3: sqlite3_bind_blob(stmt, i, Data, Size > 100 ? 100 : Size, SQLITE_STATIC); break;
                    case 4: sqlite3_bind_double(stmt, i, 3.14159); break;
                    case 0: sqlite3_bind_null(stmt, i); break;
                }
            }
            
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}