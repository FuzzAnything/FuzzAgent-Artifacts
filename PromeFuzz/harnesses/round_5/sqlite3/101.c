// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
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

static void create_protected_value(sqlite3 *db, sqlite3_value **protected_val, const uint8_t *data, size_t size) {
    // Create a temporary database and use sqlite3_bind_value to create a protected value
    sqlite3_stmt *stmt = NULL;
    const char *create_sql = "CREATE TABLE t1 (a);";
    const char *insert_sql = "INSERT INTO t1 VALUES (?);";
    const char *select_sql = "SELECT a FROM t1;";
    
    // Create table
    if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
        return;
    }
    
    // Prepare insert statement
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        return;
    }
    
    // Bind the data as a blob
    if (size > 0) {
        sqlite3_bind_blob(stmt, 1, data, size > 1000 ? 1000 : size, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Prepare select statement to get a protected value
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        return;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get unprotected value from column
        sqlite3_value *unprotected = sqlite3_column_value(stmt, 0);
        // Create protected value using sqlite3_value_dup
        *protected_val = sqlite3_value_dup(unprotected);
    }
    
    sqlite3_finalize(stmt);
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_value *protected_val = NULL;
    
    // Open in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a protected value using the fuzzer input
    create_protected_value(db, &protected_val, Data, Size);
    
    if (protected_val) {
        // Test all target API functions
        (void)sqlite3_value_bytes16(protected_val);
        
        const void *blob = sqlite3_value_blob(protected_val);
        if (blob) {
            // Access the blob data to potentially trigger issues
            volatile char dummy = *((const char*)blob);
            (void)dummy;
        }
        
        (void)sqlite3_value_bytes(protected_val);
        (void)sqlite3_value_type(protected_val);
        (void)sqlite3_value_int(protected_val);
        (void)sqlite3_value_nochange(protected_val);
        
        // Clean up the protected value
        sqlite3_value_free(protected_val);
    }
    
    // Clean up database
    sqlite3_close(db);
    
    return 0;
}