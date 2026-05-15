// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    (void)argc;  // Unused parameter
    
    // Call the target functions in the specified order
    const unsigned char* text_result = sqlite3_value_text(argv[0]);
    const void* blob_result = sqlite3_value_blob(argv[0]);
    int bytes_result = sqlite3_value_bytes(argv[0]);
    
    // Use the fuzzing input to determine the int64 value
    // We'll use a simple hash of the blob data if available
    sqlite3_int64 int64_value = 0;
    if (blob_result != NULL && bytes_result > 0) {
        // Simple hash: sum of first 8 bytes (or fewer if less data)
        int limit = bytes_result < 8 ? bytes_result : 8;
        const unsigned char* data = (const unsigned char*)blob_result;
        for (int i = 0; i < limit; i++) {
            int64_value = (int64_value << 8) | data[i];
        }
    }
    
    // Set the result
    sqlite3_result_int64(context, int64_value);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3* db = NULL;
    
    // Initialize SQLite once
    if (!initialized) {
        if (sqlite3_initialize() != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    // Create an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a test table and insert the fuzzing data
    const char* create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    // Insert the fuzzing data as a blob
    sqlite3_stmt* stmt;
    const char* insert_sql = "INSERT INTO test_table (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Create a user-defined function that will call our target functions
    sqlite3_create_function(
        db,
        "test_udf",
        1,  // Number of arguments
        SQLITE_UTF8,
        NULL,
        test_function,
        NULL,
        NULL
    );
    
    // Call the user-defined function with our data
    sqlite3_stmt* select_stmt;
    const char* select_sql = "SELECT test_udf(data) FROM test_table WHERE id = 1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
        sqlite3_step(select_stmt);
        sqlite3_finalize(select_stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}