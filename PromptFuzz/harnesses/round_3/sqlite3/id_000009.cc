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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 5
//<Quality> {"density":30,"unique_branches":{"sqlite3_clear_bindings":[[89629,12,89629,21,0,0,4,1],[89634,7,89634,17,0,0,4,1]],"sqlite3_strglob":[[128272,7,128272,17,0,0,4,1],[128274,13,128274,28,0,0,4,1]],"sqlite3KeywordCode":[[176463,7,176463,11,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_malloc","sqlite3_shutdown","sqlite3_open_v2","sqlite3_free","sqlite3_shutdown","sqlite3_prepare_v2","sqlite3_close","sqlite3_free","sqlite3_shutdown","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_bind_zeroblob64","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_origin_name","sqlite3_column_value","sqlite3_value_frombind","sqlite3_blob_open","sqlite3_column_int64","sqlite3_blob_bytes","sqlite3_malloc64","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_finalize","sqlite3_complete","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_strglob","sqlite3_keyword_count","sqlite3_keyword_check","sqlite3_close","sqlite3_free","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_malloc","sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_origin_name","sqlite3_column_value","sqlite3_value_frombind","sqlite3_blob_open","sqlite3_column_int64","sqlite3_blob_bytes","sqlite3_malloc64","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_finalize","sqlite3_complete","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_strglob","sqlite3_keyword_count","sqlite3_keyword_check","sqlite3_close","sqlite3_free","sqlite3_shutdown"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<105) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
	//fuzzer shim end}




    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for database operations
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

    // Create a custom VFS that uses our file descriptors
    sqlite3_vfs *custom_vfs = static_cast<sqlite3_vfs*>(sqlite3_malloc(sizeof(sqlite3_vfs)));
    if (!custom_vfs) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize custom VFS structure (simplified for fuzzing)
    memset(custom_vfs, 0, sizeof(sqlite3_vfs));
    custom_vfs->iVersion = 3;
    custom_vfs->szOsFile = sizeof(FILE*);
    custom_vfs->mxPathname = 256;
    custom_vfs->zName = "fuzz_vfs";
    
    // Note: sqlite3_vfs_register is not in the provided API list, so we'll skip it
    // and use the default VFS instead for database operations

    // Open a database using the default VFS
    sqlite3 *db = nullptr;
    if (sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        sqlite3_free(custom_vfs);
        sqlite3_shutdown();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create a table with a BLOB column
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "name TEXT);";
    
    sqlite3_stmt *create_stmt = nullptr;
    if (sqlite3_prepare_v2(db, create_table_sql, -1, &create_stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_free(custom_vfs);
        sqlite3_shutdown();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    sqlite3_step(create_stmt);
    sqlite3_finalize(create_stmt);

    // Insert data using a prepared statement
    sqlite3_stmt *insert_stmt = nullptr;
    const char *insert_sql = "INSERT INTO fuzz_table (data, name) VALUES (?, ?);";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr) == SQLITE_OK) {
        // Bind the fuzzer data as a BLOB
        if (size > 0) {
            sqlite3_bind_blob64(insert_stmt, fuzz_int32_t_1, data, static_cast<sqlite3_uint64>(size), SQLITE_STATIC);
        } else {
            sqlite3_bind_zeroblob64(insert_stmt, fuzz_int32_t_2, fuzz_uint64_t_3);
        }
        
        // Bind a text parameter
        const char *test_name = "fuzz_test";
        sqlite3_bind_text(insert_stmt, fuzz_int32_t_4, test_name, -1, SQLITE_STATIC);
        
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }

    // Query the inserted data
    sqlite3_stmt *select_stmt = nullptr;
    const char *select_sql = "SELECT * FROM fuzz_table;";
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr) == SQLITE_OK) {
        // Check if statement is an EXPLAIN statement
        int is_explain = sqlite3_stmt_isexplain(select_stmt);
        (void)is_explain;  // Use the result to avoid unused variable warning
        
        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // Get column origin name for the data column (index 1)
            const char *origin_name = sqlite3_column_origin_name(select_stmt, fuzz_int32_t_5);
            (void)origin_name;  // Use the result
            
            // Get the BLOB value from the column
            sqlite3_value *value = sqlite3_column_value(select_stmt, fuzz_int32_t_6);
            if (value) {
                // Check if the value is from a bind parameter
                int from_bind = sqlite3_value_frombind(value);
                (void)from_bind;  // Use the result
                
                // Open the BLOB for reading
                sqlite3_blob *blob = nullptr;
                if (sqlite3_blob_open(db, fuzz_str_7, fuzz_str_8, fuzz_str_9, 
                                      sqlite3_column_int64(select_stmt, fuzz_int32_t_11), 
                                      fuzz_int32_t_10, &blob) == SQLITE_OK) {
                    // Get the size of the BLOB
                    int blob_bytes = sqlite3_blob_bytes(blob);
                    (void)blob_bytes;  // Use the result
                    
                    // Read a portion of the BLOB (if it exists)
                    if (blob_bytes > 0) {
                        uint8_t *buffer = static_cast<uint8_t*>(sqlite3_malloc64(blob_bytes));
                        if (buffer) {
                            sqlite3_blob_read(blob, buffer, blob_bytes, fuzz_int32_t_12);
                            sqlite3_free(buffer);
                        }
                    }
                    
                    sqlite3_blob_close(blob);
                }
            }
        }
        sqlite3_finalize(select_stmt);
    }

    // Test some other APIs that are in the provided list
    const char *test_sql = "SELECT 1;";
    if (sqlite3_complete(fuzz_str_13)) {
        // Prepare another statement to test more APIs
        sqlite3_stmt *test_stmt = nullptr;
        if (sqlite3_prepare_v2(db, test_sql, -1, &test_stmt, nullptr) == SQLITE_OK) {
            // Test binding parameters
            sqlite3_bind_int(test_stmt, fuzz_int32_t_14, fuzz_int32_t_15);
            sqlite3_clear_bindings(test_stmt);
            sqlite3_finalize(test_stmt);
        }
    }

    // Test string glob matching
    const char *glob_pattern = "test*";
    const char *test_string = "test123";
    int glob_result = sqlite3_strglob(fuzz_str_16, fuzz_str_17);
    (void)glob_result;

    // Test keyword checking
    int keyword_count = sqlite3_keyword_count();
    (void)keyword_count;
    
    if (keyword_count > 0) {
        int is_keyword = sqlite3_keyword_check("SELECT", 6);
        (void)is_keyword;
    }

    // Clean up
    sqlite3_close(db);
    sqlite3_free(custom_vfs);
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}