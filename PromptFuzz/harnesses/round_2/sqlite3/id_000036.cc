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
//<ID> 248
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 4
//<Quality> {"density":21,"unique_branches":{"sqlite3_db_config":[[178588,15,178588,22,0,0,4,1],[178590,21,178590,29,0,0,4,1],[178593,15,178593,34,0,0,4,1],[178596,15,178596,19,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_result_int","sqlite3_result_int","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_reset","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_db_config","sqlite3_test_control","sqlite3_wal_checkpoint_v2","sqlite3_errcode","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_result_int","sqlite3_result_int","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_reset","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_db_config","sqlite3_test_control","sqlite3_wal_checkpoint_v2","sqlite3_errcode","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for database operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    char *errmsg = nullptr;
    int rc;
    int test_control_op = 0;
    int db_config_op = 0;
    
    // 1. Open database
    rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Set rollback hook
    int hook_data = 42;
    sqlite3_rollback_hook(db, [](void *data) {
        // Simple callback
    }, &hook_data);

    // 3. Create a custom function
    rc = sqlite3_create_function(
        db, "custom_func", 1, SQLITE_UTF8, nullptr,
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // xFunc implementation
            sqlite3_result_int(ctx, 0);
        },
        nullptr,  // xStep
        nullptr   // xFinal
    );

    // 4. Prepare SQL statement using data from input
    const char *sql = "SELECT ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 5. Get bind parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 6. Bind blob from input data if we have parameters
        if (param_count > 0 && size > 0) {
            rc = sqlite3_bind_blob(stmt, 1, data, 
                                  size > 1024 ? 1024 : (int)size, 
                                  SQLITE_STATIC);
            
            // 7. Execute the statement
            rc = sqlite3_step(stmt);
            
            if (rc == SQLITE_ROW) {
                // 8. Get column information
                int col_count = sqlite3_column_count(stmt);
                
                for (int i = 0; i < col_count; i++) {
                    // 9. Get column type
                    int col_type = sqlite3_column_type(stmt, i);
                    
                    if (col_type == SQLITE_BLOB) {
                        // 10. Get blob data
                        const void *blob = sqlite3_column_blob(stmt, i);
                        // 11. Get blob size
                        int blob_size = sqlite3_column_bytes(stmt, i);
                        
                        // Write blob to output file if it exists
                        if (blob && blob_size > 0) {
                            fwrite(blob, 1, blob_size, out_file);
                        }
                    }
                }
                
                // 12. Reset the statement
                sqlite3_reset(stmt);
                
                // 13. Clear bindings
                sqlite3_clear_bindings(stmt);
            }
        }
        
        // 14. Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // 15. Execute SQL using sqlite3_exec
    const char *exec_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER); INSERT INTO test VALUES(1);";
    rc = sqlite3_exec(db, exec_sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }

    // 16. Database configuration
    rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY);

    // 17. Test control (use a safe operation)
    rc = sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0);

    // 18. WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    rc = sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);

    // 19. Check error code
    int errcode = sqlite3_errcode(db);

    // 20. Clean up
    
    // Close database
    if (db) {
        sqlite3_close(db);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}