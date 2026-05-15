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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 252, nr_unique_branch: 43
//<Quality> {"density":21,"unique_branches":{"sqlite3CorruptError":[[14098,27,14098,28,1,0,4,1]],"getPageNormal":[[61956,7,61956,14,0,0,4,0]],"pcache1Cachesize":[[55302,9,55302,57,0,0,4,0]],"lockBtree":[[73100,9,73100,35,0,0,4,0],[73101,9,73101,38,0,0,4,0],[73102,9,73102,22,0,0,4,0],[73131,9,73131,24,0,0,4,0],[73132,11,73132,44,0,0,4,0],[73150,24,73150,50,0,0,4,0]],"sqlite3PagerFilename":[[63495,7,63495,18,0,0,4,1]],"sqlite3VdbeExec":[[94491,9,94491,37,0,0,4,0],[94492,11,94492,36,0,0,4,1],[98485,7,98485,9,0,0,4,0],[101225,7,101225,25,0,0,4,0],[101225,29,101225,46,0,0,4,1]],"unixShmMap":[[42621,9,42621,22,0,0,4,0]],"btreeInitPage":[[71965,7,71965,34,0,0,4,0],[71978,7,71978,27,0,0,4,0]],"unixLockSharedMemory":[[42416,13,42416,33,0,0,4,0],[42420,7,42420,20,0,0,4,1]],"sqlite3Step":[[90330,34,90330,76,0,0,4,1]],"sqlite3_errmsg":[[180247,9,180247,13,0,0,4,0],[14098,27,14098,28,4,0,4,0]],"decodeFlags":[[71780,15,71780,50,0,0,4,1],[71804,15,71804,52,0,0,4,1]],"corruptPageError":[[69932,7,69932,11,0,0,4,0]],"unixOpenSharedMemory":[[42551,11,42551,24,0,0,4,0],[42551,28,42551,56,0,0,4,0]],"moveToRoot":[[75248,9,75248,22,0,0,4,0],[75269,27,75269,61,0,0,4,0]],"walIndexPageRealloc":[[65017,9,65017,22,0,0,4,1],[65019,15,65019,41,0,0,4,1]],"sqlite3VdbeReset":[[87533,21,87533,31,0,0,4,0]],"sqlite3BtreeFirst":[[75358,13,75358,29,0,0,4,1]],"sqlite3AbsInt32":[[36061,7,36061,11,0,0,4,1],[36062,7,36062,25,0,0,4,1]],"moveToChild":[[75131,7,75131,20,0,0,4,1],[75137,7,75137,9,0,0,4,0]],"getAndInitPage":[[72126,7,72126,9,0,0,4,0],[72134,9,72134,22,0,0,4,0]],"walIndexReadHdr":[[66878,7,66878,20,0,0,4,0],[66880,9,66880,37,0,0,4,1]],"sqlite3RollbackAll":[[179071,61,179071,77,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_bind_zeroblob","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_reset","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_errcode","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_reset","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_errcode","sqlite3_close"],"visited":1}
/**/


// Custom function implementation that uses various sqlite3_value and sqlite3_context APIs
static void  id_000033_custom_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc == 0) {
        sqlite3_result_null(context);
        return;
    }
    
    sqlite3_value *val = argv[0];
    int type = sqlite3_value_type(val);
    
    switch (type) {
        case SQLITE_INTEGER: {
            int int_val = sqlite3_value_int(val);
            sqlite3_result_int(context, int_val * 2);
            break;
        }
        case SQLITE_BLOB: {
            int bytes = sqlite3_value_bytes16(val);
            sqlite3_result_zeroblob(context, bytes);
            break;
        }
        case SQLITE_NULL:
            sqlite3_result_null(context);
            break;
        default:
            sqlite3_result_null(context);
            break;
    }
}

// Rollback hook callback
static void  id_000033_rollback_hook_callback(void *user_data) {
    // Empty callback - just needs to exist
}

// Exec callback
static int  id_000033_exec_callback(void *data, int argc, char **argv, char **col_names) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Open a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    
    // Create a temporary database file name
    const char *db_name = "input_file";
    sqlite3 *db = nullptr;
    
    // Open database
    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set rollback hook
    sqlite3_rollback_hook(db,  id_000033_rollback_hook_callback, nullptr);
    
    // Create custom function
    sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, nullptr,
                            id_000033_custom_function, nullptr, nullptr);
    
    // Create a table and insert data using exec
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER, data BLOB);"
        "INSERT INTO test_table VALUES (1, x'123456');"
        "INSERT INTO test_table VALUES (2, zeroblob(100));";
    
    char *errmsg = nullptr;
    sqlite3_exec(db, create_table_sql,  id_000033_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Prepare a statement
    const char *select_sql = "SELECT custom_func(data), id FROM test_table WHERE id = ?";
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        if (param_count > 0) {
            // Bind a blob parameter using input data
            // Use min(size, 100) to avoid large allocations
            size_t bind_size = size > 100 ? 100 : size;
            if (bind_size > 0) {
                void *bind_data = malloc(bind_size);
                if (bind_data) {
                    // Read from input file for binding data
                    fseek(in_file, 0, SEEK_SET);
                    fread(bind_data, 1, bind_size, in_file);
                    sqlite3_bind_blob(stmt, 1, bind_data, bind_size, free);
                } else {
                    sqlite3_bind_zeroblob(stmt, 1, 100);
                }
            } else {
                sqlite3_bind_zeroblob(stmt, 1, 100);
            }
        }
        
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                
                switch (col_type) {
                    case SQLITE_INTEGER:
                        sqlite3_column_int(stmt, i);
                        break;
                    case SQLITE_BLOB: {
                        const void *blob_data = sqlite3_column_blob(stmt, i);
                        int blob_size = sqlite3_column_bytes(stmt, i);
                        // Process blob data if needed
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        
        // Reset statement
        sqlite3_reset(stmt);
        
        // Clear bindings
        sqlite3_clear_bindings(stmt);
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Check error code
    int errcode = sqlite3_errcode(db);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        assert_file_closed(&out_file);;
    }
    
    // Close database
    sqlite3_close(db);
    
    // Clean up input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
}