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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"sqlite3_result_null":[[89976,7,89976,14,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_close","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_reset","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_finalize","sqlite3_errcode","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_reset","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_finalize","sqlite3_errcode","sqlite3_close"],"visited":0}
/**/


// Custom function implementation
static void  id_000032_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc == 0) {
        sqlite3_result_null(context);
        return;
    }
    
    // Use various value APIs
    int val_type = sqlite3_value_type(argv[0]);
    
    if (val_type == SQLITE_INTEGER) {
        int int_val = sqlite3_value_int(argv[0]);
        sqlite3_result_int(context, int_val * 2); // Example transformation
    } 
    else if (val_type == SQLITE_BLOB || val_type == SQLITE_TEXT) {
        int bytes = sqlite3_value_bytes16(argv[0]);
        if (bytes > 100) { // Arbitrary threshold
            sqlite3_result_zeroblob(context, 16); // Return zeroblob
        } else {
            sqlite3_result_null(context);
        }
    } 
    else {
        sqlite3_result_null(context);
    }
}

// Rollback hook callback
static void  id_000032_rollback_hook_callback(void* user_data) {
    // Do nothing - just for demonstration
}

// Exec callback function
static int  id_000032_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0; // Continue processing
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* err_msg = nullptr;
    
    // 1. Open database (in memory for fuzzing)
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // 2. Set rollback hook
    sqlite3_rollback_hook(db,  id_000032_rollback_hook_callback, nullptr);
    
    // 3. Create custom function
    rc = sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, nullptr,
                                  id_000032_custom_function, nullptr, nullptr);
    
    // 4. Use fmemopen to create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create output file (won't actually be used much in fuzzing)
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create a table and insert data using exec
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql,  id_000032_exec_callback, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 7. Prepare statement for insertion
    const char* insert_sql = "INSERT INTO test_table (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 8. Check parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        if (param_count > 0) {
            // 9. Bind input data as blob (use first half of data)
            size_t bind_size = size > 1024 ? 1024 : size; // Limit bind size
            rc = sqlite3_bind_blob(stmt, 1, data, bind_size, SQLITE_STATIC);
            
            if (rc == SQLITE_OK) {
                // 10. Execute statement
                rc = sqlite3_step(stmt);
                
                if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
                    // Get column count (for demonstration)
                    int col_count = sqlite3_column_count(stmt);
                    
                    // Try to get column data (won't work for INSERT, but for demonstration)
                    if (col_count > 0) {
                        int col_type = sqlite3_column_type(stmt, 0);
                        if (col_type == SQLITE_INTEGER) {
                            int int_val = sqlite3_column_int(stmt, 0);
                            // Use the value somehow
                        } else if (col_type == SQLITE_BLOB) {
                            const void* blob_data = sqlite3_column_blob(stmt, 0);
                            int blob_size = sqlite3_column_bytes(stmt, 0);
                            // Use the blob data somehow
                        }
                    }
                }
            }
            
            // 11. Reset statement for potential reuse
            sqlite3_reset(stmt);
            
            // 12. Bind zeroblob (use second parameter if available)
            if (param_count > 1) {
                sqlite3_bind_zeroblob(stmt, 2, 100); // 100-byte zeroblob
                sqlite3_step(stmt); // Execute again
                sqlite3_reset(stmt);
            }
        }
        
        // 13. Finalize statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 14. Prepare another statement for querying
    const char* select_sql = "SELECT custom_func(data) FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute query
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Get column information
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                // Process based on type...
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // 15. Check error code
    int errcode = sqlite3_errcode(db);
    
    // 16. Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 17. Close database
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}