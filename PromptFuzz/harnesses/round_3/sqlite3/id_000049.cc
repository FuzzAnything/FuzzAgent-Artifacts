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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 5
//<Quality> {"density":20,"unique_branches":{"sqlite3_set_clientdata":[[181368,22,181368,23,0,0,4,0],[181368,27,181368,49,0,0,4,1],[181371,7,181371,8,0,0,4,0],[181373,9,181373,23,0,0,4,0],[181374,9,181374,17,0,0,4,0]]},"library_calls":["sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_close","sqlite3_close","sqlite3_set_clientdata","sqlite3_create_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_text","sqlite3_column_bytes","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_hard_heap_limit64","sqlite3_set_clientdata","sqlite3_close"],"critical_calls":["sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_set_clientdata","sqlite3_create_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_text","sqlite3_column_bytes","sqlite3_finalize","sqlite3_hard_heap_limit64","sqlite3_set_clientdata","sqlite3_close"],"visited":0}
/**/


// Custom function to demonstrate sqlite3_result_* APIs
static void  id_000049_custom_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // Get client data (set during function creation)
    void *client_data = sqlite3_get_clientdata(sqlite3_context_db_handle(context), "test_key");
    
    if (argc > 0) {
        const void *input_data = sqlite3_value_blob(argv[0]);
        int data_size = sqlite3_value_bytes(argv[0]);
        
        if (data_size > 100) {
            // Return an error if data is too large
            sqlite3_result_error(context, "Input data too large", -1);
        } else if (data_size > 0) {
            // Return text16be result
            sqlite3_result_text16be(context, input_data, data_size, SQLITE_TRANSIENT);
        } else {
            // Return a pointer result (using client_data if available)
            const char *type_name = "test_pointer_type";
            void *ptr_to_return = client_data ? client_data : (void*)context;
            sqlite3_result_pointer(context, ptr_to_return, type_name, nullptr);
        }
    } else {
        // No arguments - return NULL
        sqlite3_result_null(context);
    }
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}




    // Set hard heap limit
    sqlite3_int64 original_limit = sqlite3_hard_heap_limit64(1024 * 1024); // 1MB limit
    
    // Open in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a FILE* to read input data if needed
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set client data
    char *test_data = static_cast<char*>(malloc(100));
    if (test_data) {
        strcpy(test_data, "test_client_data");
        sqlite3_set_clientdata(db, fuzz_str_1, test_data, free);
    }
    
    // Register custom function
    sqlite3_create_function(
        db,
        fuzz_str_2,
        fuzz_int32_t_3,  // Number of arguments
        fuzz_int32_t_4,
        nullptr,  // pAppData (can be used instead of clientdata)
         id_000049_custom_function,
        nullptr,
        nullptr
    );
    
    // Create a table and use the custom function with input data
    char *err_msg = nullptr;
    sqlite3_exec(db, "CREATE TABLE test (id INTEGER, data BLOB)", nullptr, nullptr, &err_msg);
    
    // Prepare statement to insert fuzzer data
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test VALUES (1, ?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_blob(stmt, fuzz_int32_t_5, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Use custom function with the inserted data
    const char *select_sql = "SELECT custom_func(data) FROM test";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get result (could be text16be, error, or pointer)
            int result_type = sqlite3_column_type(stmt, fuzz_int32_t_6);
            
            // Write result to output file based on type
            switch (result_type) {
                case SQLITE_TEXT: {
                    const void *text_result = sqlite3_column_text(stmt, 0);
                    int text_len = sqlite3_column_bytes(stmt, fuzz_int32_t_7);
                    fwrite(text_result, 1, text_len, out_file);
                    break;
                }
                case SQLITE_BLOB: {
                    const void *blob_result = sqlite3_column_blob(stmt, fuzz_int32_t_8);
                    int blob_len = sqlite3_column_bytes(stmt, fuzz_int32_t_9);
                    fwrite(blob_result, 1, blob_len, out_file);
                    break;
                }
                // For pointer types, we write a marker
                case SQLITE_NULL:
                    fwrite("NULL_RESULT", 1, 11, out_file);
                    break;
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Read from input file and write to output file (demonstrating FILE* usage)
    fseek(in_file, 0, SEEK_SET);
    uint8_t buffer[1024];
    size_t bytes_read = fread(buffer, 1, size > 1024 ? 1024 : size, in_file);
    fwrite(buffer, 1, bytes_read, out_file);
    
    // Get file descriptors (demonstrating fuzz_fileno usage)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // Restore original heap limit
    sqlite3_hard_heap_limit64(original_limit);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clear client data
    sqlite3_set_clientdata(db, fuzz_str_10, nullptr, nullptr);
    
    // Close database
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}