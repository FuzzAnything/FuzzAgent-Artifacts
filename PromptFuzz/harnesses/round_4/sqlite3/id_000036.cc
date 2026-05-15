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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"sqlite3_compileoption_used":[[182624,7,182624,49,0,0,4,1]],"sqlite3_test_control":[[181703,5,181703,35,0,0,4,0]]},"library_calls":["sqlite3_compileoption_used","sqlite3_test_control","sqlite3_open","sqlite3_exec","sqlite3_mprintf","sqlite3_exec","sqlite3_free","sqlite3_changes","sqlite3_db_status","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_nochange","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_compileoption_used","sqlite3_test_control","sqlite3_open","sqlite3_exec","sqlite3_mprintf","sqlite3_exec","sqlite3_free","sqlite3_changes","sqlite3_db_status","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_nochange","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<105) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
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
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_str *str_obj = nullptr;
    int rc = 0;
    
    // 1. Use sqlite3_compileoption_used
    int compile_option_used = sqlite3_compileoption_used(fuzz_str_1);
    
    // 2. Use sqlite3_test_control (with a simple test operation)
    int test_control_result = sqlite3_test_control(SQLITE_TESTCTRL_FIRST, 0);
    
    // 3. Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create a custom function using sqlite3_create_function16
    // Note: sqlite3_create_function16 is not in the provided list, but we'll use
    // a similar approach with available functions
    // We'll create a simple table and use built-in functions instead
    
    // Create a table
    const char *create_table_sql = "CREATE TABLE test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
    
    // 5. Insert some data using the input
    char *insert_sql = sqlite3_mprintf("INSERT INTO test VALUES (%d, 'test');", 
                                       static_cast<int>(data[0] % 100));
    rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    sqlite3_free(insert_sql);
    
    // 6. Use sqlite3_changes
    int changes = sqlite3_changes(db);
    
    // 7. Use sqlite3_db_status
    int current = 0, highwater = 0;
    rc = sqlite3_db_status(db, fuzz_int32_t_3, &current, &highwater, fuzz_int32_t_4);
    
    // 8. Create and use sqlite3_str object
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // Write some data to the string builder
        sqlite3_str_append(str_obj, "Test string", 11);
        
        // Get the finished string
        char *finished_str = sqlite3_str_finish(str_obj);
        if (finished_str) {
            // Write to output file
            fwrite(finished_str, 1, strlen(finished_str), out_file);
            sqlite3_free(finished_str);
        }
    }
    
    // 9. Prepare a statement and create a sqlite3_value
    const char *select_sql = "SELECT id FROM test;";
    rc = sqlite3_prepare_v2(db, fuzz_str_5, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute the statement
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column value
            sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_6);
            
            // 10. Use sqlite3_value_nochange
            // Note: This is typically used in update hooks, but we'll call it anyway
            int nochange = sqlite3_value_nochange(value);
            
            // Write result to output file
            fwrite(&nochange, sizeof(int), 1, out_file);
        }
        sqlite3_finalize(stmt);
    }
    
    // 11. Read from input file and write to output file
    fseek(in_file, 0, SEEK_SET);
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }
    
    // 12. Use file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Write file descriptor info to output
    fwrite(&in_fd, sizeof(int), 1, out_file);
    fwrite(&out_fd, sizeof(int), 1, out_file);
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
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