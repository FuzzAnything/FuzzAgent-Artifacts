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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 330, nr_unique_branch: 24
//<Quality> {"density":33,"unique_branches":{"sqlite3VdbeReleaseRegisters":[[17754,41,17754,71,1,0,4,0]],"sqlite3GetVdbe":[[17755,41,17755,71,1,0,4,1]],"sqlite3GenerateConstraintChecks":[[133642,7,133642,63,0,0,4,1],[133648,9,133648,37,0,0,4,1],[133655,9,133655,19,0,0,4,1]],"sqlite3FkCheck":[[131074,7,131074,40,0,0,4,1],[131075,7,131075,23,0,0,4,1],[131082,32,131082,37,0,0,4,1],[131185,42,131185,47,0,0,4,1]],"sqlite3FkRequired":[[131319,7,131319,43,0,0,4,0],[131320,9,131320,17,0,0,4,0],[131320,9,131320,17,0,0,4,1],[131324,18,131324,43,0,0,4,1],[131324,47,131324,64,0,0,4,1],[131331,32,131331,33,0,0,4,1],[131339,40,131339,41,0,0,4,1],[18323,29,18323,57,2,0,4,0]],"sqlite3_result_value":[[90096,7,90096,14,0,0,4,1],[90097,7,90097,16,0,0,4,1],[90106,7,90106,33,0,0,4,1]],"sqlite3Insert":[[133003,11,133003,41,0,0,4,0]],"sqlite3Select":[[17755,41,17755,71,38,0,4,1],[17755,41,17755,71,39,0,4,1]],"sqlite3ExprIfFalse":[[14098,27,14098,28,42,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_close","sqlite3_db_config","sqlite3_test_control","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_finalize","sqlite3_close","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_reset","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_wal_checkpoint_v2","sqlite3_errcode","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_db_config","sqlite3_test_control","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_wal_checkpoint_v2","sqlite3_errcode","sqlite3_close"],"visited":0}
/**/


// Custom SQL function implementation
static void  id_000035_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Simple function that returns the first argument unchanged
    if (argc > 0) {
        sqlite3_result_value(context, argv[0]);
    } else {
        sqlite3_result_null(context);
    }
}

// Rollback hook callback
static void  id_000035_rollback_hook_callback(void* data) {
    // Empty callback - just to demonstrate the hook
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
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
    char* errmsg = nullptr;
    int rc = 0;
    
    // Step 1: Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Set rollback hook
    sqlite3_rollback_hook(db,  id_000035_rollback_hook_callback, nullptr);
    
    // Step 4: Create custom function
    rc = sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, 
                                 nullptr,  id_000035_custom_function, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Configure database
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &rc);
    
    // Step 6: Test control
    sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0xFFFFFFFF);
    
    // Step 7: Create table and execute SQL
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 8: Prepare INSERT statement
    const char* insert_sql = "INSERT INTO test(data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 9: Check parameter count and bind blob
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count >= 1) {
        // Bind the entire input data as a blob
        rc = sqlite3_bind_blob(stmt, 1, data, size, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Step 10: Execute the statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
        
        // Step 11: Reset statement and clear bindings
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    
    // Step 12: Prepare SELECT statement
    sqlite3_finalize(stmt);
    const char* select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Step 13: Execute SELECT and process results
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                
                if (col_type == SQLITE_BLOB) {
                    // Get blob data
                    const void* blob_data = sqlite3_column_blob(stmt, i);
                    int blob_size = sqlite3_column_bytes(stmt, i);
                    // Can process blob data here if needed
                }
            }
        }
    }
    
    // Step 14: Finalize statement
    sqlite3_finalize(stmt);
    
    // Step 15: Execute SQL using custom function
    const char* func_sql = "SELECT custom_func('test');";
    rc = sqlite3_exec(db, func_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
    }
    
    // Step 16: Perform WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);
    
    // Step 17: Check for errors
    int errcode = sqlite3_errcode(db);
    
    // Step 18: Clean up
    assert_file_closed(&in_file);;
    
    // Step 19: Close database
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}