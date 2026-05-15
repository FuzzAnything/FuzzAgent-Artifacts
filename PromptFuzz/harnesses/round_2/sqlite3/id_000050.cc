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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 275, nr_unique_branch: 24
//<Quality> {"density":25,"unique_branches":{"sqlite3_bind_text64":[[91257,7,91257,23,0,0,4,1]],"sqlite3MutexInit":[[28572,9,28572,39,0,0,4,1]],"sqlite3_mutex_notheld":[[28699,10,28699,14,0,0,4,0]],"pcache1Init":[[55207,7,55207,37,0,0,4,1]],"sqlite3_data_count":[[90725,7,90725,13,0,0,4,1],[90725,17,90725,35,0,0,4,0]],"sqlite3MutexAlloc":[[28633,7,28633,38,0,0,4,0]],"sqlite3_initialize":[[177852,11,177852,41,0,0,4,1]],"openDatabase":[[180839,7,180839,40,0,0,4,0],[180881,7,180881,19,0,0,4,1]],"sqlite3_stmt_scanstatus_v2":[[91877,11,91877,23,0,0,4,0],[91879,13,91879,20,0,0,4,0],[91883,7,91883,20,0,0,4,1],[91886,5,91886,31,0,0,4,0],[91887,11,91887,28,0,0,4,0],[91894,5,91894,32,0,0,4,1],[91902,5,91902,29,0,0,4,1],[91912,5,91912,30,0,0,4,1],[91916,5,91916,33,0,0,4,1],[91924,5,91924,34,0,0,4,1],[91932,5,91932,34,0,0,4,1],[91940,5,91940,32,0,0,4,1],[91971,5,91971,12,0,0,4,1]],"sqlite3_config":[[178057,5,178057,36,0,0,4,0]]},"library_calls":["sqlite3_config","sqlite3_open","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text64","sqlite3_bind_text64","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_step","sqlite3_stmt_scanstatus","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_result_text16be","sqlite3_result_text16be","sqlite3_create_function","sqlite3_exec","sqlite3_drop_modules","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_config","sqlite3_open","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text64","sqlite3_bind_text64","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_step","sqlite3_stmt_scanstatus","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_result_text16be","sqlite3_result_text16be","sqlite3_create_function","sqlite3_exec","sqlite3_drop_modules","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Create a FILE* to read the input data
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
    
    // Configuration call - must be done before other sqlite3 operations
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    
    // Initialize database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to acquire a mutex (simulate thread safety check)
    sqlite3_mutex* mutex = sqlite3_db_mutex(db);
    if (mutex) {
        sqlite3_mutex_try(mutex);
    }
    
    // Create a table to work with
    const char* create_table_sql = 
        "CREATE TABLE test(id INTEGER PRIMARY KEY, data BLOB, text TEXT);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Insert some data using the input
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test(data, text) VALUES(?, ?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use sqlite3_bind_text64 to bind text data
        // Use the first half of input for blob, second half for text
        size_t half_size = size / 2;
        const char* text_data = reinterpret_cast<const char*>(data + half_size);
        size_t text_len = size - half_size;
        
        sqlite3_bind_text64(stmt, 1, text_data, text_len, 
                           SQLITE_TRANSIENT, SQLITE_UTF8);
        sqlite3_bind_text64(stmt, 2, "test_string", 11, 
                           SQLITE_STATIC, SQLITE_UTF8);
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Get last insert rowid
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    
    // Query the data back
    const char* select_sql = "SELECT * FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use sqlite3_data_count to get number of columns
        int col_count = sqlite3_data_count(stmt);
        
        // Process results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get scan status information for the statement
            void* scan_status_out = nullptr;
            sqlite3_stmt_scanstatus(stmt, 0, SQLITE_SCANSTAT_NLOOP, &scan_status_out);
            
            // Write data to output file
            for (int i = 0; i < col_count; i++) {
                const void* col_data = sqlite3_column_blob(stmt, i);
                int col_size = sqlite3_column_bytes(stmt, i);
                if (col_data && col_size > 0) {
                    fwrite(col_data, 1, col_size, out_file);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Create a user-defined function context for sqlite3_result_text16be
    struct FuncContext {
        sqlite3_context* ctx;
        const void* text;
        int len;
    };
    
    // Set up a function to demonstrate sqlite3_result_text16be
    auto test_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        const char* test_text = "TestBE";
        // Convert to big-endian UTF-16 for demonstration
        // Note: This is simplified - actual UTF-16 BE would require byte swapping
        sqlite3_result_text16be(ctx, test_text, 6, SQLITE_TRANSIENT);
    };
    
    // Register the function
    sqlite3_create_function(db, "testfunc", 0, SQLITE_UTF8, nullptr, 
                           test_func, nullptr, nullptr);
    
    // Execute the function
    sqlite3_exec(db, "SELECT testfunc();", nullptr, nullptr, nullptr);
    
    // Drop modules - use empty list to drop all modules
    const char* keep_modules[] = {nullptr};  // Empty list drops all
    sqlite3_drop_modules(db, keep_modules);
    
    // Read remaining data from input file and write to database
    fseek(in_file, 0, SEEK_SET);
    char buffer[1024];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        // Create another table for the remaining data
        sqlite3_stmt* stmt2 = nullptr;
        const char* sql = "INSERT INTO test(data) VALUES(?);";
        
        if (sqlite3_prepare_v2(db, sql, -1, &stmt2, nullptr) == SQLITE_OK) {
            sqlite3_bind_blob(stmt2, 1, buffer, bytes_read, SQLITE_TRANSIENT);
            sqlite3_step(stmt2);
            sqlite3_finalize(stmt2);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file to clean up
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}