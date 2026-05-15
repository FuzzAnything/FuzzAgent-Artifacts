// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to generate parameters for sqlite3_limit calls
    // We'll call it 4 times as specified
    for (int i = 0; i < 4 && Size >= sizeof(int) * 2; i++) {
        int limit_id = 0;
        int new_limit = 0;
        
        // Extract parameters from fuzzer data
        memcpy(&limit_id, Data + i * sizeof(int) * 2, sizeof(int));
        memcpy(&new_limit, Data + i * sizeof(int) * 2 + sizeof(int), sizeof(int));
        
        // Ensure limit_id is within valid range (0-9 based on SQLITE_N_LIMIT)
        limit_id = limit_id % 10;
        
        sqlite3_limit(db, limit_id, new_limit);
    }
    
    // Call sqlite3_hard_heap_limit64
    if (Size >= sizeof(sqlite3_int64)) {
        sqlite3_int64 heap_limit = 0;
        memcpy(&heap_limit, Data, sizeof(sqlite3_int64));
        sqlite3_hard_heap_limit64(heap_limit);
    }
    
    // Call sqlite3_test_control
    // We'll use a simple test control opcode that doesn't require additional arguments
    if (Size >= sizeof(int)) {
        int opcode = 0;
        memcpy(&opcode, Data, sizeof(int));
        // Use a safe opcode range (1-100)
        opcode = (opcode % 100) + 1;
        sqlite3_test_control(opcode);
    }
    
    // Call sqlite3_deserialize
    // We need to prepare a buffer for deserialization
    if (Size >= 512) {  // Minimum reasonable size for a database
        unsigned char *buffer = (unsigned char*)sqlite3_malloc(Size);
        if (buffer) {
            memcpy(buffer, Data, Size);
            
            // Calculate sizes - ensure they're reasonable
            sqlite3_int64 szDb = Size;
            sqlite3_int64 szBuf = Size;
            unsigned mFlags = 0;  // No special flags
            
            // Try deserializing
            rc = sqlite3_deserialize(db, "main", buffer, szDb, szBuf, mFlags);
            
            // Free the buffer if deserialization failed (since we didn't use FREEONCLOSE)
            if (rc != SQLITE_OK) {
                sqlite3_free(buffer);
            }
        }
    }
    
    // Call sqlite3_file_control
    if (Size >= sizeof(int)) {
        int op = 0;
        memcpy(&op, Data, sizeof(int));
        
        // Use a simple file control operation
        // SQLITE_FCNTL_LOCKSTATE is 1 and doesn't require additional arguments
        op = (op % 100) + 1;
        
        // Try with main database
        sqlite3_file_control(db, "main", op, NULL);
        
        // Try with temp database
        sqlite3_file_control(db, "temp", op, NULL);
        
        // Try with NULL database name (refers to main)
        sqlite3_file_control(db, NULL, op, NULL);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}