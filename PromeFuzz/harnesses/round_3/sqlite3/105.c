// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static sqlite3_value* create_protected_value(const uint8_t *data, size_t size) {
    // Create a database connection
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a table and insert data to get a protected value
    char *err_msg = NULL;
    const char *create_table = "CREATE TABLE test (col BLOB);";
    if (sqlite3_exec(db, create_table, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    // Prepare an INSERT statement with bound parameter
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind the fuzzer data as a blob
    if (sqlite3_bind_blob(stmt, 1, data, (int)size, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    sqlite3_finalize(stmt);
    
    // Now create a SELECT statement to get a protected value
    const char *select_sql = "SELECT col FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Step to get the row
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }
    
    // Get the unprotected value from the column
    sqlite3_value *unprotected = sqlite3_column_value(stmt, 0);
    
    // Duplicate it to create a protected value
    sqlite3_value *protected = sqlite3_value_dup(unprotected);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return protected;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Limit size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }
    
    // Create a protected sqlite3_value object
    sqlite3_value *value = create_protected_value(Data, Size);
    if (value == NULL) {
        return 0;
    }
    
    // Test all target API functions
    volatile int bytes = sqlite3_value_bytes(value);
    volatile int type = sqlite3_value_type(value);
    volatile int nochange = sqlite3_value_nochange(value);
    volatile int bytes16 = sqlite3_value_bytes16(value);
    volatile int int_val = sqlite3_value_int(value);
    volatile const void *blob = sqlite3_value_blob(value);
    
    // Use the blob pointer to avoid optimization
    if (blob != NULL) {
        // Access a small portion if available
        if (bytes > 0 && bytes < 100) {
            volatile char first_byte = *((const char*)blob);
            (void)first_byte;
        }
    }
    
    // Try to explore different program states by creating different types of values
    // Create another value with different encoding
    if (Size > 0) {
        // Create a text value
        sqlite3 *db = NULL;
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            sqlite3_stmt *stmt = NULL;
            const char *sql = "SELECT ?;";
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                // Bind as text
                sqlite3_bind_text(stmt, 1, (const char*)Data, (int)(Size > 100 ? 100 : Size), SQLITE_STATIC);
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    sqlite3_value *text_value = sqlite3_value_dup(sqlite3_column_value(stmt, 0));
                    if (text_value != NULL) {
                        volatile int text_bytes = sqlite3_value_bytes(text_value);
                        volatile int text_type = sqlite3_value_type(text_value);
                        volatile int text_bytes16 = sqlite3_value_bytes16(text_value);
                        sqlite3_value_free(text_value);
                    }
                }
                sqlite3_finalize(stmt);
            }
            sqlite3_close(db);
        }
    }
    
    // Clean up
    sqlite3_value_free(value);
    
    return 0;
}