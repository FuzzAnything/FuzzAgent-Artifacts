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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 5
//<Quality> {"density":17,"unique_branches":{"pager_truncate":[[59094,11,59094,30,0,0,4,1],[59096,17,59096,46,0,0,4,0],[59096,17,59096,46,0,0,4,1],[14098,27,14098,28,6,0,4,1],[14098,27,14098,28,7,0,4,0]]},"library_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_open_v2","sqlite3_close","sqlite3_close","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_create_function","sqlite3_create_function_v2","sqlite3_create_module","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_status","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_open_v2","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_create_function","sqlite3_create_function_v2","sqlite3_create_module","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_status","sqlite3_close","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to achieve the event of opening a database from input data, executing SQL, and profiling the execution:

*/


// Profile callback function
static void  id_000011_profile_callback(void *user_data, const char *sql, sqlite3_uint64 elapsed_ns) {
    // Do nothing for fuzzing - just needed for sqlite3_profile
    (void)user_data;
    (void)sql;
    (void)elapsed_ns;
}

// Exec callback function
static int  id_000011_exec_callback(void *user_data, int argc, char **argv, char **col_names) {
    // Do nothing for fuzzing - just needed for sqlite3_exec
    (void)user_data;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

// Custom function callback
static void  id_000011_custom_func_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3_result_int(ctx, 42);
}

// Custom function step callback
static void  id_000011_custom_step_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    (void)ctx;
}

// Custom function final callback
static void  id_000011_custom_final_callback(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 100);
}

// Custom v2 function destroy callback
static void  id_000011_custom_destroy_callback(void *ptr) {
    (void)ptr;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<279) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Write input data to a temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a temporary file name
    const char *temp_db_name = "input_file";
    FILE *temp_file = fopen(temp_db_name, "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Copy data from memory to temp file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&temp_file);;
    
    // 2. Shutdown and initialize SQLite
    sqlite3_shutdown();
    
    // 3. Open database using different methods
    sqlite3 *db1 = nullptr;
    sqlite3 *db2 = nullptr;
    
    // Open with sqlite3_open
    int rc1 = sqlite3_open(fuzz_str_1, &db1);
    
    // Open with sqlite3_open_v2
    int rc2 = sqlite3_open_v2(fuzz_str_2, &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    
    if (rc1 != SQLITE_OK || rc2 != SQLITE_OK) {
        if (db1) sqlite3_close(db1);
        if (db2) sqlite3_close(db2);
        remove(temp_db_name);
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Set up profiling
    void *profile_data = malloc(64);
    sqlite3_profile(db1,  id_000011_profile_callback, profile_data);
    
    // 5. Execute SQL using sqlite3_exec
    char *errmsg = nullptr;
    const char *test_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT);"
                          "INSERT INTO test(data) VALUES('fuzz test');"
                          "SELECT * FROM test;"
                          "DROP TABLE IF EXISTS test;";
    
    sqlite3_exec(db1, fuzz_str_3,  id_000011_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // 6. Create custom functions
    sqlite3_create_function(db1, fuzz_str_4, fuzz_int32_t_5, fuzz_int32_t_6, nullptr, 
                            id_000011_custom_func_callback, nullptr, nullptr);
    
    sqlite3_create_function_v2(db2, fuzz_str_7, fuzz_int32_t_8, fuzz_int32_t_9, nullptr,
                               id_000011_custom_func_callback,  id_000011_custom_step_callback, 
                               id_000011_custom_final_callback,  id_000011_custom_destroy_callback);
    
    // 7. Create module (dummy implementation)
    sqlite3_module dummy_module = {0};
    sqlite3_create_module(db1, fuzz_str_10, &dummy_module, nullptr);
    
    // 8. Prepare and execute statements
    sqlite3_stmt *stmt = nullptr;
    const char *select_sql = "SELECT 1, 2, 3;";
    
    if (sqlite3_prepare_v2(db1, fuzz_str_11, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use sqlite3_step
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row data if needed
        }
        
        // Use sqlite3_reset
        sqlite3_reset(stmt);
        
        // Use sqlite3_finalize
        sqlite3_finalize(stmt);
    }
    
    // 9. Check transaction state
    sqlite3_txn_state(db1, fuzz_str_12);
    
    // 10. Get SQLite status
    int current = 0;
    int highwater = 0;
    sqlite3_status(fuzz_int32_t_13, &current, &highwater, fuzz_int32_t_14);
    
    // 11. Clean up and close databases
    free(profile_data);
    sqlite3_close(db1);
    sqlite3_close(db2);
    
    // 12. Remove temporary file
    remove(temp_db_name);
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}