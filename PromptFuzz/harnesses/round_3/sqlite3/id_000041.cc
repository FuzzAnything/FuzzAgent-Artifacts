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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 216, nr_unique_branch: 15
//<Quality> {"density":27,"unique_branches":{"blobReadWrite":[[101705,9,101705,35,0,0,4,0],[101705,39,101705,61,0,0,4,1]],"saveAllCursors":[[70555,7,70555,14,0,0,4,0]],"sqlite3_value_pointer":[[89680,7,89681,49,0,0,4,1]],"fillInCell":[[76807,9,76807,16,0,0,4,1],[76809,15,76809,21,0,0,4,0],[76809,15,76809,21,0,0,4,1],[76821,9,76821,21,0,0,4,1]],"accessPayload":[[74962,16,74962,22,0,0,4,1]],"sqlite3BtreePutData":[[81042,7,81042,20,0,0,4,1],[81046,7,81046,33,0,0,4,1],[81068,7,81068,43,0,0,4,1],[70664,4,70664,33,1,0,4,1]],"copyPayload":[[74758,7,74758,10,0,0,4,0],[74761,9,74761,22,0,0,4,1]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_write","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_value_pointer","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_exec","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_exec","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_write","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_value_pointer","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_exec","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<192) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
	//fuzzer shim end}




    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Get SQLite version
    const char *lib_version = sqlite3_libversion();
    (void)lib_version; // Use to avoid unused variable warning

    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Set limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);

    // Set collation callback
    sqlite3_collation_needed16(db, nullptr, nullptr);

    // Create a table and insert data
    char *errmsg = nullptr;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_7, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Insert the input data as a blob
    char insert_sql[256];
    snprintf(insert_sql, sizeof(insert_sql),
             "INSERT INTO test(data) VALUES(zeroblob(%zu));", size);
    rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Get the last inserted row ID
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);

    // Open blob to write input data
    sqlite3_blob *blob = nullptr;
    rc = sqlite3_blob_open(db, fuzz_str_8, fuzz_str_9, fuzz_str_10, rowid, fuzz_int32_t_11, &blob);
    if (rc == SQLITE_OK) {
        // Read chunks from input file and write to blob
        uint8_t buffer[4096];
        size_t total_written = 0;
        size_t offset = 0;
        
        while (total_written < size) {
            size_t to_read = (size - total_written) > sizeof(buffer) ? 
                            sizeof(buffer) : (size - total_written);
            size_t bytes_read = fread(buffer, 1, to_read, in_file);
            if (bytes_read == 0) break;
            
            rc = sqlite3_blob_write(blob, buffer, bytes_read, fuzz_int32_t_12);
            if (rc != SQLITE_OK) break;
            
            total_written += bytes_read;
            offset += bytes_read;
        }
        sqlite3_blob_close(blob);
    }

    // Prepare a statement to query the data
    sqlite3_stmt *stmt = nullptr;
    const char *query_sql = "SELECT data FROM test WHERE id = ?;";
    const char *tail = nullptr;
    rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind the rowid parameter
        sqlite3_bind_int64(stmt, fuzz_int32_t_13, rowid);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // Get column value
            sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_14);
            
            // Check encoding
            int encoding = sqlite3_value_encoding(value);
            (void)encoding; // Use to avoid unused variable warning
            
            // Try to get pointer from value (will likely return null)
            const char *ptr_type = "test_pointer_type";
            void *ptr = sqlite3_value_pointer(value, fuzz_str_15);
            (void)ptr; // Use to avoid unused variable warning
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Get WAL filename (will be null for in-memory database)
    sqlite3_filename db_filename = sqlite3_db_filename(db, fuzz_str_16);
    const char *wal_filename = sqlite3_filename_wal(db_filename);
    (void)wal_filename; // Use to avoid unused variable warning

    // Execute a simple query to verify data
    char *select_sql = "SELECT length(data) FROM test;";
    rc = sqlite3_exec(db, fuzz_str_17, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
    }

    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;

    // Also close output file if we had opened one
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}