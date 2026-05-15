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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_db_filename","sqlite3_filename_database","sqlite3_total_changes64","sqlite3_memory_highwater","sqlite3_memory_alarm","sqlite3_snapshot_recover","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_db_filename","sqlite3_filename_database","sqlite3_total_changes64","sqlite3_memory_highwater","sqlite3_memory_alarm","sqlite3_snapshot_recover","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_close"],"visited":0}
/**/


// Memory alarm callback function
static void  id_000029_memory_alarm_callback(void *pArg, sqlite3_int64 used, int N) {
    // This callback is invoked when memory usage exceeds the alarm threshold
    // We don't need to do anything specific for fuzzing
    (void)pArg;
    (void)used;
    (void)N;
}

// User-defined function context for testing
static void  id_000029_test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Test sqlite3_get_auxdata
    void *auxdata = sqlite3_get_auxdata(context, 0);
    
    // Test sqlite3_value_frombind
    int from_bind = sqlite3_value_frombind(argv[0]);
    
    // Test sqlite3_result_subtype
    sqlite3_result_subtype(context, 123);
    
    (void)auxdata;
    (void)from_bind;
    (void)argc;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<201) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
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
    (void)in_fd;
    (void)out_fd;
    
    // Create a temporary database filename
    const char *temp_db = "fuzz_temp.db";
    
    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(temp_db, &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Test sqlite3_filename_database
    sqlite3_filename db_filename = sqlite3_db_filename(db, fuzz_str_1);
    if (db_filename) {
        const char *db_name = sqlite3_filename_database(db_filename);
        (void)db_name;
    }
    
    // Test sqlite3_total_changes64
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    (void)total_changes;
    
    // Test sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(fuzz_int32_t_2);
    (void)highwater;
    
    // Test sqlite3_memory_alarm
    rc = sqlite3_memory_alarm( id_000029_memory_alarm_callback, nullptr, fuzz_int64_t_3);
    (void)rc;
    
    // Test sqlite3_snapshot_recover
    rc = sqlite3_snapshot_recover(db, fuzz_str_4);
    (void)rc;
    
    // Create a simple table and insert some data
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);";
    sqlite3_exec(db, fuzz_str_5, nullptr, nullptr, nullptr);
    
    // Prepare a statement for binding
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test (id, data) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_6, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Test sqlite3_bind_zeroblob64
        // Use a small size to avoid excessive memory usage
        size_t blob_size = size % 1024;  // Limit size for fuzzing safety
        rc = sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_7, blob_size);
        
        // Bind an integer parameter
        sqlite3_bind_int(stmt, fuzz_int32_t_8, (int)(size % 1000));
        
        // Execute the statement
        sqlite3_step(stmt);
        
        // Reset statement for potential reuse
        sqlite3_reset(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Create a user-defined function to test context APIs
    rc = sqlite3_create_function(
        db, 
        fuzz_str_9, 
        fuzz_int32_t_10, 
        fuzz_int32_t_11, 
        nullptr, 
         id_000029_test_function, 
        nullptr, 
        nullptr
    );
    
    if (rc == SQLITE_OK) {
        // Execute the function
        sqlite3_stmt *func_stmt = nullptr;
        const char *func_sql = "SELECT test_func(1);";
        rc = sqlite3_prepare_v2(db, fuzz_str_12, -1, &func_stmt, nullptr);
        
        if (rc == SQLITE_OK && func_stmt) {
            sqlite3_step(func_stmt);
            sqlite3_finalize(func_stmt);
        }
        
        // Drop the function
        sqlite3_create_function(db, fuzz_str_13, fuzz_int32_t_14, fuzz_int32_t_15, nullptr, nullptr, nullptr, nullptr);
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Remove temporary database file
    remove(temp_db);
    
    // Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}