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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 2
//<Quality> {"density":28,"unique_branches":{"sqlite3_finalize":[[89557,8,89557,26,3,0,4,0]],"vdbeColumnFromOverflow":[[92941,9,92941,27,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_profile","sqlite3_memory_used","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_bind_text","sqlite3_bind_null","sqlite3_bind_text","sqlite3_bind_null","sqlite3_step","sqlite3_finalize","sqlite3_changes64","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_bytes","sqlite3_column_text","sqlite3_column_text","sqlite3_strnicmp","sqlite3_finalize","sqlite3_memory_used","sqlite3_strnicmp","sqlite3_strnicmp","sqlite3_strnicmp","sqlite3_profile","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_profile","sqlite3_memory_used","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_changes64","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_bytes","sqlite3_column_text","sqlite3_column_text","sqlite3_strnicmp","sqlite3_finalize","sqlite3_memory_used","sqlite3_strnicmp","sqlite3_strnicmp","sqlite3_strnicmp","sqlite3_profile","sqlite3_close","sqlite3_shutdown"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to process SQLite database files:

*/


extern "C" {
    // Declare the APIs we need (some are already in the provided list)
    sqlite3_int64 sqlite3_changes64(sqlite3 *db);
    int sqlite3_strnicmp(const char *str1, const char *str2, int n);
    int sqlite3_value_bytes(sqlite3_value *pVal);
    sqlite3_int64 sqlite3_memory_used(void);
    void *sqlite3_profile(sqlite3 *db, void (*xProfile)(void*, const char*, sqlite3_uint64), void *p);
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	 // Need minimum data
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    FILE *in_file = nullptr;
    void *profile_data = nullptr;
    
    // Profile callback function
    auto profile_callback = [](void *data, const char *sql, sqlite3_uint64 ns) {
        // Do nothing, just needed for sqlite3_profile
    };
    
    // Create a temporary input file from the fuzzing data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use input file descriptor for SQLite operations if needed
    int fd = fuzz_fileno(in_file);
    (void)fd; // Suppress unused variable warning
    
    // Create an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set up profiling
    profile_data = sqlite3_profile(db, profile_callback, nullptr);
    
    // Get initial memory usage
    sqlite3_int64 initial_memory = sqlite3_memory_used();
    
    // Create a table and insert data using the input
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "text1 TEXT, "
        "text2 TEXT);";
    
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Insert data from input buffer
    const char *insert_sql = "INSERT INTO fuzz_table (data, text1, text2) VALUES (?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind data (using first third of input for blob)
        size_t blob_size = size / 3;
        if (blob_size > 0) {
            sqlite3_bind_blob(stmt, 1, data, blob_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        // Bind text1 (using second third of input)
        size_t text1_size = size / 3;
        if (text1_size > 0) {
            const char *text1_start = (const char *)(data + blob_size);
            sqlite3_bind_text(stmt, 2, text1_start, text1_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        
        // Bind text2 (using last third of input)
        size_t text2_size = size - blob_size - text1_size;
        if (text2_size > 0) {
            const char *text2_start = (const char *)(data + blob_size + text1_size);
            sqlite3_bind_text(stmt, 3, text2_start, text2_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Get changes count
    sqlite3_int64 changes = sqlite3_changes64(db);
    
    // Query the data back and use sqlite3_value_bytes
    const char *select_sql = "SELECT data, text1, text2 FROM fuzz_table WHERE id = 1";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get first column as value
            sqlite3_value *value = sqlite3_column_value(stmt, 0);
            if (value) {
                // Use sqlite3_value_bytes
                int value_bytes = sqlite3_value_bytes(value);
                (void)value_bytes; // Suppress unused variable warning
            }
            
            // Compare text1 and text2 using sqlite3_strnicmp
            const char *text1 = (const char *)sqlite3_column_text(stmt, 1);
            const char *text2 = (const char *)sqlite3_column_text(stmt, 2);
            
            if (text1 && text2) {
                // Compare up to 100 characters or actual length
                int compare_len = 100;
                if (strlen(text1) < 100) compare_len = strlen(text1);
                if (strlen(text2) < compare_len) compare_len = strlen(text2);
                
                if (compare_len > 0) {
                    // Use sqlite3_strnicmp
                    int cmp_result = sqlite3_strnicmp(text1, text2, compare_len);
                    (void)cmp_result; // Suppress unused variable warning
                }
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Get final memory usage
    sqlite3_int64 final_memory = sqlite3_memory_used();
    sqlite3_int64 memory_diff = final_memory - initial_memory;
    (void)memory_diff; // Suppress unused variable warning
    
    // Try to execute some random SQL from the input (if it looks like SQL)
    if (size > 50) {
        // Try to extract what looks like SQL from the input
        // Simple heuristic: look for common SQL keywords
        const char *possible_sql = (const char *)data;
        const char *select_keyword = "SELECT";
        const char *insert_keyword = "INSERT";
        const char *update_keyword = "UPDATE";
        
        // Use sqlite3_strnicmp to check for SQL keywords
        int has_select = 0;
        int has_insert = 0;
        int has_update = 0;
        
        // Check first 100 bytes for SQL keywords
        size_t check_len = size > 100 ? 100 : size;
        for (size_t i = 0; i <= check_len - 6; i++) {
            if (!has_select && sqlite3_strnicmp(possible_sql + i, select_keyword, 6) == 0) {
                has_select = 1;
            }
            if (!has_insert && sqlite3_strnicmp(possible_sql + i, insert_keyword, 6) == 0) {
                has_insert = 1;
            }
            if (!has_update && sqlite3_strnicmp(possible_sql + i, update_keyword, 6) == 0) {
                has_update = 1;
            }
        }
    }
    
    // Cleanup
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (db) {
        // Remove profile callback
        if (profile_data) {
            sqlite3_profile(db, nullptr, nullptr);
        }
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}