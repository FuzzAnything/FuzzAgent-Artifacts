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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 572, nr_unique_branch: 74
//<Quality> {"density":26,"unique_branches":{"accessPayload":[[74842,9,74842,35,0,0,4,1],[74854,24,74854,29,0,0,4,1],[74981,24,74981,29,0,0,4,1]],"sqlite3BtreeLockTable":[[81008,7,81008,18,0,0,4,1]],"sqlite3MallocEnd":[[30003,7,30003,38,0,0,4,0]],"sqlite3VdbeExec":[[94276,1,94276,21,0,0,4,0],[94278,7,94278,42,0,0,4,1],[95266,29,95266,41,0,0,4,1],[95632,11,95632,40,0,0,4,0],[95632,44,95632,67,0,0,4,1]],"blobSeekToRow":[[101363,7,101363,14,0,0,4,1],[101370,7,101370,21,0,0,4,0],[101375,12,101375,34,0,0,4,0],[101378,9,101378,16,0,0,4,1],[101393,7,101393,21,0,0,4,0],[14098,27,14098,28,2,0,4,1],[14098,27,14098,28,3,0,4,1]],"sqlite3_blob_close":[[101656,7,101656,8,0,0,4,0]],"sqlite3ErrorWithMsg":[[34631,7,34631,17,0,0,4,0]],"sqlite3_reset_auto_extension":[[136552,7,136552,38,0,0,4,0]],"sqlite3_shutdown":[[177984,7,177984,33,0,0,4,0],[177993,7,177993,39,0,0,4,0],[177997,7,177997,39,0,0,4,0],[178013,7,178013,38,0,0,4,0]],"sqlite3ParseObjectInit":[[140882,7,140882,23,0,0,4,1]],"sqlite3BtreePayloadChecked":[[75033,7,75033,33,0,0,4,0]],"sqlite3JournalSize":[[14679,20,14679,27,1,0,4,1]],"sqlite3_blob_open":[[101434,7,101434,16,0,0,4,1],[101440,7,101440,32,0,0,4,1],[101440,36,101440,45,0,0,4,1],[101440,49,101440,59,0,0,4,1],[101451,9,101451,15,0,0,4,1],[101457,9,101457,13,0,0,4,0],[101461,9,101461,13,0,0,4,0],[101461,17,101461,26,0,0,4,1],[101466,9,101466,13,0,0,4,0],[101471,9,101471,14,0,0,4,1],[101485,17,101485,32,0,0,4,0],[101486,11,101486,63,0,0,4,0],[101486,11,101486,63,0,0,4,1],[101490,9,101490,25,0,0,4,1],[101501,9,101501,15,0,0,4,0],[101505,11,101505,39,0,0,4,1],[101522,30,101522,34,0,0,4,1],[101531,11,101531,17,0,0,4,1],[101542,9,101542,21,0,0,4,0],[101583,11,101583,30,0,0,4,0],[101594,11,101594,30,0,0,4,0],[101599,13,101599,19,0,0,4,0],[101624,9,101624,25,0,0,4,1],[101628,9,101628,46,0,0,4,1],[101628,50,101628,67,0,0,4,0],[101633,7,101633,20,0,0,4,0],[101633,24,101633,43,0,0,4,0],[101639,32,101639,36,0,0,4,1],[18331,29,18331,57,4,0,4,1],[18322,29,18322,57,6,0,4,1]],"sqlite3_vfs_unregister":[[26077,7,26077,9,0,0,4,1]],"sqlite3_bind_double":[[91193,7,91193,20,0,0,4,0]],"sqlite3MutexEnd":[[28609,7,28609,42,0,0,4,0]],"sqlite3ColumnDefault":[[151622,7,151622,38,0,0,4,0],[151622,42,151622,52,0,0,4,0]],"blobReadWrite":[[101684,7,101684,11,0,0,4,1],[101689,7,101689,10,0,0,4,1],[101689,14,101689,23,0,0,4,1],[101689,27,101689,62,0,0,4,1],[101692,13,101692,17,0,0,4,1],[101705,9,101705,35,0,0,4,1],[101731,9,101731,25,0,0,4,1]],"sqlite3_blob_bytes":[[101766,11,101766,12,0,0,4,0],[101766,16,101766,24,0,0,4,0]],"vfsUnlink":[[26031,24,26031,38,0,0,4,1],[26034,9,26034,23,0,0,4,0]],"sqlite3PcacheShutdown":[[53852,7,53852,44,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_malloc","sqlite3_shutdown","sqlite3_vfs_register","sqlite3_free","sqlite3_shutdown","sqlite3_open_v2","sqlite3_vfs_unregister","sqlite3_free","sqlite3_shutdown","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_double","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_origin_name","sqlite3_column_value","sqlite3_value_frombind","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_malloc","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_finalize","sqlite3_close","sqlite3_vfs_unregister","sqlite3_free","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_malloc","sqlite3_vfs_register","sqlite3_open_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_double","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_origin_name","sqlite3_column_value","sqlite3_value_frombind","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_malloc","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_finalize","sqlite3_close","sqlite3_vfs_unregister","sqlite3_free","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<184) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
	//fuzzer shim end}




    // Create a temporary file from the input data
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
    
    // Create a custom VFS that uses our input file
    sqlite3_vfs *custom_vfs = (sqlite3_vfs *)sqlite3_malloc(sizeof(sqlite3_vfs));
    if (!custom_vfs) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    memset(custom_vfs, 0, sizeof(sqlite3_vfs));
    custom_vfs->iVersion = 3;
    custom_vfs->szOsFile = sizeof(FILE*);
    custom_vfs->mxPathname = 1024;
    custom_vfs->zName = "custom_vfs";
    
    // Register the custom VFS
    if (sqlite3_vfs_register(custom_vfs, fuzz_int32_t_1) != SQLITE_OK) {
        sqlite3_free(custom_vfs);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Open database using the custom VFS
    sqlite3 *db = nullptr;
    if (sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, "custom_vfs") != SQLITE_OK) {
        sqlite3_vfs_unregister(custom_vfs);
        sqlite3_free(custom_vfs);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create a table and insert some data
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "data BLOB, "
        "value REAL);";
    
    char *err_msg = nullptr;
    if (sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
    
    // Insert some data
    const char *insert_sql = 
        "INSERT INTO test_table (name, data, value) VALUES (?, ?, ?);";
    
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind some values
        const char *name = "test_name";
        const char *blob_data = "test_blob_data";
        double value = 3.14159;
        
        sqlite3_bind_text(stmt, fuzz_int32_t_3, name, -1, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, fuzz_int32_t_5, fuzz_str_4, static_cast<int>(fuzz_str_sz_4), SQLITE_STATIC);
        sqlite3_bind_double(stmt, fuzz_int32_t_6, value);
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Query the data and use sqlite3_column_origin_name
    const char *select_sql = "SELECT * FROM test_table;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                // Use sqlite3_column_origin_name
                const char *origin_name = sqlite3_column_origin_name(stmt, fuzz_int32_t_7);
                if (origin_name) {
                    // Do something with origin_name
                }
                
                // Check if value is from bind using sqlite3_value_frombind
                sqlite3_value *col_value = sqlite3_column_value(stmt, fuzz_int32_t_8);
                if (col_value) {
                    int from_bind = sqlite3_value_frombind(col_value);
                    // Use from_bind result
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Create a BLOB and use sqlite3_blob_bytes
    sqlite3_blob *blob = nullptr;
    if (sqlite3_blob_open(db, fuzz_str_9, fuzz_str_10, fuzz_str_11, fuzz_int64_t_12, fuzz_int32_t_13, &blob) == SQLITE_OK) {
        int blob_size = sqlite3_blob_bytes(blob);
        if (blob_size > 0) {
            char *buffer = (char *)sqlite3_malloc(blob_size);
            if (buffer) {
                sqlite3_blob_read(blob, buffer, blob_size, fuzz_int32_t_14);
                sqlite3_free(buffer);
            }
        }
        sqlite3_blob_close(blob);
    }
    
    // Create an EXPLAIN statement and use sqlite3_stmt_isexplain
    const char *explain_sql = "EXPLAIN SELECT * FROM test_table;";
    if (sqlite3_prepare_v2(db, explain_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        int is_explain = sqlite3_stmt_isexplain(stmt);
        // Use is_explain result
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_vfs_unregister(custom_vfs);
    sqlite3_free(custom_vfs);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}