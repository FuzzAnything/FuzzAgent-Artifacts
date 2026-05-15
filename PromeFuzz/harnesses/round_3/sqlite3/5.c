// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static sqlite3_stmt* create_test_statement(void) {
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    const char* sql = "SELECT ?1, ?2, :name, $var, @param";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    sqlite3_close(db);
    return stmt;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    sqlite3_stmt* stmt = create_test_statement();
    if (!stmt) {
        return 0;
    }
    
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    if (param_count > 0 && Size > 0) {
        int idx = (Data[0] % param_count) + 1;
        const char* name = sqlite3_bind_parameter_name(stmt, idx);
        
        int int_val = 0;
        if (Size >= sizeof(int)) {
            memcpy(&int_val, Data, sizeof(int));
        }
        sqlite3_bind_int(stmt, idx, int_val);
        
        sqlite3_uint64 alloc_size = 0;
        if (Size >= sizeof(sqlite3_uint64)) {
            memcpy(&alloc_size, Data, sizeof(sqlite3_uint64));
        }
        void* allocated = sqlite3_malloc64(alloc_size);
        if (allocated) {
            sqlite3_free(allocated);
        }
        
        const char* text = (const char*)Data;
        size_t text_len = Size;
        if (text_len > 0) {
            sqlite3_bind_text64(stmt, idx, text, text_len, 
                               SQLITE_TRANSIENT, SQLITE_UTF8);
        }
    }
    
    sqlite3_finalize(stmt);
    return 0;
}