// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static int dummy_busy_handler(void *ptr, int count) {
    return 0;
}

static int dummy_progress_handler(void *ptr) {
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;
    const char *dummy_sql1 = "CREATE TABLE IF NOT EXISTS t(x,y,z);";
    const char *dummy_sql2 = "INSERT INTO t VALUES(?,?,?);";
    const char *dummy_sql3 = "SELECT * FROM t;";
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set some handlers to avoid hangs
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    sqlite3_progress_handler(db, 4, dummy_progress_handler, NULL);
    
    // Create a table
    rc = sqlite3_exec(db, dummy_sql1, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare an INSERT statement
    rc = sqlite3_prepare_v2(db, dummy_sql2, -1, &stmt1, NULL);
    if (rc != SQLITE_OK || stmt1 == NULL) {
        if (stmt1) sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Bind some values using fuzzer data
    if (Size > 0) {
        // Bind first parameter as blob
        sqlite3_bind_blob(stmt1, 1, Data, Size > 100 ? 100 : (int)Size, SQLITE_TRANSIENT);
        // Bind second parameter as text
        if (Size > 10) {
            sqlite3_bind_text(stmt1, 2, (const char*)Data, Size > 50 ? 50 : (int)Size, SQLITE_TRANSIENT);
        }
        // Bind third parameter as integer
        int int_val = Size > 0 ? Data[0] : 0;
        sqlite3_bind_int(stmt1, 3, int_val);
    }
    
    // Execute the INSERT
    rc = sqlite3_step(stmt1);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Reset the statement
    sqlite3_reset(stmt1);
    
    // Prepare a SELECT statement
    rc = sqlite3_prepare_v2(db, dummy_sql3, -1, &stmt2, NULL);
    if (rc != SQLITE_OK || stmt2 == NULL) {
        sqlite3_finalize(stmt1);
        if (stmt2) sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Execute the SELECT to get a row
    rc = sqlite3_step(stmt2);
    if (rc == SQLITE_ROW) {
        // Now we have a row to work with
        
        // 1. sqlite3_column_text
        const unsigned char *text_result = sqlite3_column_text(stmt2, 0);
        (void)text_result; // Use result to avoid unused variable warning
        
        // 2. sqlite3_bind_value
        sqlite3_value *val1 = sqlite3_column_value(stmt2, 0);
        if (val1) {
            sqlite3_bind_value(stmt1, 1, val1);
        }
        
        // 3. sqlite3_column_value
        sqlite3_value *val2 = sqlite3_column_value(stmt2, 1);
        
        // 4. sqlite3_bind_value
        if (val2) {
            sqlite3_bind_value(stmt1, 2, val2);
        }
        
        // 5. sqlite3_column_value
        sqlite3_value *val3 = sqlite3_column_value(stmt2, 2);
        
        // 6. sqlite3_step
        rc = sqlite3_step(stmt1);
        if (rc != SQLITE_DONE) {
            // Reset if not done
            sqlite3_reset(stmt1);
        }
        
        // Reset stmt2 to use column functions again
        sqlite3_reset(stmt2);
        
        // Step again to get row
        rc = sqlite3_step(stmt2);
        if (rc == SQLITE_ROW) {
            // 7. sqlite3_column_int
            int int_result = sqlite3_column_int(stmt2, 2);
            (void)int_result;
            
            // 8. sqlite3_reset
            sqlite3_reset(stmt2);
            
            // Step again for remaining column functions
            rc = sqlite3_step(stmt2);
            if (rc == SQLITE_ROW) {
                // 9. sqlite3_column_bytes16
                int bytes16_1 = sqlite3_column_bytes16(stmt2, 0);
                (void)bytes16_1;
                
                // 10. sqlite3_column_blob
                const void *blob1 = sqlite3_column_blob(stmt2, 0);
                (void)blob1;
                
                // 11. sqlite3_column_bytes16
                int bytes16_2 = sqlite3_column_bytes16(stmt2, 1);
                (void)bytes16_2;
                
                // 12. sqlite3_column_blob
                const void *blob2 = sqlite3_column_blob(stmt2, 1);
                (void)blob2;
                
                // 13. sqlite3_column_bytes
                int bytes1 = sqlite3_column_bytes(stmt2, 0);
                (void)bytes1;
                
                // 14. sqlite3_column_blob
                const void *blob3 = sqlite3_column_blob(stmt2, 0);
                (void)blob3;
                
                // 15. sqlite3_column_bytes
                int bytes2 = sqlite3_column_bytes(stmt2, 1);
                (void)bytes2;
                
                // 16. sqlite3_column_blob
                const void *blob4 = sqlite3_column_blob(stmt2, 1);
                (void)blob4;
            }
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}