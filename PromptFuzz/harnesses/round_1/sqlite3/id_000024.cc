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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 14
//<Quality> {"density":17,"unique_branches":{"sqlite3LeaveMutexAndCloseZombie":[[179004,11,179004,24,0,0,4,0]],"sqlite3_serialize":[[52993,7,52993,9,0,0,4,0],[52995,7,52995,21,0,0,4,0],[53000,9,53000,41,0,0,4,1],[53004,11,53004,15,0,0,4,0],[53004,11,53004,15,0,0,4,1],[53008,21,53008,32,0,0,4,0],[53008,21,53008,32,0,0,4,1],[53012,15,53012,28,0,0,4,0]],"lockBtree":[[73142,9,73142,23,0,0,4,0]],"sqlite3_transfer_bindings":[[91407,7,91407,29,0,0,4,1],[91411,7,91411,19,0,0,4,1],[91415,7,91415,21,0,0,4,1]],"sqlite3_clear_bindings":[[89629,12,89629,21,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_extended_result_codes","sqlite3_malloc64","sqlite3_create_collation_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_transfer_bindings","sqlite3_clear_bindings","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_finalize","sqlite3_db_filename","sqlite3_serialize","sqlite3_complete16","sqlite3_free","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_extended_result_codes","sqlite3_malloc64","sqlite3_create_collation_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_transfer_bindings","sqlite3_clear_bindings","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_finalize","sqlite3_db_filename","sqlite3_serialize","sqlite3_complete16","sqlite3_free","sqlite3_free","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    // Write input data to a file for database operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    const char *input_filename = "input_file";
    FILE *input_copy = fopen(input_filename, "wb");
    if (input_copy) {
        uint8_t buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, input_copy);
        }
        assert_file_closed(&input_copy);;
    }
    assert_file_closed(&in_file);;
    
    // Open the database from the input file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(input_filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        remove(input_filename);
        assert_file_closed(&input_copy);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Enable extended result codes
    sqlite3_extended_result_codes(db, 1);
    
    // Allocate memory using sqlite3_malloc64
    sqlite3_uint64 alloc_size = (size % 1024) + 1;
    void *allocated_mem = sqlite3_malloc64(alloc_size);
    if (allocated_mem) {
        memset(allocated_mem, 0, (size_t)alloc_size);
    }
    
    // Create a simple collation
    sqlite3_create_collation_v2(db, "test_collation", SQLITE_UTF8, nullptr,
        [](void *, int len1, const void *str1, int len2, const void *str2) {
            int min_len = len1 < len2 ? len1 : len2;
            return memcmp(str1, str2, min_len);
        },
        [](void *) {}
    );
    
    // Create a table and prepare statements
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    
    // Prepare statements
    sqlite3_stmt *stmt1 = nullptr;
    sqlite3_stmt *stmt2 = nullptr;
    const char *insert_sql = "INSERT INTO test(data) VALUES(?);";
    const char *select_sql = "SELECT * FROM test WHERE data = ?;";
    
    // Prepare first statement
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, nullptr) == SQLITE_OK) {
        // Prepare second statement
        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr) == SQLITE_OK) {
            // Bind data to first statement
            const char *test_data = "test_value";
            sqlite3_bind_text(stmt1, 1, test_data, -1, SQLITE_STATIC);
            
            // Transfer bindings from stmt1 to stmt2
            sqlite3_transfer_bindings(stmt1, stmt2);
            
            // Clear bindings from stmt1
            sqlite3_clear_bindings(stmt1);
            
            // Execute stmt2 with transferred bindings
            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                // Process row if needed
            }
            
            sqlite3_reset(stmt2);
            sqlite3_finalize(stmt2);
        }
        sqlite3_finalize(stmt1);
    }
    
    // Get database filename
    const char *db_filename = sqlite3_db_filename(db, "main");
    
    // Serialize the database
    sqlite3_int64 serialized_size = 0;
    unsigned int flags = 0;
    unsigned char *serialized = sqlite3_serialize(db, "main", &serialized_size, flags);
    
    // Check if an SQL string is complete (using a sample SQL string)
    const void *sample_sql = u"SELECT * FROM test;";
    int is_complete = sqlite3_complete16(sample_sql);
    
    // Write serialized data to output file if available
    if (serialized && serialized_size > 0) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(serialized, 1, (size_t)serialized_size, out_file);
            assert_file_closed(&out_file);;
        }
    }
    
    // Clean up
    if (serialized) {
        sqlite3_free(serialized);
    }
    if (allocated_mem) {
        sqlite3_free(allocated_mem);
    }
    
    // Close database
    sqlite3_close(db);
    
    // Remove temporary files
    remove(input_filename);
    remove("output_file");
    
    assert_file_closed(&input_copy);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}