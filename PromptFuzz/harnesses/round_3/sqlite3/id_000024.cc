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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 6
//<Quality> {"density":21,"unique_branches":{"sqlite3_backup_pagecount":[[81817,7,81817,11,0,0,4,1]],"sqlite3_db_readonly":[[182460,7,182460,32,0,0,4,1],[182466,10,182466,13,0,0,4,0]],"sqlite3_db_name":[[182425,7,182425,32,0,0,4,1],[182430,7,182430,10,0,0,4,1],[182430,14,182430,24,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_mutex_leave","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_db_readonly","sqlite3_db_name","sqlite3_create_function","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_backup_pagecount","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_mutex_leave","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_db_readonly","sqlite3_db_name","sqlite3_create_function","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_backup_pagecount","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom function for sqlite3_create_function
static void  id_000024_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    sqlite3_result_null(context);
}

// Callback for sqlite3_exec
static int  id_000024_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

// Callback for sqlite3_trace_v2
static int  id_000024_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<129) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize sqlite3
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // Open database
    sqlite3* db = nullptr;
    int rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set trace callback
    sqlite3_trace_v2(db, fuzz_uint32_t_1,  id_000024_trace_callback, nullptr);
    
    // Get database mutex
    sqlite3_mutex* mutex = sqlite3_db_mutex(db);
    if (mutex) {
        // Try to acquire mutex (non-blocking)
        sqlite3_mutex_try(mutex);
        sqlite3_mutex_leave(mutex);
    }
    
    // Execute SQL command
    char* errmsg = nullptr;
    const char* sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_2,  id_000024_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Prepare a statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    const char* select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail);
    
    if (stmt) {
        // Get data count
        int data_count = sqlite3_data_count(stmt);
        (void)data_count;  // Mark as used
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Set last insert rowid
    sqlite3_set_last_insert_rowid(db, fuzz_int64_t_3);
    
    // Check if database is readonly
    int is_readonly = sqlite3_db_readonly(db, fuzz_str_4);
    (void)is_readonly;  // Mark as used
    
    // Get database name
    const char* db_name = sqlite3_db_name(db, fuzz_int32_t_5);
    (void)db_name;  // Mark as used
    
    // Create custom function
    rc = sqlite3_create_function(db, fuzz_str_6, fuzz_int32_t_7, fuzz_int32_t_8, nullptr, 
                                  id_000024_custom_function, nullptr, nullptr);
    
    // Create backup
    sqlite3* dest_db = nullptr;
    rc = sqlite3_open("output_file", &dest_db);
    if (dest_db) {
        sqlite3_backup* backup = sqlite3_backup_init(dest_db, fuzz_str_9, db, fuzz_str_10);
        if (backup) {
            // Get backup info
            int remaining = sqlite3_backup_remaining(backup);
            int pagecount = sqlite3_backup_pagecount(backup);
            (void)remaining;  // Mark as used
            (void)pagecount;  // Mark as used
            
            // Finish backup
            sqlite3_backup_finish(backup);
        }
        sqlite3_close(dest_db);
    }
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}