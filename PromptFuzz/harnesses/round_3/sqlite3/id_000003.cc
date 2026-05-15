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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 77, nr_unique_branch: 18
//<Quality> {"density":14,"unique_branches":{"sqlite3StartTable":[[121372,25,121372,31,0,0,4,0],[121372,35,121372,46,0,0,4,1],[121378,25,121378,31,0,0,4,0]],"sqlite3Pragma":[[137689,7,137689,13,0,0,4,0],[137689,17,137689,48,0,0,4,1]],"sqlite3OpenTempDatabase":[[125345,7,125345,24,0,0,4,1]],"sqlite3PagerSnapshotGet":[[64141,7,64141,19,0,0,4,1]],"sqlite3VdbeExec":[[96375,7,96375,17,0,0,4,0]],"sqlite3FindTable":[[120537,9,120537,10,0,0,4,0]],"sqlite3_snapshot_get":[[182493,11,182493,54,0,0,4,0],[182495,13,182495,26,0,0,4,0],[182495,13,182495,26,0,0,4,1]],"sqlite3Prepare":[[140932,7,140932,44,0,0,4,0]],"yy_reduce":[[174034,7,174034,14,0,0,4,0],[174206,7,174206,14,0,0,4,0],[174246,7,174246,14,0,0,4,0],[14098,27,14098,28,260,0,4,0]],"vdbeCommit":[[86899,11,86899,57,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_close_v2","sqlite3_limit","sqlite3_limit","sqlite3_get_autocommit","sqlite3_db_status","sqlite3_prepare_v3","sqlite3_column_count","sqlite3_step","sqlite3_column_text","sqlite3_exec","sqlite3_free","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_limit","sqlite3_limit","sqlite3_get_autocommit","sqlite3_db_status","sqlite3_prepare_v3","sqlite3_column_count","sqlite3_step","sqlite3_column_text","sqlite3_exec","sqlite3_free","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close_v2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<184) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open output database file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy input data to output file to simulate database file
    fseek(in_file, 0, SEEK_SET);
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Now work with the database file
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_snapshot *snapshot = nullptr;
    const char *tail = nullptr;
    
    // Open the database
    int rc = sqlite3_open_v2(fuzz_str_1, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close_v2(db);
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // 1. Use sqlite3_limit to set some limits
    int old_limit = sqlite3_limit(db, fuzz_int32_t_2, fuzz_int32_t_3);
    sqlite3_limit(db, fuzz_int32_t_4, fuzz_int32_t_5);
    
    // 2. Check autocommit mode
    int autocommit = sqlite3_get_autocommit(db);
    
    // 3. Get database status
    int current = 0, highwater = 0;
    sqlite3_db_status(db, fuzz_int32_t_6, &current, &highwater, fuzz_int32_t_7);
    
    // 4. Prepare a statement using input data as SQL
    // Use the first part of input data as SQL string (ensure null-terminated)
    if (size > 0) {
        char *sql = (char *)malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            
            // Try to prepare statement
            rc = sqlite3_prepare_v3(db, sql, -1, fuzz_uint32_t_8, &stmt, &tail);
            
            // 5. Get column count if statement was prepared successfully
            if (rc == SQLITE_OK && stmt) {
                int col_count = sqlite3_column_count(stmt);
                
                // Try to step through the statement to get values
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    // 6. Get value subtype if we have a column
                    if (col_count > 0) {
                        // Create a value from the first column
                        const char *col_text = (const char *)sqlite3_column_text(stmt, fuzz_int32_t_9);
                        if (col_text) {
                            // We need a sqlite3_value* to use sqlite3_value_subtype
                            // Since we don't have direct access, we'll create a simple test
                            // using sqlite3_exec to create a function that returns a subtype
                            char *err_msg = nullptr;
                            sqlite3_exec(db, 
                                fuzz_str_10, 
                                nullptr, nullptr, &err_msg);
                            if (err_msg) sqlite3_free(err_msg);
                        }
                    }
                }
            }
            free(sql);
        }
    }
    
    // 7. Try to get a snapshot (may fail if not in WAL mode or no transaction)
    sqlite3_snapshot_get(db, fuzz_str_11, &snapshot);
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (snapshot) {
        // Note: sqlite3_snapshot_free is not in the provided list, 
        // but we should free it if we got one
        // Since it's not provided, we'll just not use it
    }
    
    if (db) {
        sqlite3_close_v2(db);
    }
    
    // Clean up temporary file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}