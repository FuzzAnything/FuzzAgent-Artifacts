// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:78474:25 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc == 0) return;
    
    sqlite3_value *value = argv[0];
    
    // Test sqlite3_value_text16le
    const void *text16le = sqlite3_value_text16le(value);
    (void)text16le;
    
    // Test sqlite3_value_subtype
    unsigned int subtype = sqlite3_value_subtype(value);
    (void)subtype;
    
    // Test sqlite3_value_int
    int int_val = sqlite3_value_int(value);
    (void)int_val;
    
    // Test sqlite3_value_text16
    const void *text16 = sqlite3_value_text16(value);
    (void)text16;
    
    // Test sqlite3_value_blob
    const void *blob = sqlite3_value_blob(value);
    (void)blob;
    
    // Test sqlite3_result_value
    sqlite3_result_value(ctx, value);
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Create a test function that uses all the target APIs
        sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8 | SQLITE_SUBTYPE, 
                               NULL, test_function, NULL, NULL);
        initialized = 1;
    }
    
    if (db == NULL) {
        return 0;
    }
    
    // Create a table and insert the fuzzer data
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    
    // Insert fuzzer data as a blob
    sqlite3_stmt *insert_stmt;
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(insert_stmt, 1, Data, Size, SQLITE_STATIC);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Query the data using our test function
    sqlite3_stmt *select_stmt;
    const char *select_sql = "SELECT test_func(data) FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
        sqlite3_step(select_stmt);
        sqlite3_finalize(select_stmt);
    }
    
    // Also test with different data types
    // Insert as text
    const char *insert_text_sql = "INSERT INTO test (data) VALUES (cast(? as text));";
    if (sqlite3_prepare_v2(db, insert_text_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
        // Use first part of data as text (up to null terminator or size)
        size_t text_len = Size;
        for (size_t i = 0; i < Size; i++) {
            if (Data[i] == 0) {
                text_len = i;
                break;
            }
        }
        sqlite3_bind_text(insert_stmt, 1, (const char*)Data, text_len, SQLITE_STATIC);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Query again with text data
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
        sqlite3_step(select_stmt);
        sqlite3_finalize(select_stmt);
    }
    
    // Test with integer data
    const char *insert_int_sql = "INSERT INTO test (data) VALUES (cast(? as integer));";
    if (sqlite3_prepare_v2(db, insert_int_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
        // Use first 4 bytes as integer
        int int_data = 0;
        if (Size >= sizeof(int)) {
            memcpy(&int_data, Data, sizeof(int));
        } else if (Size > 0) {
            memcpy(&int_data, Data, Size);
        }
        sqlite3_bind_int(insert_stmt, 1, int_data);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Query again with integer data
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
        sqlite3_step(select_stmt);
        sqlite3_finalize(select_stmt);
    }
    
    // Clear the table for next iteration
    sqlite3_exec(db, "DELETE FROM test;", NULL, NULL, NULL);
    
    return 0;
}