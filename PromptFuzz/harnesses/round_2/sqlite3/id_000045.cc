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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 92, nr_unique_branch: 6
//<Quality> {"density":23,"unique_branches":{"sqlite3_db_config":[[178542,5,178542,36,0,0,4,0],[178556,5,178556,12,0,0,4,1]],"sqlite3VdbeExec":[[95355,15,95355,35,0,0,4,0]],"sqlite3ExprCodeTarget":[[112788,15,112788,32,0,0,4,0],[14098,27,14098,28,139,0,4,1],[14098,27,14098,28,140,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_sourceid","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_sourceid","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Custom function implementation
static void  id_000045_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    sqlite3_result_int(context, argc); // Return number of arguments
}

// Rollback hook callback
static void  id_000045_rollback_hook_callback(void* data) {
    // Empty callback for rollback hook
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* err_msg = nullptr;
    
    // Create an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
	return 0;
	}
	
    
    // Set up rollback hook
    sqlite3_rollback_hook(db,  id_000045_rollback_hook_callback, nullptr);
    
    // Create custom function
    rc = sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, 
                                nullptr,  id_000045_custom_function, nullptr, nullptr);
    
    // Create test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Prepare insert statement
    const char* insert_sql = "INSERT INTO test(data) VALUES(?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind blob data from input
        int param_count = sqlite3_bind_parameter_count(stmt);
        if (param_count >= 1) {
            // Use the input data as blob
            rc = sqlite3_bind_blob(stmt, 1, data, 
                                  size > 10000 ? 10000 : (int)size,  // Limit size for safety
                                  SQLITE_STATIC);
            
            if (rc == SQLITE_OK) {
                // Execute statement
                rc = sqlite3_step(stmt);
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Execute a query using our custom function
    const char* query_sql = "SELECT custom_func(data), typeof(data) FROM test;";
    rc = sqlite3_exec(db, query_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Prepare and execute another statement to test column access
    const char* select_sql = "SELECT data FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Get column type
            int col_type = sqlite3_column_type(stmt, 0);
            if (col_type == SQLITE_BLOB) {
                // Get blob data
                const void* blob_data = sqlite3_column_blob(stmt, 0);
                int blob_size = sqlite3_column_bytes(stmt, 0);
                // We could process blob_data here if needed
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Call sqlite3_sourceid (doesn't affect state)
    const char* source_id = sqlite3_sourceid();
    
    // Perform WAL checkpoint (even though in-memory db might not use WAL)
    int pnLog = 0, pnCkpt = 0;
    rc = sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);
    
    // Try database configuration
    rc = sqlite3_db_config(db, SQLITE_DBCONFIG_MAINDBNAME);
    
    // Test control
    rc = sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0);
    
    // Get error code
    int errcode = sqlite3_errcode(db);
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}