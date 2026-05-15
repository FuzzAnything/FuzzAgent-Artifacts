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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["sqlite3_auto_extension","sqlite3_open","sqlite3_rollback_hook","sqlite3_malloc","sqlite3_free","sqlite3_close_v2","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_finalize","sqlite3_serialize","sqlite3_strglob","sqlite3_free","sqlite3_global_recover","sqlite3_close_v2"],"critical_calls":["sqlite3_auto_extension","sqlite3_open","sqlite3_rollback_hook","sqlite3_malloc","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_finalize","sqlite3_serialize","sqlite3_strglob","sqlite3_free","sqlite3_global_recover","sqlite3_close_v2"],"visited":0}
/**/


// Dummy extension function for sqlite3_auto_extension
static void  id_000008_dummy_extension() {}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
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
	
    
    // 1. Write input data to a file (simulating sqlite3 output data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a database file from input data
    FILE *db_file = fopen("input_file", "wb");
    if (!db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy input data to database file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, db_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&db_file);;
    
    // 2. Register an auto-extension
    sqlite3_auto_extension( id_000008_dummy_extension);
    
    // 3. Open the database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK || !db) {
        remove("input_file");
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Set a rollback hook
    sqlite3_rollback_hook(db, [](void *p) {
        // Dummy callback
    }, nullptr);
    
    // 5. Allocate memory using sqlite3_malloc
    void *malloc_ptr = sqlite3_malloc(1024);
    if (malloc_ptr) {
        // Use some of the input data in the allocated memory
        size_t copy_size = size < 1024 ? size : 1024;
        memcpy(malloc_ptr, data, copy_size);
        
        // 6. Free the allocated memory
        sqlite3_free(malloc_ptr);
    }
    
    // 7. Prepare a statement using input data as SQL
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    // Use the first part of input data as SQL string (null-terminated)
    size_t sql_len = size < 256 ? size : 256;
    char *sql = (char *)malloc(sql_len + 1);
    if (!sql) {
        sqlite3_close_v2(db);
        remove("input_file");
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    memcpy(sql, data, sql_len);
    sql[sql_len] = '\0';
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    free(sql);
    
    if (rc == SQLITE_OK && stmt) {
        // 8. Get bind parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 9. Step through the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 10. Get column values for each column
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_value *value = sqlite3_column_value(stmt, i);
                // value can be used for further processing if needed
            }
        }
        
        // 11. Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // 12. Serialize the database
    sqlite3_int64 serialize_size = 0;
    unsigned char *serialized = sqlite3_serialize(db, "main", &serialize_size, 0);
    if (serialized) {
        // 13. Use strglob with some pattern and data
        if (size > 0) {
            // Create a pattern from input data
            char pattern[256];
            size_t pattern_len = size < 255 ? size : 255;
            memcpy(pattern, data, pattern_len);
            pattern[pattern_len] = '\0';
            
            // Create a string to compare against
            char compare_str[256] = "test_string";
            
            // Use strglob
            sqlite3_strglob(pattern, compare_str);
        }
        
        // Free serialized data
        sqlite3_free(serialized);
    }
    
    // 14. Call global recover
    sqlite3_global_recover();
    
    // 15. Close database (using close_v2)
    sqlite3_close_v2(db);
    
    // Clean up
    remove("input_file");
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}