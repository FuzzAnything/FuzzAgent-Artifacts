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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 198, nr_unique_branch: 43
//<Quality> {"density":18,"unique_branches":{"sqlite3FkRequired":[[131319,7,131319,43,0,0,4,0],[131320,9,131320,17,0,0,4,0],[131320,9,131320,17,0,0,4,1],[131324,18,131324,43,0,0,4,1],[131324,47,131324,64,0,0,4,1],[131331,32,131331,33,0,0,4,1],[131339,40,131339,41,0,0,4,1],[18323,29,18323,57,2,0,4,0]],"sqlite3Insert":[[133003,11,133003,41,0,0,4,0]],"sqlite3GenerateConstraintChecks":[[133642,7,133642,63,0,0,4,1],[133648,9,133648,37,0,0,4,1],[133655,9,133655,19,0,0,4,1]],"pcache1Shrink":[[55321,7,55321,25,0,0,4,1]],"sqlite3_db_config":[[178537,7,178537,32,0,0,4,1],[178542,5,178542,36,0,0,4,1],[178549,5,178549,35,0,0,4,1],[178556,5,178556,12,0,0,4,0],[178583,16,178583,27,0,0,4,0],[178584,13,178584,30,0,0,4,0],[178584,13,178584,30,0,0,4,1],[178588,15,178588,22,0,0,4,0],[178593,15,178593,34,0,0,4,0],[178593,15,178593,34,0,0,4,1],[178596,15,178596,19,0,0,4,0]],"sqlite3_soft_heap_limit":[[29936,7,29936,10,0,0,4,1]],"sqlite3FkCheck":[[131074,7,131074,40,0,0,4,1],[131075,7,131075,23,0,0,4,1],[131082,32,131082,37,0,0,4,1],[131185,42,131185,47,0,0,4,1]],"sqlite3_bind_pointer":[[91231,7,91231,20,0,0,4,0]],"sqlite3_stmt_scanstatus_v2":[[91842,15,91842,22,0,0,4,1],[91843,10,91843,45,0,0,4,1],[91844,10,91844,46,0,0,4,1],[91850,7,91850,16,0,0,4,1],[91857,7,91857,14,0,0,4,1],[91869,7,91869,38,0,0,4,1],[91875,16,91875,28,0,0,4,1],[91883,7,91883,20,0,0,4,0]],"sqlite3_db_release_memory":[[178483,7,178483,32,0,0,4,1],[178487,12,178487,21,0,0,4,0],[178487,12,178487,21,0,0,4,1],[178489,9,178489,12,0,0,4,0],[178489,9,178489,12,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_soft_heap_limit","sqlite3_open","sqlite3_shutdown","sqlite3_db_config","sqlite3_db_config","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_db_status","sqlite3_db_release_memory","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_soft_heap_limit","sqlite3_open","sqlite3_db_config","sqlite3_db_config","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_db_status","sqlite3_db_release_memory","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<139) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set soft heap limit (using a reasonable value)
    sqlite3_soft_heap_limit(1024 * 1024); // 1MB limit
    
    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Configure database
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &rc);
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, &rc);
    
    // Create a table and insert some data
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT, data BLOB);";
    sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test (name, data) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind text parameter
        const char *test_name = "fuzz_test";
        sqlite3_bind_text(stmt, fuzz_int32_t_2, test_name, -1, SQLITE_STATIC);
        
        // Bind blob parameter from input data
        if (size > 0) {
            sqlite3_bind_blob(stmt, fuzz_int32_t_3, data, static_cast<int>(size > 1000 ? 1000 : size), 
                            SQLITE_STATIC);
        }
        
        // Execute statement
        rc = sqlite3_step(stmt);
        
        // Get scan status
        if (rc == SQLITE_DONE) {
            int scan_status_out = 0;
            sqlite3_stmt_scanstatus_v2(stmt, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, &scan_status_out);
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Create another statement for pointer binding
    sqlite3_stmt *stmt2 = nullptr;
    const char *select_sql = "SELECT * FROM test WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
    
    if (rc == SQLITE_OK && stmt2) {
        // Create a dummy pointer to bind
        int dummy_value = 42;
        void *dummy_ptr = &dummy_value;
        
        // Bind pointer parameter
        sqlite3_bind_pointer(stmt2, fuzz_int32_t_8, dummy_ptr, fuzz_str_7, nullptr);
        
        // Execute and get scan status
        rc = sqlite3_step(stmt2);
        if (rc == SQLITE_ROW) {
            int scan_status_out = 0;
            sqlite3_stmt_scanstatus_v2(stmt2, fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11, &scan_status_out);
        }
        
        sqlite3_finalize(stmt2);
    }
    
    // Test control operations (simulating with available APIs)
    // Since sqlite3_test_control is not in the provided list, we'll use other available APIs
    // to achieve similar testing functionality
    
    // Get database status
    int curr = 0, hiwtr = 0;
    sqlite3_db_status(db, fuzz_int32_t_12, &curr, &hiwtr, fuzz_int32_t_13);
    
    // Release memory
    sqlite3_db_release_memory(db);
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Final shutdown
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}