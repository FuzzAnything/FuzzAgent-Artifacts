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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"sqlite3_get_auxdata":[[90620,7,90620,14,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_bind_text","sqlite3_bind_double","sqlite3_bind_blob","sqlite3_step","sqlite3_changes64","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_get_auxdata","sqlite3_cancel_auto_extension","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_bind_text","sqlite3_bind_double","sqlite3_bind_blob","sqlite3_step","sqlite3_changes64","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_get_auxdata","sqlite3_cancel_auto_extension","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<234) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
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
    
    // Initialize SQLite database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(fuzz_str_1, &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create a table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
    
    // Prepare a statement for inserting data
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test_table (name, value, data) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_3, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind parameters using the input data
        if (size > 0) {
            // Use sqlite3_malloc to allocate memory
            void *allocated_mem = sqlite3_malloc(fuzz_int32_t_4);
            if (allocated_mem) {
                // Copy some data to allocated memory
                size_t copy_size = size < 100 ? size : 100;
                memcpy(allocated_mem, data, copy_size);
                
                // Bind text parameter
                const char *name = "test_name";
                sqlite3_bind_text(stmt, fuzz_int32_t_6, fuzz_str_5, -1, SQLITE_STATIC);
                
                // Bind double value - use first 8 bytes as double if available
                double double_value = 0.0;
                if (size >= sizeof(double)) {
                    memcpy(&double_value, data, sizeof(double));
                }
                sqlite3_bind_double(stmt, fuzz_int32_t_7, double_value);
                
                // Bind blob parameter
                sqlite3_bind_blob(stmt, fuzz_int32_t_8, allocated_mem, copy_size, sqlite3_free);
                
                // Execute the statement
                rc = sqlite3_step(stmt);
                
                // Get changes count using sqlite3_changes64
                sqlite3_int64 changes = sqlite3_changes64(db);
                
                // Reset statement for potential reuse
                sqlite3_reset(stmt);
                
                // Now prepare a SELECT statement to use other APIs
                sqlite3_stmt *select_stmt = nullptr;
                const char *select_sql = "SELECT * FROM test_table;";
                rc = sqlite3_prepare_v2(db, fuzz_str_9, -1, &select_stmt, nullptr);
                
                if (rc == SQLITE_OK && select_stmt) {
                    // Execute SELECT
                    rc = sqlite3_step(select_stmt);
                    
                    if (rc == SQLITE_ROW) {
                        // Create a context for user-defined function simulation
                        // We'll create a simple structure to simulate sqlite3_context
                        struct MockContext {
                            sqlite3 *db;
                            sqlite3_value *value;
                        };
                        
                        MockContext mock_ctx = {db, nullptr};
                        
                        // Simulate using sqlite3_get_auxdata (though we don't have real auxdata set)
                        // We'll just call it with NULL context
                        void *aux_data = sqlite3_get_auxdata(nullptr, fuzz_int32_t_10);
                        
                        // Try to cancel auto extension (even though we likely have none)
                        int cancel_result = sqlite3_cancel_auto_extension(nullptr);
                        
                        // Note: sqlite3_column_table_name16 is not in the provided API list
                        // We'll skip it as it's not available
                        
                        // Note: sqlite3_result_zeroblob64 is not in the provided API list
                        // We'll skip it as it's not available
                        
                        // Use sqlite3_result_double if we had a real context
                        // sqlite3_result_double(&mock_ctx, double_value);
                    }
                    
                    sqlite3_finalize(select_stmt);
                }
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}