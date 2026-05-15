// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int) * 3) {
        return 0;
    }

    // Extract values from fuzzer input
    int param_index = *(const int*)Data % 100 + 1;  // Ensure positive index
    int int_value = *((const int*)Data + 1);
    sqlite3_uint64 malloc_size = *((const sqlite3_uint64*)(Data + sizeof(int) * 2));
    
    // Prepare a dummy SQL statement with parameters
    const char* sql = "SELECT ?1, ?2, ?3, ?4, ?5";
    
    // Create in-memory database and prepare statement
    sqlite3* db = NULL;
    sqlite3_stmt* stmt = NULL;
    
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. Call sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // 2. Call sqlite3_bind_parameter_name for a valid parameter index
    const char* param_name = NULL;
    if (param_index > 0 && param_index <= param_count) {
        param_name = sqlite3_bind_parameter_name(stmt, param_index);
    }
    
    // 3. Call sqlite3_bind_int with a valid parameter index
    if (param_index > 0 && param_index <= param_count) {
        sqlite3_bind_int(stmt, param_index, int_value);
    }
    
    // 4. Call sqlite3_malloc64
    void* allocated_mem = sqlite3_malloc64(malloc_size);
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    
    // 5. Call sqlite3_bind_text64
    if (param_index > 0 && param_index <= param_count && Size > sizeof(int) * 3) {
        size_t text_offset = sizeof(int) * 3;
        size_t text_size = Size - text_offset;
        const char* text_data = (const char*)(Data + text_offset);
        
        // Ensure text_size doesn't overflow sqlite3_uint64
        sqlite3_uint64 text_size_64 = (text_size > 0xFFFFFFFFFFFFFFFFULL) ? 0xFFFFFFFFFFFFFFFFULL : (sqlite3_uint64)text_size;
        
        sqlite3_bind_text64(stmt, param_index, text_data, text_size_64, 
                           SQLITE_TRANSIENT, SQLITE_UTF8);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}