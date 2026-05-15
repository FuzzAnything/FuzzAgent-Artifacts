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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"sqlite3_enable_load_extension":[[136428,7,136428,12,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_commit_hook","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_appendchar","sqlite3_str_finish","sqlite3_create_filename","sqlite3_open_v2","sqlite3_free_filename","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_text16","sqlite3_finalize","sqlite3_memory_alarm","sqlite3_db_cacheflush","sqlite3_db_cacheflush","sqlite3_free","sqlite3_finalize","sqlite3_close_v2","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_commit_hook","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_appendchar","sqlite3_str_finish","sqlite3_create_filename","sqlite3_open_v2","sqlite3_free_filename","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_text16","sqlite3_finalize","sqlite3_memory_alarm","sqlite3_db_cacheflush","sqlite3_db_cacheflush","sqlite3_free","sqlite3_finalize","sqlite3_close_v2","sqlite3_close_v2"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Return early if there's no data
    if (!data || size == 0) {
        return 0;
    }

    sqlite3 *db1 = nullptr;
    sqlite3 *db2 = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_str *str = nullptr;
    sqlite3_filename temp_filename = nullptr;
    char *finished_str = nullptr;
    
    // Open input database from memory stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read database into temporary file
    FILE *temp_db = fopen("input_file", "wb");
    if (!temp_db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_db);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data from memory to temporary file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&temp_db);;
    
    // Open first database with SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
    int rc = sqlite3_open_v2("input_file", &db1, 
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                             nullptr);
    if (rc != SQLITE_OK || !db1) {
        assert_file_closed(&temp_db);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Enable load extension
    sqlite3_enable_load_extension(db1, 1);
    
    // Set commit hook
    sqlite3_commit_hook(db1, [](void *userdata) -> int {
        return 0;  // Return 0 to allow commit
    }, nullptr);
    
    // Create and manipulate string builder
    str = sqlite3_str_new(db1);
    if (str) {
        // Append some characters
        sqlite3_str_appendchar(str, 5, 'A');
        sqlite3_str_appendchar(str, 3, 'B');
        
        // Finish string
        finished_str = sqlite3_str_finish(str);
    }
    
    // Create temporary filename
    const char *database = "test.db";
    const char *journal = "test.db-journal";
    const char *wal = "test.db-wal";
    temp_filename = sqlite3_create_filename(database, journal, wal, 0, nullptr);
    
    // Open second database using the created filename
    if (temp_filename) {
        rc = sqlite3_open_v2(temp_filename, &db2,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                             nullptr);
        
        // Free the filename after use
        sqlite3_free_filename(temp_filename);
    }
    
    // Create a simple table and query it if second database opened successfully
    if (db2) {
        // Create a table
        const char *create_table_sql = 
            "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB, text TEXT)";
        rc = sqlite3_exec(db2, create_table_sql, nullptr, nullptr, nullptr);
        
        if (rc == SQLITE_OK) {
            // Insert some test data
            const char *insert_sql = 
                "INSERT INTO test (data, text) VALUES (?, ?)";
            rc = sqlite3_prepare_v2(db2, insert_sql, -1, &stmt, nullptr);
            
            if (rc == SQLITE_OK && stmt) {
                // Bind blob data (use first 100 bytes or available size)
                size_t blob_size = size > 100 ? 100 : size;
                sqlite3_bind_blob(stmt, 1, data, blob_size, SQLITE_STATIC);
                
                // Bind text (use finished_str if available, otherwise fixed text)
                const char *text_data = finished_str ? finished_str : "test";
                sqlite3_bind_text(stmt, 2, text_data, -1, SQLITE_STATIC);
                
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                stmt = nullptr;
            }
            
            // Query the data back to use column APIs
            const char *select_sql = "SELECT data, text FROM test";
            rc = sqlite3_prepare_v2(db2, select_sql, -1, &stmt, nullptr);
            
            if (rc == SQLITE_OK && stmt) {
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    // Use the required column APIs
                    const void *blob_data = sqlite3_column_blob(stmt, 0);
                    const void *text_data = sqlite3_column_text16(stmt, 1);
                    
                    // Access the data to avoid optimization removal
                    if (blob_data && text_data) {
                        // Do nothing, just accessing is enough
                    }
                }
                sqlite3_finalize(stmt);
                stmt = nullptr;
            }
        }
    }
    
    // Set up memory alarm callback
    sqlite3_memory_alarm([](void *userdata, sqlite3_int64 used, int N) {
        // Memory alarm callback - no specific action needed for fuzzing
    }, nullptr, 1000000);  // 1MB threshold
    
    // Flush database cache
    if (db1) {
        sqlite3_db_cacheflush(db1);
    }
    if (db2) {
        sqlite3_db_cacheflush(db2);
    }
    
    // Cleanup
    if (finished_str) {
        sqlite3_free(finished_str);
    }
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db2) {
        sqlite3_close_v2(db2);
    }
    
    if (db1) {
        sqlite3_close_v2(db1);
    }
    
    // Clean up temporary files
    remove("input_file");
    if (temp_filename) {
        remove(database);
        remove(journal);
        remove(wal);
    }
    
    assert_file_closed(&temp_db);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}