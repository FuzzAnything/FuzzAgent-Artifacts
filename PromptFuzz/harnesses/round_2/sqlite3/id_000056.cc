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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 8
//<Quality> {"density":15,"unique_branches":{"sqlite3_realloc64":[[30388,7,30388,27,0,0,4,1]],"sqlite3_auto_extension":[[136477,7,136477,15,0,0,4,1],[136481,7,136481,9,0,0,4,1],[136492,14,136492,31,0,0,4,1],[136495,9,136495,27,0,0,4,0],[136499,11,136499,18,0,0,4,1]],"sqlite3_set_last_insert_rowid":[[178699,7,178699,32,0,0,4,1]],"sqlite3_is_interrupted":[[179446,7,179446,32,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_set_last_insert_rowid","sqlite3_last_insert_rowid","sqlite3_is_interrupted","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_finalize","sqlite3_malloc64","sqlite3_realloc64","sqlite3_free","sqlite3_free","sqlite3_auto_extension","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_set_last_insert_rowid","sqlite3_last_insert_rowid","sqlite3_is_interrupted","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_finalize","sqlite3_malloc64","sqlite3_realloc64","sqlite3_free","sqlite3_auto_extension","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Dummy extension function for auto_extension
static void  id_000056_dummy_extension() {
    // Empty extension function
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize sqlite3
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create database filename
    const char *db_filename = "fuzz_db.sqlite";
    
    // Open database
    sqlite3 *db = nullptr;
    if (sqlite3_open(db_filename, &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set last insert rowid to a value from input data
    if (size >= sizeof(sqlite3_int64)) {
        sqlite3_int64 rowid;
        memcpy(&rowid, data, sizeof(rowid));
        sqlite3_set_last_insert_rowid(db, rowid);
    }

    // Get last insert rowid
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);

    // Check if database is interrupted
    int interrupted = sqlite3_is_interrupted(db);

    // Create a temporary sqlite3_str object using reallocated memory
    // Note: sqlite3_str_new is not in the provided list, so we'll work around it
    // by creating a value and checking its encoding
    sqlite3_value *test_value = nullptr;
    
    // Prepare a simple statement to create a value
    sqlite3_stmt *stmt = nullptr;
    const char *simple_sql = "SELECT 1";
    if (sqlite3_prepare_v2(db, simple_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            test_value = sqlite3_column_value(stmt, 0);
            
            // Check value encoding if we got a value
            if (test_value) {
                int encoding = sqlite3_value_encoding(test_value);
                (void)encoding; // Use encoding to avoid unused variable warning
            }
        }
        sqlite3_finalize(stmt);
    }

    // Use realloc64 with data from input
    if (size > 0) {
        void *allocated = sqlite3_malloc64(size);
        if (allocated) {
            memcpy(allocated, data, size);
            
            // Reallocate to double the size
            void *reallocated = sqlite3_realloc64(allocated, size * 2);
            if (reallocated) {
                // Copy remaining data if any
                if (size * 2 > size) {
                    size_t remaining = (size * 2) - size;
                    if (remaining > 0 && size > 0) {
                        uint8_t *dest = (uint8_t *)reallocated + size;
                        size_t copy_size = remaining < size ? remaining : size;
                        memcpy(dest, data, copy_size);
                    }
                }
                sqlite3_free(reallocated);
            } else {
                sqlite3_free(allocated);
            }
        }
    }

    // Register auto-extension
    sqlite3_auto_extension((void (*)()) id_000056_dummy_extension);

    // Note: sqlite3_str_value requires a sqlite3_str* object, but we don't have
    // a direct way to create one with the provided APIs. We'll attempt to use
    // it if we can get a sqlite3_str object through other means.
    
    // Create a table and insert some data using the input
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Insert data from input
    if (size > 0) {
        sqlite3_stmt *insert_stmt = nullptr;
        const char *insert_sql = "INSERT INTO fuzz_table (data) VALUES (?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_blob(insert_stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
            sqlite3_step(insert_stmt);
            sqlite3_finalize(insert_stmt);
        }
    }

    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    
    // Remove the temporary database file
    remove(db_filename);
    
    // Finalize sqlite3
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	return 0;
}