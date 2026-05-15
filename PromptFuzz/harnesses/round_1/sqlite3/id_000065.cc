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
//<ID> 433
//<Prompt> []
/*<Combination>: [
*/
//<score> 189, nr_unique_branch: 12
//<Quality> {"density":21,"unique_branches":{"jsonAppendSeparator":[[203008,7,203008,18,0,0,4,0]],"jsonAppendValue":[[203226,5,203226,12,0,0,4,0],[203227,11,203227,21,0,0,4,0]],"avgFinalize":[[129320,9,129320,18,0,0,4,0],[129322,11,129322,33,0,0,4,0]],"jsonGrow":[[202907,9,202907,16,0,0,4,0]],"instrFunc":[[127680,44,127680,67,0,0,4,1],[127687,11,127687,23,0,0,4,1],[127691,11,127691,21,0,0,4,1]],"jsonResult":[[203243,7,203243,17,0,0,4,1]],"syncJournal":[[60777,12,60777,47,0,0,4,0]],"jsonPrintf":[[202964,38,202964,52,0,0,4,0]]},"library_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_close","sqlite3_profile","sqlite3_create_module_v2","sqlite3_result_int","sqlite3_result_int","sqlite3_create_function","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_column_value","sqlite3_exec","sqlite3_free","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_drop_modules","sqlite3_close"],"critical_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_result_int","sqlite3_result_int","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_column_value","sqlite3_exec","sqlite3_free","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_drop_modules","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: Initialize SQLite and check version
    int version = sqlite3_libversion_number();
    if (version == 0) {
	return 0;
	}
	
    
    // Step 2: Create a temporary database file
    sqlite3* db = nullptr;
    const char* db_filename = "output_file";
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        assert_file_name_closed("output_file");
	return 0;
    }
    
    // Step 3: Set up profiling
    auto profile_callback = [](void* userdata, const char* sql, sqlite3_uint64 elapsed) {};
    sqlite3_profile(db, profile_callback, nullptr);
    
    // Step 4: Create a virtual table module (minimal implementation)
    static sqlite3_module dummy_module = {
        0,  // iVersion
        nullptr, nullptr, nullptr, nullptr, nullptr,  // xCreate, xConnect, etc.
        nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr
    };
    rc = sqlite3_create_module_v2(db, "fuzz_module", &dummy_module, nullptr, nullptr);
    
    // Step 5: Create a custom function
    auto dummy_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        sqlite3_result_int(ctx, 42);
    };
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, nullptr, 
                                dummy_func, nullptr, nullptr);
    
    // Step 6: Execute SQL from input data
    char* errmsg = nullptr;
    if (size > 0) {
        // Convert input data to null-terminated string
        char* sql = (char*)malloc(size + 1);
        if (!sql) {
            sqlite3_close(db);
            assert_file_name_closed("output_file");
	return 0;
        }
        memcpy(sql, data, size);
        sql[size] = '\0';
        
        // First exec call (create tables)
        const char* create_sql = 
            "CREATE TABLE IF NOT EXISTS fuzz_table(id INTEGER, data BLOB);"
            "CREATE VIRTUAL TABLE IF NOT EXISTS fuzz_vtab USING fuzz_module;";
        rc = sqlite3_exec(db, create_sql, nullptr, nullptr, &errmsg);
        if (errmsg) sqlite3_free(errmsg);
        
        // Prepare and execute statements from input
        sqlite3_stmt* stmt = nullptr;
        const char* tail = nullptr;
        rc = sqlite3_prepare(db, sql, -1, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt) {
            // Bind blob data if we have placeholders
            int param_count = sqlite3_bind_parameter_count(stmt);
            if (param_count > 0 && size > 0) {
                sqlite3_bind_blob(stmt, 1, data, (int)(size > 100 ? 100 : size), SQLITE_TRANSIENT);
            }
            
            // Execute statement
            rc = sqlite3_step(stmt);
            
            // Process results if any
            if (rc == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count; i++) {
                    const unsigned char* text = sqlite3_column_text(stmt, i);
                    sqlite3_value* value = sqlite3_column_value(stmt, i);
                    // Use values to prevent optimization removal
                    if (text || value) {}
                }
            }
            
            // Second exec call (insert data)
            char insert_sql[256];
            snprintf(insert_sql, sizeof(insert_sql),
                    "INSERT INTO fuzz_table VALUES(%d, ?);", version);
            rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &errmsg);
            if (errmsg) sqlite3_free(errmsg);
            
            // Reset scan status and finalize
            sqlite3_stmt_scanstatus_reset(stmt);
            sqlite3_finalize(stmt);
        }
        
        free(sql);
    }
    
    // Step 7: Clean up modules
    const char* keep_modules[] = {nullptr};
    rc = sqlite3_drop_modules(db, keep_modules);
    
    // Step 8: Close database and clean up
    sqlite3_close(db);
    
    // Remove temporary file
    remove("output_file");
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}