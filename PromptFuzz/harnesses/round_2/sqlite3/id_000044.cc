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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":23,"unique_branches":{"sqlite3_db_config":[[178590,21,178590,29,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_sourceid","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_sourceid","sqlite3_close"],"visited":0}
/**/


// Custom function implementation
static void  id_000044_custom_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int(ctx, 42);
}

// Rollback hook callback
static void  id_000044_rollback_hook_callback(void* arg) {
    // Do nothing - just ensure hook is called
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
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
    int rc;
    
    // Create input FILE* from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE*
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Open database (use memory database for fuzzing)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Set rollback hook
    sqlite3_rollback_hook(db,  id_000044_rollback_hook_callback, nullptr);
    
    // 3. Create custom function
    rc = sqlite3_create_function(db, " id_000044_custom_func", 1, SQLITE_UTF8, 
                                 nullptr,  id_000044_custom_func, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        // Continue anyway
    }
    
    // 4. Execute SQL statements
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 5. Prepare INSERT statement
    const char* insert_sql = "INSERT INTO test(data) VALUES(?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 6. Check parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 7. Bind blob from input data
        if (param_count >= 1) {
            // Use first part of input data as blob
            size_t blob_size = size > 1024 ? 1024 : size;
            rc = sqlite3_bind_blob(stmt, 1, data, blob_size, SQLITE_STATIC);
            
            if (rc == SQLITE_OK) {
                // 8. Execute statement
                rc = sqlite3_step(stmt);
                
                if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
                    // 9. Prepare SELECT statement
                    sqlite3_finalize(stmt);
                    const char* select_sql = "SELECT data FROM test WHERE id = 1;";
                    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
                    
                    if (rc == SQLITE_OK && stmt) {
                        // 10. Execute SELECT
                        rc = sqlite3_step(stmt);
                        
                        if (rc == SQLITE_ROW) {
                            // 11. Check column type
                            int col_type = sqlite3_column_type(stmt, 0);
                            
                            if (col_type == SQLITE_BLOB) {
                                // 12. Get blob data
                                const void* blob = sqlite3_column_blob(stmt, 0);
                                
                                // 13. Get blob size
                                int blob_bytes = sqlite3_column_bytes(stmt, 0);
                                
                                // Write blob to output file
                                if (blob && blob_bytes > 0) {
                                    fwrite(blob, 1, blob_bytes, out_file);
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // 14. Finalize statement
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 15. Execute custom function
    const char* func_sql = "SELECT  id_000044_custom_func(42);";
    rc = sqlite3_exec(db, func_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 16. WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);
    
    // 17. Database configuration
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 0, &rc);
    
    // 18. Test control
    sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0xFFFFFFFF);
    
    // 19. Get error code
    int errcode = sqlite3_errcode(db);
    
    // 20. Get source ID
    const char* sourceid = sqlite3_sourceid();
    
    // Write source ID to file
    if (sourceid) {
        fwrite(sourceid, 1, strlen(sourceid), out_file);
    }
    
    // Clean up
    fflush(out_file);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 21. Close database
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}