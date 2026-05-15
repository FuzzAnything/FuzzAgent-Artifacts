// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int trace_callback(unsigned mask, void *context, void *p1, void *p2) {
    return 0;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    if (Size < 1) {
        return 0;
    }
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    int extended_codes = Data[0] & 1;
    sqlite3_extended_result_codes(db, extended_codes);
    
    unsigned trace_mask = 0;
    if (Size > 1) {
        trace_mask = Data[1] & 0xFF;
    }
    sqlite3_trace_v2(db, trace_mask, trace_callback, NULL);
    
    const char *sql = (const char*)Data;
    int sql_len = (int)Size;
    if (sql_len > 0 && sql[sql_len-1] != '\0') {
        static char *buffer = NULL;
        static size_t buffer_size = 0;
        
        if (buffer_size < Size + 1) {
            free(buffer);
            buffer = (char*)malloc(Size + 1);
            buffer_size = Size + 1;
        }
        
        if (buffer) {
            memcpy(buffer, Data, Size);
            buffer[Size] = '\0';
            sql = buffer;
        }
    }
    
    rc = sqlite3_prepare_v2(db, sql, sql_len, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        int step_rc;
        do {
            step_rc = sqlite3_step(stmt);
            
            if (step_rc == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count && i < 10; i++) {
                    sqlite3_column_type(stmt, i);
                }
            }
        } while (step_rc == SQLITE_ROW);
        
        sqlite3_reset(stmt);
        
        if (step_rc == SQLITE_ROW || step_rc == SQLITE_DONE) {
            sqlite3_reset(stmt);
        }
        
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return 0;
}