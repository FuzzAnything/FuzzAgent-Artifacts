// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_name at sqlite3.c:80178:24 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *dummy_sql = "SELECT ?1, ?2, ?3, ?4, ?5";
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with parameters
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // 2. sqlite3_bind_parameter_name
    // Use data to determine which parameter to query (ensure within bounds)
    int param_index = 1;
    if (param_count > 0 && Size > 0) {
        param_index = (Data[0] % param_count) + 1;
    }
    const char *param_name = sqlite3_bind_parameter_name(stmt, param_index);
    
    // 3. sqlite3_bind_int
    // Use data for integer value (if available)
    int int_value = 0;
    if (Size >= 4) {
        int_value = *(const int*)Data;  // Use first 4 bytes as integer
    }
    rc = sqlite3_bind_int(stmt, param_index, int_value);
    
    // 4. sqlite3_malloc64
    // Use data for allocation size (if available)
    sqlite3_uint64 alloc_size = 0;
    if (Size >= 8) {
        alloc_size = *(const sqlite3_uint64*)Data;  // Use first 8 bytes as size
        // Limit size to avoid excessive memory usage
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    }
    void *allocated_mem = sqlite3_malloc64(alloc_size);
    
    // 5. sqlite3_bind_text64
    // Use remaining data for text binding
    const char *text_data = (const char*)Data;
    size_t text_len = Size;
    if (text_len > 0) {
        // Ensure we don't exceed reasonable bounds
        if (text_len > 1024 * 1024) {
            text_len = 1024 * 1024;
        }
        rc = sqlite3_bind_text64(stmt, 
                                param_index, 
                                text_data, 
                                text_len,
                                SQLITE_TRANSIENT,  // SQLite makes its own copy
                                SQLITE_UTF8);
    }
    
    // Cleanup
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}