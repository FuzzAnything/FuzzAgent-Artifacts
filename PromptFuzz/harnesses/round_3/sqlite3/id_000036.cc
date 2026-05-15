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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 252, nr_unique_branch: 24
//<Quality> {"density":28,"unique_branches":{"sqlite3VdbeExec":[[97885,7,97885,11,0,0,4,0],[97886,9,97886,31,0,0,4,1],[97889,9,97889,31,0,0,4,1],[97889,35,97889,48,0,0,4,1],[97923,7,97923,11,0,0,4,0],[97927,12,97927,39,0,0,4,0],[97927,12,97927,39,0,0,4,1],[98018,7,98018,17,0,0,4,0],[98019,7,98019,50,0,0,4,0],[98039,9,98039,43,0,0,4,1],[92294,57,92294,78,596,0,4,0],[92294,57,92294,78,617,0,4,0]],"blobSeekToRow":[[101363,7,101363,14,0,0,4,0]],"sqlite3BtreeTableMoveto":[[75452,9,75452,32,0,0,4,0]],"sqlite3BtreeInsert":[[79083,9,79083,26,0,0,4,0]],"sqlite3_update_hook":[[179900,7,179900,32,0,0,4,1]],"sqlite3_rollback_hook":[[179925,7,179925,32,0,0,4,1]],"sqlite3_blob_reopen":[[101784,7,101784,11,0,0,4,1],[101788,7,101788,18,0,0,4,1],[101797,9,101797,22,0,0,4,1]],"sqlite3RollbackAll":[[179097,7,179097,28,0,0,4,0],[179097,33,179097,40,0,0,4,0],[179097,33,179097,40,0,0,4,1],[179097,44,179097,59,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_sourceid","sqlite3_rollback_hook","sqlite3_update_hook","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_busy","sqlite3_step","sqlite3_stmt_busy","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_read","sqlite3_blob_close","sqlite3_strnicmp","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_sourceid","sqlite3_rollback_hook","sqlite3_update_hook","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_busy","sqlite3_step","sqlite3_stmt_busy","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_read","sqlite3_blob_close","sqlite3_strnicmp","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


// Rollback hook callback
static void  id_000036_rollback_hook_callback(void* user_data) {
    // This is called when a transaction is rolled back
    // We can use user_data to pass information
}

// Update hook callback
static void  id_000036_update_hook_callback(void* user_data, int op_type, 
                                 const char* db_name, 
                                 const char* table_name, 
                                 sqlite3_int64 rowid) {
    // This is called when a row is updated, inserted, or deleted
    // We can use user_data to pass information
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<161) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Use the input data for various operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a temporary database in memory
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use sqlite3_sourceid() - get the source ID string
    const char* source_id = sqlite3_sourceid();
    if (source_id) {
        // Use the source ID in some way
    }
    
    // Set up rollback hook
    void* rollback_user_data = nullptr;
    sqlite3_rollback_hook(db,  id_000036_rollback_hook_callback, rollback_user_data);
    
    // Set up update hook
    void* update_user_data = nullptr;
    sqlite3_update_hook(db,  id_000036_update_hook_callback, update_user_data);
    
    // Create a simple table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    // Insert some data using the input
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the input data as a blob
        sqlite3_bind_blob(stmt, fuzz_int32_t_2, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Check if a statement is busy
    stmt = nullptr;
    const char* select_sql = "SELECT * FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Check if statement is busy (should be false before stepping)
        int is_busy = sqlite3_stmt_busy(stmt);
        
        // Step through the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row if needed
        }
        
        // Check again if statement is busy (should be false after finishing)
        is_busy = sqlite3_stmt_busy(stmt);
        
        sqlite3_finalize(stmt);
    }
    
    // Create a blob handle for the inserted data
    sqlite3_blob* blob = nullptr;
    rc = sqlite3_blob_open(db, fuzz_str_3, fuzz_str_4, fuzz_str_5, fuzz_int64_t_6, fuzz_int32_t_7, &blob);
    if (rc == SQLITE_OK && blob) {
        // Try to reopen the blob to a different row (if possible)
        // For demonstration, try to reopen to row 1 (which we just inserted)
        rc = sqlite3_blob_reopen(blob, fuzz_int64_t_8);
        
        // Read some data from the blob using the input data
        if (size > 0) {
            uint8_t buffer[256];
            int to_read = (int)(size > sizeof(buffer) ? sizeof(buffer) : size);
            rc = sqlite3_blob_read(blob, buffer, to_read, fuzz_int32_t_9);
        }
        
        sqlite3_blob_close(blob);
    }
    
    // Use sqlite3_strnicmp for string comparison
    // Compare part of the source ID with a constant string
    if (source_id && size > 0) {
        // Create a test string from the input data
        char test_str[64];
        size_t copy_len = size > sizeof(test_str) - 1 ? sizeof(test_str) - 1 : size;
        memcpy(test_str, data, copy_len);
        test_str[copy_len] = '\0';
        
        // Compare case-insensitively
        int cmp_result = sqlite3_strnicmp(source_id, test_str, 
                                         (int)(copy_len > 10 ? 10 : copy_len));
        
        // Use the comparison result
        (void)cmp_result; // Suppress unused variable warning
    }
    
    // Simulate getting a journal filename (for a hypothetical file)
    // Note: sqlite3_filename_journal expects a sqlite3_filename type
    // We'll create a dummy filename from the input data
    if (size > 0) {
        char dummy_filename[256];
        size_t filename_len = size > sizeof(dummy_filename) - 1 ? 
                             sizeof(dummy_filename) - 1 : size;
        memcpy(dummy_filename, data, filename_len);
        dummy_filename[filename_len] = '\0';
        
        // In real usage, we would pass this to sqlite3_filename_journal
        // but we need a valid sqlite3_filename which we don't have here
        // const char* journal_name = sqlite3_filename_journal((sqlite3_filename)dummy_filename);
    }
    
    // Perform a transaction with rollback to trigger the rollback hook
    sqlite3_exec(db, fuzz_str_10, nullptr, nullptr, nullptr);
    
    // Insert another row
    stmt = nullptr;
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use remaining data or a portion of it
        size_t offset = size > 512 ? 512 : 0;
        size_t remaining = size > offset ? size - offset : 0;
        
        if (remaining > 0) {
            sqlite3_bind_blob(stmt, fuzz_int32_t_11, data + offset, 
                             (int)(remaining > 512 ? 512 : remaining), 
                             SQLITE_STATIC);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    
    // Rollback to trigger the rollback hook
    sqlite3_exec(db, fuzz_str_12, nullptr, nullptr, nullptr);
    
    // Update a row to trigger the update hook
    const char* update_sql = "UPDATE test SET data = ? WHERE id = 1;";
    stmt = nullptr;
    if (sqlite3_prepare_v2(db, update_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use a different portion of the input data
        size_t update_offset = size > 256 ? 256 : 0;
        size_t update_size = size > update_offset ? size - update_offset : 0;
        
        if (update_size > 0) {
            sqlite3_bind_blob(stmt, fuzz_int32_t_13, data + update_offset, 
                             (int)(update_size > 256 ? 256 : update_size), 
                             SQLITE_STATIC);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}