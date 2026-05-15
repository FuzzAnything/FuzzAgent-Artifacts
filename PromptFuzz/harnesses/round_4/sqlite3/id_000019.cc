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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 6
//<Quality> {"density":17,"unique_branches":{"sqlite3VdbeSerialGet":[[88087,5,88087,11,0,0,4,0],[14098,27,14098,28,19,0,4,0],[14098,27,14098,28,19,0,4,1]],"sqlite3VdbeExec":[[95735,17,95735,38,0,0,4,0],[95856,11,95856,17,0,0,4,0]],"sqlite3VdbeRealValue":[[82613,13,82613,45,0,0,4,1]]},"library_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_double","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_free","sqlite3_finalize","sqlite3_changes64","sqlite3_cancel_auto_extension","sqlite3_close"],"critical_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_double","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_free","sqlite3_finalize","sqlite3_changes64","sqlite3_cancel_auto_extension","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<240) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* to write output data
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
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used

    // Initialize SQLite
    if (sqlite3_shutdown() != SQLITE_OK) {
        // Continue anyway
    }
    
    // Open an in-memory database
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

    // Create a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "value REAL, "
        "data BLOB"
        ");";
    
    sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);

    // Prepare a statement to insert data
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test_table (value, data) VALUES (?, ?)";
    
    if (sqlite3_prepare_v2(db, fuzz_str_3, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind a double value from input data
        double double_value = 0.0;
        if (size >= sizeof(double)) {
            memcpy(&double_value, data, sizeof(double));
        }
        sqlite3_bind_double(stmt, fuzz_int32_t_4, double_value);
        
        // Bind a zeroblob using available API (simulating zeroblob64 behavior)
        sqlite3_bind_zeroblob(stmt, fuzz_int32_t_5, size > 100 ? 100 : (int)size);
        
        // Execute the statement
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // Query the data back
    const char *select_sql = "SELECT value, data FROM test_table";
    if (sqlite3_prepare_v2(db, fuzz_str_6, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column values
            double col_value = sqlite3_column_double(stmt, fuzz_int32_t_7);
            const void *blob_data = sqlite3_column_blob(stmt, fuzz_int32_t_8);
            int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_9);
            
            // Write to output file
            if (blob_data && blob_size > 0) {
                fwrite(blob_data, 1, blob_size, out_file);
            }
        }
        sqlite3_finalize(stmt);
    }

    // Create another statement for auxiliary data demonstration
    const char *aux_sql = "SELECT sqlite_version()";
    if (sqlite3_prepare_v2(db, fuzz_str_10, -1, &stmt, nullptr) == SQLITE_OK) {
        // Create a user-defined function context (simulated)
        sqlite3_context *context = nullptr;
        // Note: In real usage, we'd need to create a function to get a context
        
        // Allocate memory using sqlite3_malloc
        void *allocated_mem = sqlite3_malloc(fuzz_int32_t_11);
        if (allocated_mem) {
            // Copy some input data to allocated memory
            size_t copy_size = size > 256 ? 256 : size;
            memcpy(allocated_mem, data, copy_size);
            
            // Free the memory
            sqlite3_free(allocated_mem);
        }
        
        sqlite3_finalize(stmt);
    }

    // Get changes count
    sqlite3_int64 changes = sqlite3_changes64(db);
    (void)changes;  // Mark as used

    // Try to cancel auto extension (will likely return 0 if none registered)
    int cancel_result = sqlite3_cancel_auto_extension(nullptr);
    (void)cancel_result;  // Mark as used

    // Clean up
    sqlite3_close(db);
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