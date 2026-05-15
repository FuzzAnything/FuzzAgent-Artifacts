// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT ?1, ?2, :name, @param;";
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Prepare a statement with parameters
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // 2. sqlite3_bind_parameter_name
    const char *param_name = NULL;
    if (param_count > 0) {
        // Use fuzzer data to select parameter index (1-based)
        int idx = (Size > 0) ? (Data[0] % param_count) + 1 : 1;
        param_name = sqlite3_bind_parameter_name(stmt, idx);
    }
    
    // 3. sqlite3_bind_int
    int bind_result = SQLITE_OK;
    if (param_count > 0) {
        int bind_idx = (Size > 1) ? ((Data[1] % param_count) + 1) : 1;
        int int_value = (Size > 2) ? Data[2] : 42;
        bind_result = sqlite3_bind_int(stmt, bind_idx, int_value);
    }
    
    // 4. sqlite3_malloc64
    sqlite3_uint64 alloc_size = 0;
    if (Size >= 8) {
        alloc_size = *((sqlite3_uint64*)Data);
        // Limit allocation size to avoid excessive memory usage
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    } else {
        alloc_size = Size;
    }
    void *allocated_mem = sqlite3_malloc64(alloc_size);
    
    // 5. sqlite3_bind_text64
    if (param_count > 0 && allocated_mem != NULL) {
        int text_idx = (Size > 3) ? ((Data[3] % param_count) + 1) : 1;
        
        // Prepare text data from fuzzer input
        size_t text_len = (Size > 16) ? (Size - 16) : Size;
        const char *text_data = (const char*)(Data + 16);
        if (text_len > Size) text_len = Size;
        
        // Use SQLITE_TRANSIENT so SQLite makes its own copy
        sqlite3_bind_text64(stmt, text_idx, text_data, text_len, 
                           SQLITE_TRANSIENT, SQLITE_UTF8);
        
        // Free the allocated memory
        sqlite3_free(allocated_mem);
    } else if (allocated_mem != NULL) {
        sqlite3_free(allocated_mem);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}