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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *ptr) {
    // Do nothing - used for SQLITE_STATIC
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize database and statement
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a simple statement with parameters
    const char *sql = "SELECT ?, :param1, ?2, @param3, $param4";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Use fuzzer data to determine which parameter to query
    int param_index = 1;
    if (param_count > 0 && Size > 0) {
        param_index = (Data[0] % param_count) + 1;
    }
    
    // Get parameter name
    const char *param_name = sqlite3_bind_parameter_name(stmt, param_index);
    
    // Bind an integer value
    int int_value = 0;
    if (Size > 1) {
        int_value = Data[1];
    }
    sqlite3_bind_int(stmt, param_index, int_value);
    
    // Allocate memory using sqlite3_malloc64
    sqlite3_uint64 alloc_size = 0;
    if (Size > 9) {
        // Use first 8 bytes for allocation size, capped at reasonable value
        memcpy(&alloc_size, Data, sizeof(alloc_size));
        alloc_size = alloc_size % 4096; // Cap at 4KB to avoid excessive memory usage
    }
    void *allocated_mem = sqlite3_malloc64(alloc_size);
    
    // Bind text using sqlite3_bind_text64
    if (Size > 0) {
        // Use remaining data for text binding
        size_t text_size = Size;
        if (text_size > 0) {
            // Ensure we don't exceed available data
            const char *text_data = (const char *)Data;
            
            // Try different encoding options based on fuzzer input
            unsigned char encoding = SQLITE_UTF8;
            if (Size > 2) {
                encoding = Data[2] % 3; // 0=UTF8, 1=UTF16LE, 2=UTF16BE
                if (encoding == 0) encoding = SQLITE_UTF8;
                else if (encoding == 1) encoding = SQLITE_UTF16LE;
                else encoding = SQLITE_UTF16BE;
            }
            
            // Use SQLITE_TRANSIENT to copy the data
            sqlite3_bind_text64(stmt, param_index, text_data, 
                               text_size > 1024 ? 1024 : text_size, // Cap text size
                               SQLITE_TRANSIENT, encoding);
        }
    }
    
    // Cleanup
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}