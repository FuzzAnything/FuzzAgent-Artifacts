#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <sqlite3.h>
}
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 377, nr_unique_branch: 39
//<Quality> {"density":29,"unique_branches":{"columnMem":[[90780,7,90780,25,0,0,4,1]],"sqlite3ValueBytes":[[83969,7,83969,26,0,0,4,0]],"accessPayload":[[74840,7,74840,31,0,0,4,1],[74867,9,74867,45,0,0,4,1],[74888,11,74888,43,0,0,4,0]],"sqlite3_column_value":[[90869,7,90869,29,0,0,4,1]],"sqlite3VdbeExec":[[95359,9,95359,39,0,0,4,0],[95708,11,95708,14,0,0,4,0],[95729,17,95729,28,0,0,4,0],[95732,17,95732,31,0,0,4,0],[95858,11,95858,17,0,0,4,0],[95859,11,95859,17,0,0,4,0]],"sqlite3RollbackAll":[[179097,44,179097,59,0,0,4,0]],"vdbeColumnFromOverflow":[[92921,7,92921,17,0,0,4,1],[92973,9,92973,11,0,0,4,1],[92975,9,92975,17,0,0,4,0],[92975,9,92975,17,0,0,4,1],[92975,21,92975,42,0,0,4,0]],"sqlite3_str_vappendf":[[31113,15,31113,18,0,0,4,0],[31189,13,31189,19,0,0,4,0],[14098,27,14098,28,12,0,4,1],[14098,27,14098,28,13,0,4,0],[14098,27,14098,28,13,0,4,1]],"sqlite3GenerateColumnNames":[[143438,11,143438,17,0,0,4,0]],"sqlite3VdbeMemFromBtree":[[83215,7,83215,49,0,0,4,1],[83218,7,83218,66,0,0,4,0],[83220,9,83220,22,0,0,4,0]],"sqlite3_value_dup":[[89811,7,89811,15,0,0,4,1],[89813,7,89813,14,0,0,4,1],[89818,7,89818,37,0,0,4,1],[89825,13,89825,35,0,0,4,0]],"sqlite3VdbeSerialGet":[[88067,5,88067,11,0,0,4,0],[88075,5,88075,11,0,0,4,0],[14098,27,14098,28,6,0,4,0],[14098,27,14098,28,9,0,4,0],[14098,27,14098,28,12,0,4,0],[14098,27,14098,28,12,0,4,1],[14098,27,14098,28,15,0,4,0],[14098,27,14098,28,15,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_zeroblob","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_expanded_sql","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_int","sqlite3_finalize","sqlite3_rollback_hook","sqlite3_exec","sqlite3_exec","sqlite3_errcode","sqlite3_value_dup","sqlite3_column_value","sqlite3_value_bytes16","sqlite3_result_zeroblob","sqlite3_result_zeroblob","sqlite3_create_function","sqlite3_exec","sqlite3_value_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_expanded_sql","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_int","sqlite3_finalize","sqlite3_rollback_hook","sqlite3_exec","sqlite3_exec","sqlite3_errcode","sqlite3_value_dup","sqlite3_column_value","sqlite3_value_bytes16","sqlite3_result_zeroblob","sqlite3_result_zeroblob","sqlite3_create_function","sqlite3_exec","sqlite3_value_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize database connection
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a table to work with
    const char* create_table_sql = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB, value INTEGER);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Prepare a statement for insertion
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test(data, value) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the input data as blob
        if (size > 0) {
            sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size), SQLITE_STATIC);
        } else {
            // Use zeroblob if no data
            sqlite3_bind_zeroblob(stmt, 1, 0);
        }
        
        // Bind an integer value (using first 4 bytes as integer if available)
        int int_value = 0;
        if (size >= sizeof(int)) {
            memcpy(&int_value, data, sizeof(int));
        }
        sqlite3_bind_int(stmt, 2, int_value);
        
        // Execute the insert
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Get expanded SQL for debugging
        char* expanded_sql = sqlite3_expanded_sql(stmt);
        if (expanded_sql) {
            // Can be used for logging or analysis
            sqlite3_free(expanded_sql);
        }
        
        // Prepare a query to retrieve the inserted data
        const char* select_sql = "SELECT id, data, value FROM test;";
        sqlite3_stmt* select_stmt = nullptr;
        if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr) == SQLITE_OK) {
            // Execute the query
            while (sqlite3_step(select_stmt) == SQLITE_ROW) {
                // Use sqlite3_column_int to get the integer column
                int id = sqlite3_column_int(select_stmt, 0);
                (void)id; // Prevent unused variable warning
                
                // Get blob data
                const void* blob_data = sqlite3_column_blob(select_stmt, 1);
                int blob_size = sqlite3_column_bytes(select_stmt, 1);
                
                // Use sqlite3_column_int for the value column
                int retrieved_value = sqlite3_column_int(select_stmt, 2);
                (void)retrieved_value; // Prevent unused variable warning
                
                // Process blob data if available
                if (blob_data && blob_size > 0) {
                    // Here we could do something with the blob data
                }
            }
            sqlite3_finalize(select_stmt);
        }
        
        // Set up a rollback hook
        static int rollback_called = 0;
        sqlite3_rollback_hook(db, [](void* p) {
            int* called = static_cast<int*>(p);
            *called = 1;
        }, &rollback_called);
        
        // Create a transaction and roll it back to trigger the hook
        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        
        // Check error code
        int errcode = sqlite3_errcode(db);
        (void)errcode; // Prevent unused variable warning
        
        // Create a test sqlite3_value to use sqlite3_value_bytes16
        sqlite3_value* test_value = sqlite3_value_dup(sqlite3_column_value(stmt, 0));
        if (test_value) {
            // Use sqlite3_value_bytes16
            int bytes16_len = sqlite3_value_bytes16(test_value);
            (void)bytes16_len; // Prevent unused variable warning
            
            // Test sqlite3_result_zeroblob in a custom function context
            // We'll create a custom function that uses it
            auto test_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
                // Use sqlite3_result_zeroblob
                sqlite3_result_zeroblob(ctx, 10);
            };
            
            // Register and call the function
            sqlite3_create_function(db, "test_zeroblob", 0, SQLITE_UTF8, 
                                  nullptr, test_func, nullptr, nullptr);
            sqlite3_exec(db, "SELECT test_zeroblob();", nullptr, nullptr, nullptr);
            
            sqlite3_value_free(test_value);
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}