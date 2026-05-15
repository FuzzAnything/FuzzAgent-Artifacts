// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_expanded_sql at sqlite3.c:80388:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
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
    // Empty destructor for sqlite3_bind_pointer
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Initialize database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Initialize statement pointers
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    
    // 1. sqlite3_step (on NULL statement)
    sqlite3_step(NULL);
    
    // 2. sqlite3_finalize (on NULL statement)
    sqlite3_finalize(NULL);
    
    // 3. sqlite3_finalize (on NULL statement)
    sqlite3_finalize(NULL);
    
    // Prepare SQL statement using fuzzer data
    const char *zSql = (const char*)Data;
    int nByte = -1;  // Use -1 to read until null terminator
    
    // If Size is reasonable, use it as the length
    if (Size > 0 && Size < 2147483647) {  // INT_MAX equivalent
        nByte = (int)Size;
    }
    
    const char *pzTail = NULL;
    
    // 4. sqlite3_prepare_v2
    int rc = sqlite3_prepare_v2(db, zSql, nByte, &stmt1, &pzTail);
    
    if (rc == SQLITE_OK && stmt1 != NULL) {
        // 5. sqlite3_expanded_sql
        char *expanded = sqlite3_expanded_sql(stmt1);
        
        // 6. sqlite3_free
        if (expanded != NULL) {
            sqlite3_free(expanded);
        }
        
        // 7. sqlite3_bind_pointer
        void *dummy_ptr = (void*)Data;
        sqlite3_bind_pointer(stmt1, 1, dummy_ptr, "dummy_type", dummy_destructor);
        
        // 8. sqlite3_step
        sqlite3_step(stmt1);
        
        // 9. sqlite3_finalize
        sqlite3_finalize(stmt1);
        
        // Prepare another statement for the second finalize
        sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt2, NULL);
    }
    
    // 10. sqlite3_finalize
    sqlite3_finalize(stmt2);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}