// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:78598:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
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
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_sqlite3_value_functions(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function is called from within SQLite context where sqlite3_value objects are protected
    if (argc < 1) return;
    
    sqlite3_value *value = argv[0];
    
    // Call all target functions on the provided sqlite3_value
    const void *blob_data = sqlite3_value_blob(value);
    int bytes = sqlite3_value_bytes(value);
    int nochange = sqlite3_value_nochange(value);
    int bytes16 = sqlite3_value_bytes16(value);
    int int_val = sqlite3_value_int(value);
    int type = sqlite3_value_type(value);
    
    // Use the results to avoid compiler optimization
    (void)blob_data;
    (void)bytes;
    (void)nochange;
    (void)bytes16;
    (void)int_val;
    (void)type;
    
    // Return a dummy result
    sqlite3_result_int(context, 0);
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create an in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Register a custom SQL function that will call our target functions
    sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, 
                           fuzz_sqlite3_value_functions, NULL, NULL);
    
    // Create a simple table
    const char *create_table_sql = "CREATE TABLE test (data BLOB);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Insert fuzzed data into the table
    sqlite3_stmt *insert_stmt = NULL;
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(insert_stmt, 1, Data, (int)Size, SQLITE_STATIC);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Call our custom function with the fuzzed data
    // This ensures sqlite3_value objects are in the protected context
    sqlite3_stmt *select_stmt = NULL;
    const char *select_sql = "SELECT fuzz_func(data) FROM test;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
        sqlite3_step(select_stmt);
        sqlite3_finalize(select_stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}