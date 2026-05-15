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
//<ID> 518
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 4
//<Quality> {"density":25,"unique_branches":{"sqlite3_bind_value":[[91277,5,91277,24,0,0,4,0],[91288,5,91288,21,0,0,4,1]],"sqlite3_value_dup":[[89818,7,89818,37,0,0,4,1],[89825,13,89825,35,0,0,4,1]]},"library_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_busy_timeout","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_int64","sqlite3_prepare_v2","sqlite3_value_dup","sqlite3_column_value","sqlite3_bind_value","sqlite3_step","sqlite3_finalize","sqlite3_value_free","sqlite3_finalize","sqlite3_total_changes","sqlite3_release_memory","sqlite3_close"],"critical_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_busy_timeout","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_int64","sqlite3_prepare_v2","sqlite3_value_dup","sqlite3_column_value","sqlite3_bind_value","sqlite3_step","sqlite3_finalize","sqlite3_value_free","sqlite3_finalize","sqlite3_total_changes","sqlite3_release_memory","sqlite3_close"],"visited":0}
/**/


// Custom function that uses sqlite3_result_pointer
static void  id_000072_test_pointer_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Get blob from first argument
    const void* blob = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    // Create a copy of the blob data
    void* data_copy = malloc(size);
    if (data_copy && blob) {
        memcpy(data_copy, blob, size);
        // Return the pointer
        sqlite3_result_pointer(context, data_copy, "TestBlob", free);
    } else {
        sqlite3_result_null(context);
        if (data_copy) free(data_copy);
    }
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<155) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
	//fuzzer shim end}




    // Use libversion_number
    int version = sqlite3_libversion_number();
    
    // Create a temporary database in memory
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Set busy timeout
    sqlite3_busy_timeout(db, fuzz_int32_t_1);
    
    // Create a table and insert some data
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "value INTEGER);";
    
    sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
    
    // Prepare insert statement
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test_table (data, value) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind blob data
        sqlite3_bind_blob(stmt, fuzz_int32_t_3, data, static_cast<int>(size), SQLITE_STATIC);
        sqlite3_bind_int64(stmt, fuzz_int32_t_4, static_cast<sqlite3_int64>(size));
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Register custom function
    sqlite3_create_function(db, fuzz_str_5, fuzz_int32_t_6, fuzz_int32_t_7, nullptr, 
                            id_000072_test_pointer_func, nullptr, nullptr);
    
    // Query the data back
    const char* select_sql = 
        "SELECT id, data, test_pointer(data), value FROM test_table;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Use column_int64
            sqlite3_int64 id = sqlite3_column_int64(stmt, fuzz_int32_t_8);
            
            // Get blob from column
            const void* blob = sqlite3_column_blob(stmt, fuzz_int32_t_9);
            int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_10);
            
            // Get value for binding
            sqlite3_int64 col_value = sqlite3_column_int64(stmt, fuzz_int32_t_11);
            
            // Prepare another statement to test bind_value
            sqlite3_stmt* stmt2 = nullptr;
            const char* bind_sql = "SELECT ?;";
            
            if (sqlite3_prepare_v2(db, bind_sql, -1, &stmt2, nullptr) == SQLITE_OK) {
                // Create a value for binding
                sqlite3_value* bind_val = sqlite3_value_dup(
                    sqlite3_column_value(stmt, fuzz_int32_t_12));
                
                if (bind_val) {
                    // Bind the value
                    sqlite3_bind_value(stmt2, fuzz_int32_t_13, bind_val);
                    sqlite3_step(stmt2);
                    sqlite3_finalize(stmt2);
                    sqlite3_value_free(bind_val);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Get total changes
    int total_changes = sqlite3_total_changes(db);
    
    // Release memory
    sqlite3_release_memory(fuzz_int32_t_14);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}