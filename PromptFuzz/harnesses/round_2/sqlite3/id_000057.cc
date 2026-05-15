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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 8
//<Quality> {"density":18,"unique_branches":{"sqlite3_str_vappendf":[[31446,13,31446,21,0,0,4,0]],"sqlite3AutoLoadExtensions":[[136579,7,136579,25,0,0,4,1],[136583,12,136583,14,0,0,4,0],[136583,12,136583,14,0,0,4,1],[136594,9,136594,27,0,0,4,1],[136602,9,136602,14,0,0,4,0],[136602,18,136602,55,0,0,4,0]],"openDatabase":[[181114,9,181114,22,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_auto_extension","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_realloc64","sqlite3_free","sqlite3_free","sqlite3_str_new","sqlite3_str_append","sqlite3_str_value","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_last_insert_rowid","sqlite3_is_interrupted","sqlite3_interrupt","sqlite3_is_interrupted","sqlite3_str_finish","sqlite3_close_v2","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_auto_extension","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_realloc64","sqlite3_free","sqlite3_str_new","sqlite3_str_append","sqlite3_str_value","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_last_insert_rowid","sqlite3_is_interrupted","sqlite3_interrupt","sqlite3_is_interrupted","sqlite3_str_finish","sqlite3_close_v2","sqlite3_shutdown"],"visited":0}
/*Here's a fuzz driver that uses the specified SQLite APIs to simulate processing SQLite output data:

*/


extern "C" {
    // Declare the required SQLite APIs that aren't in the provided list
    void *sqlite3_realloc64(void *, sqlite3_uint64);
    int sqlite3_value_encoding(sqlite3_value *);
    int sqlite3_auto_extension(void (*)());
    int sqlite3_is_interrupted(sqlite3 *);
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < sizeof(sqlite3_int64)) {
        return 0;
    }

    // Create a file from the input data
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
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_str *str_obj = nullptr;
    sqlite3_value *value = nullptr;
    int rc;
    
    // 1. Use sqlite3_auto_extension at least once
    // Register a dummy extension function
    auto dummy_extension = []() {};
    rc = sqlite3_auto_extension((void (*)())dummy_extension);
    
    // 2. Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK) {
        // Create a simple table
        const char *create_table_sql = 
            "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
        rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
        
        // 3. Insert data into the table
        const char *insert_sql = "INSERT INTO test_table (data) VALUES (?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_blob(stmt, 1, data, size > 1024 ? 1024 : size, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = nullptr;
            
            // 4. Get last insert rowid
            sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
            
            // 5. Use sqlite3_realloc64
            // Reallocate a buffer to hold the rowid
            void *buffer = malloc(sizeof(sqlite3_int64));
            if (buffer) {
                memcpy(buffer, &last_rowid, sizeof(sqlite3_int64));
                void *new_buffer = sqlite3_realloc64(buffer, sizeof(sqlite3_int64) * 2);
                if (new_buffer) {
                    // Copy the rowid again to the new location
                    memcpy((char*)new_buffer + sizeof(sqlite3_int64), &last_rowid, sizeof(sqlite3_int64));
                    sqlite3_free(new_buffer);
                } else {
                    sqlite3_free(buffer);
                }
            }
            
            // 6. Create a sqlite3_str object and use sqlite3_str_value
            str_obj = sqlite3_str_new(nullptr);
            if (str_obj) {
                // Append some data to the string
                char rowid_str[64];
                snprintf(rowid_str, sizeof(rowid_str), "Last rowid: %lld", last_rowid);
                sqlite3_str_append(str_obj, rowid_str, strlen(rowid_str));
                
                // Get the string value
                char *str_val = sqlite3_str_value(str_obj);
                if (str_val && out_file) {
                    fwrite(str_val, 1, strlen(str_val), out_file);
                }
            }
            
            // 7. Create a value and check its encoding
            const char *select_sql = "SELECT data FROM test_table WHERE id = ?;";
            if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int64(stmt, 1, last_rowid);
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    value = sqlite3_column_value(stmt, 0);
                    if (value) {
                        // Use sqlite3_value_encoding
                        int encoding = sqlite3_value_encoding(value);
                        // Encoding values: 1=SQLITE_UTF8, 2=SQLITE_UTF16LE, 3=SQLITE_UTF16BE, 4=SQLITE_UTF16
                        // For BLOBs, encoding is 0 (SQLITE_UTF8 is returned for text values)
                    }
                }
                sqlite3_finalize(stmt);
                stmt = nullptr;
            }
            
            // 8. Set the last insert rowid
            sqlite3_int64 new_rowid = 9999;
            sqlite3_set_last_insert_rowid(db, new_rowid);
            
            // Verify it was set
            sqlite3_int64 verify_rowid = sqlite3_last_insert_rowid(db);
            
            // 9. Use sqlite3_is_interrupted
            int interrupted = sqlite3_is_interrupted(db);
            // interrupted will be 0 unless an interrupt was triggered
            
            // Try to trigger an interrupt (simulate)
            sqlite3_interrupt(db);
            interrupted = sqlite3_is_interrupted(db);
            // Now interrupted should be 1
            
            // Write some output based on our operations
            if (out_file) {
                fwrite(&last_rowid, 1, sizeof(last_rowid), out_file);
                fwrite(&new_rowid, 1, sizeof(new_rowid), out_file);
                fwrite(&verify_rowid, 1, sizeof(verify_rowid), out_file);
                fwrite(&interrupted, 1, sizeof(interrupted), out_file);
            }
        }
    }
    
    // Clean up
    if (str_obj) {
        sqlite3_str_finish(str_obj);
    }
    
    if (db) {
        sqlite3_close_v2(db);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}