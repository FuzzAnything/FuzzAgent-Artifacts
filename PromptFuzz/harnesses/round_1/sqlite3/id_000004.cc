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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 3
//<Quality> {"density":25,"unique_branches":{"isFatalError":[[81394,11,81394,24,0,0,4,0],[81394,28,81394,43,0,0,4,1]],"sqlite3AddPrimaryKey":[[121974,7,121974,14,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_busy_handler","sqlite3_threadsafe","sqlite3_open","sqlite3_close","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_parameter_count","sqlite3_bind_text","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_count","sqlite3_column_decltype","sqlite3_column_int64","sqlite3_column_text","sqlite3_column_int64","sqlite3_finalize","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_busy_handler","sqlite3_threadsafe","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_parameter_count","sqlite3_bind_text","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_count","sqlite3_column_decltype","sqlite3_column_int64","sqlite3_finalize","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 1: Open the input database from the provided byte stream
    // First write the data to a file since sqlite3_open expects a filename
    FILE *input_stream = fmemopen((void *)data, size, "rb");
    if (!input_stream) {
	assert_file_closed(&input_stream);
	return 0;
	}
	
    
    // Create a temporary input file from the byte stream
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&input_stream);;
        assert_file_closed(&in_file);
	assert_file_closed(&input_stream);
	return 0;
    }
    
    // Copy data from memory stream to file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_stream)) > 0) {
        fwrite(buffer, 1, bytes_read, in_file);
    }
    assert_file_closed(&in_file);;
    assert_file_closed(&input_stream);;
    
    // Open the database from the file
    sqlite3 *db_source = nullptr;
    int rc = sqlite3_open("input_file", &db_source);
    if (rc != SQLITE_OK || !db_source) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&input_stream);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 2: Configure database settings
    sqlite3_soft_heap_limit64(1024 * 1024);  // 1MB soft heap limit
    
    // Set busy handler (simple callback that always returns 0 to not retry)
    sqlite3_busy_handler(db_source, [](void*, int) -> int { return 0; }, nullptr);
    
    // Check thread safety (though we won't use threads)
    int threadsafe = sqlite3_threadsafe();
    (void)threadsafe;  // Mark as used to avoid warnings
    
    // Step 3: Create a destination database for backup
    sqlite3 *db_dest = nullptr;
    rc = sqlite3_open("output_file", &db_dest);
    if (rc != SQLITE_OK || !db_dest) {
        sqlite3_close(db_source);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&input_stream);
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 4: Perform database backup
    sqlite3_backup *backup = sqlite3_backup_init(db_dest, "main", db_source, "main");
    if (backup) {
        // Backup in steps
        while ((rc = sqlite3_backup_step(backup, 5)) == SQLITE_OK || rc == SQLITE_BUSY) {
            // Continue stepping
        }
        sqlite3_backup_finish(backup);
    }
    
    // Step 5: Execute SQL operations on destination database
    
    // First, create a simple table if it doesn't exist
    char *errmsg = nullptr;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER);";
    
    rc = sqlite3_exec(db_dest, create_table_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Step 6: Use prepared statements with parameter binding
    const char *insert_sql = "INSERT INTO test_table (name, value) VALUES (?, ?);";
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    // Prepare the statement
    rc = sqlite3_prepare_v3(db_dest, insert_sql, -1, 0, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        (void)param_count;  // Mark as used
        
        // Bind parameters using the input data
        // Use first part of data for text, second part for integer
        size_t half_size = size / 2;
        if (half_size > 0) {
            // Create a text parameter from first half of data
            std::string text_param(reinterpret_cast<const char*>(data), 
                                  std::min(half_size, (size_t)100));  // Limit to 100 chars
            
            // Create integer from second half of data
            sqlite3_int64 int_value = 0;
            if (size - half_size >= sizeof(sqlite3_int64)) {
                memcpy(&int_value, data + half_size, sizeof(sqlite3_int64));
            } else if (size > half_size) {
                // Use whatever bytes we have
                memcpy(&int_value, data + half_size, size - half_size);
            }
            
            // Bind the parameters
            sqlite3_bind_text(stmt, 1, text_param.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int64(stmt, 2, int_value);
            
            // Execute the statement
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                // Handle error - but continue with cleanup
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Step 7: Query the data back
    const char *select_sql = "SELECT id, name, value FROM test_table;";
    rc = sqlite3_prepare_v3(db_dest, select_sql, -1, 0, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column count
            int col_count = sqlite3_column_count(stmt);
            (void)col_count;  // Mark as used
            
            // Access each column
            for (int i = 0; i < 3; i++) {
                // Get column declaration type
                const char *decl_type = sqlite3_column_decltype(stmt, i);
                (void)decl_type;  // Mark as used
                
                if (i == 0) {  // id column (integer)
                    sqlite3_int64 id = sqlite3_column_int64(stmt, i);
                    (void)id;  // Mark as used
                }
                else if (i == 1) {  // name column (text)
                    const unsigned char *text = sqlite3_column_text(stmt, i);
                    (void)text;  // Mark as used
                }
                else if (i == 2) {  // value column (integer)
                    sqlite3_int64 value = sqlite3_column_int64(stmt, i);
                    (void)value;  // Mark as used
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Step 8: Cleanup
    sqlite3_close(db_dest);
    sqlite3_close(db_source);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&input_stream);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}