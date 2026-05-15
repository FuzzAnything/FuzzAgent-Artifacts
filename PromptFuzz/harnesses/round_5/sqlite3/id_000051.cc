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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 8
//<Quality> {"density":17,"unique_branches":{"appendOnePathElement":[[44362,11,44362,49,0,0,4,1],[44367,11,44367,17,0,0,4,1],[44367,21,44367,49,0,0,4,1],[44372,11,44372,23,0,0,4,0],[44372,11,44372,23,0,0,4,1]],"sqlite3PagerOpen":[[61199,11,61199,32,0,0,4,0],[61200,13,61200,44,0,0,4,1]],"unixFullPathname":[[44433,7,44433,20,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_get_table","sqlite3_filename_wal","sqlite3_database_file_object","sqlite3_interrupt","sqlite3_user_data","sqlite3_result_int","sqlite3_user_data","sqlite3_result_int","sqlite3_create_function_v2","sqlite3_exec","sqlite3_context_db_handle","sqlite3_vtab_on_conflict","sqlite3_prepare_v3","sqlite3_value_dup","sqlite3_value_dup","sqlite3_bind_value","sqlite3_bind_int64","sqlite3_step","sqlite3_value_free","sqlite3_value_free","sqlite3_free_table","sqlite3_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_get_table","sqlite3_filename_wal","sqlite3_database_file_object","sqlite3_interrupt","sqlite3_result_int","sqlite3_user_data","sqlite3_result_int","sqlite3_user_data","sqlite3_create_function_v2","sqlite3_exec","sqlite3_context_db_handle","sqlite3_vtab_on_conflict","sqlite3_prepare_v3","sqlite3_value_dup","sqlite3_value_dup","sqlite3_bind_value","sqlite3_bind_int64","sqlite3_step","sqlite3_value_free","sqlite3_value_free","sqlite3_free_table","sqlite3_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char** pazResult = nullptr;
    char* pzErrmsg = nullptr;
    int pnRow = 0;
    int pnColumn = 0;
    
    // Open database using the input file descriptor
    int fd = fuzz_fileno(in_file);
    char db_name[64];
    snprintf(db_name, sizeof(db_name), "file:/dev/fd/%d?mode=ro", fd);
    
    int rc = sqlite3_open_v2(db_name, &db, SQLITE_OPEN_URI | SQLITE_OPEN_READONLY, nullptr);
    
    if (rc == SQLITE_OK && db) {
        // 1. Use sqlite3_get_table
        const char* simple_query = "SELECT name FROM sqlite_master WHERE type='table' LIMIT 1;";
        rc = sqlite3_get_table(db, simple_query, &pazResult, &pnRow, &pnColumn, &pzErrmsg);
        
        // 2. Use sqlite3_filename_wal (need a filename)
        const char* filename = "input_file";
        const char* wal_name = sqlite3_filename_wal(filename);
        
        // 3. Use sqlite3_database_file_object
        sqlite3_file* db_file = sqlite3_database_file_object(filename);
        
        // 4. Use sqlite3_interrupt
        sqlite3_interrupt(db);
        
        // 5. Use sqlite3_context_db_handle - need a context
        // Create a simple function to get context
        struct FuncData {
            sqlite3_context* ctx;
        } func_data;
        
        auto func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            // Store context
            FuncData* data = static_cast<FuncData*>(sqlite3_user_data(ctx));
            if (data) {
                data->ctx = ctx;
            }
            sqlite3_result_int(ctx, 42);
        };
        
        // Register function
        rc = sqlite3_create_function_v2(db, "test_func", 0, SQLITE_UTF8, 
                                        &func_data, func, nullptr, nullptr, nullptr);
        
        // Execute function to populate context
        if (rc == SQLITE_OK) {
            rc = sqlite3_exec(db, "SELECT test_func();", nullptr, nullptr, nullptr);
            
            // Now we can use sqlite3_context_db_handle
            if (func_data.ctx) {
                sqlite3* ctx_db = sqlite3_context_db_handle(func_data.ctx);
                // Use the db handle
                if (ctx_db) {
                    // 6. Use sqlite3_vtab_on_conflict
                    int conflict_mode = sqlite3_vtab_on_conflict(ctx_db);
                }
            }
        }
        
        // 7. Use sqlite3_prepare_v3
        const char* sql = "SELECT ?1, ?2;";
        rc = sqlite3_prepare_v3(db, sql, -1, SQLITE_PREPARE_PERSISTENT, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // 8. Use sqlite3_bind_value - need a sqlite3_value
            // Create a simple value
            sqlite3_value* test_value = sqlite3_value_dup(nullptr);
            if (test_value) {
                // Initialize the value with some data
                sqlite3_value* bound_val = sqlite3_value_dup(test_value);
                if (bound_val) {
                    // Bind the value
                    rc = sqlite3_bind_value(stmt, 1, bound_val);
                    // Also bind another parameter
                    rc = sqlite3_bind_int64(stmt, 2, 100);
                    
                    // Step through the statement
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // Process row if needed
                    }
                    
                    sqlite3_value_free(bound_val);
                }
                sqlite3_value_free(test_value);
            }
        }
        
        // Write some output to file
        if (db_file && out_file) {
            // We could write database info to output file
            fprintf(out_file, "Database processed. Rows in get_table: %d\n", pnRow);
        }
    }
    
    // Cleanup
    if (pazResult) {
        sqlite3_free_table(pazResult);
    }
    if (pzErrmsg) {
        sqlite3_free(pzErrmsg);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}