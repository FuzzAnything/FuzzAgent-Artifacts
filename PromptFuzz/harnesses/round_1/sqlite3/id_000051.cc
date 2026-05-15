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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 85, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"hasHotJournal":[[61585,15,61585,60,0,0,4,1]],"sqlite3_backup_remaining":[[81803,7,81803,11,0,0,4,1]],"sqlite3_db_mutex":[[178467,7,178467,32,0,0,4,1]],"sqlite3_filename_wal":[[182406,7,182406,16,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_open_v2","sqlite3_close","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_zeroblob64","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_remaining","sqlite3_backup_finish","sqlite3_create_function","sqlite3_result_null","sqlite3_result_text16le","sqlite3_result_null","sqlite3_result_null","sqlite3_result_text16le","sqlite3_result_null","sqlite3_open","sqlite3_exec","sqlite3_filename_wal","sqlite3_db_filename","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_exec","sqlite3_mutex_leave","sqlite3_close","sqlite3_strglob","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_open_v2","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_zeroblob64","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_remaining","sqlite3_backup_finish","sqlite3_create_function","sqlite3_result_null","sqlite3_result_text16le","sqlite3_result_null","sqlite3_result_null","sqlite3_result_text16le","sqlite3_result_null","sqlite3_open","sqlite3_exec","sqlite3_filename_wal","sqlite3_db_filename","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_exec","sqlite3_mutex_leave","sqlite3_close","sqlite3_strglob","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2","sqlite3_close_v2"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to achieve an event involving database backup operations with zero blob binding and result handling:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_stmt* stmt2 = nullptr;
    sqlite3_mutex* mutex = nullptr;
    
    // Create a temporary file from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open first database
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open second database
    rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db2) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create a table in first database
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB, text TEXT);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Prepare a statement for inserting data
    const char* insert_sql = "INSERT INTO test (data, text) VALUES (?, ?);";
    rc = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind zeroblob64 using input size as parameter
        // Use modulo to prevent overflow issues
        sqlite3_uint64 blob_size = (sqlite3_uint64)(size % 1024);
        rc = sqlite3_bind_zeroblob64(stmt, 1, blob_size);
        
        // Bind some text data
        const char* sample_text = "test";
        rc = sqlite3_bind_text(stmt, 2, sample_text, -1, SQLITE_STATIC);
        
        // Execute the statement
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
                // Reset for potential reuse
                sqlite3_reset(stmt);
            }
        }
    }
    
    // Create backup from first to second database
    backup = sqlite3_backup_init(db2, "main", db, "main");
    if (backup) {
        // Perform backup in steps
        int backup_rc;
        do {
            backup_rc = sqlite3_backup_step(backup, 5); // Backup 5 pages at a time
        } while (backup_rc == SQLITE_OK || backup_rc == SQLITE_BUSY || backup_rc == SQLITE_LOCKED);
        
        // Use backup_remaining API
        int remaining = sqlite3_backup_remaining(backup);
        
        // Finish backup
        sqlite3_backup_finish(backup);
    }
    
    // Create a custom function that uses result_text16le and result_null
    // First, create a function that will be called by SQL queries
    struct FuncContext {
        bool return_null;
    };
    
    // Create a scalar function
    rc = sqlite3_create_function(
        db,
        "fuzz_func",
        1,
        SQLITE_UTF8,
        nullptr,
        [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            if (argc < 1) {
                sqlite3_result_null(ctx);
                return;
            }
            
            // Alternate between returning text and null
            static int call_count = 0;
            call_count++;
            
            if (call_count % 2 == 0) {
                // Return text16le
                const char* text = "fuzzed_text";
                sqlite3_result_text16le(ctx, text, -1, SQLITE_TRANSIENT);
            } else {
                // Return null
                sqlite3_result_null(ctx);
            }
        },
        nullptr,
        nullptr
    );
    
    // Create a file-based database to test filename_wal API
    sqlite3* file_db = nullptr;
    rc = sqlite3_open("output_file", &file_db);
    if (rc == SQLITE_OK && file_db) {
        // Create table
        rc = sqlite3_exec(file_db, "CREATE TABLE test2 (id INT);", nullptr, nullptr, nullptr);
        
        // Get WAL filename (if WAL mode is enabled)
        const char* wal_filename = sqlite3_filename_wal(sqlite3_db_filename(file_db, "main"));
        
        // Use db_mutex API
        mutex = sqlite3_db_mutex(file_db);
        if (mutex) {
            sqlite3_mutex_enter(mutex);
            // Perform some operation while mutex is held
            rc = sqlite3_exec(file_db, "INSERT INTO test2 VALUES (1);", nullptr, nullptr, nullptr);
            sqlite3_mutex_leave(mutex);
        }
        
        sqlite3_close(file_db);
    }
    
    // Use strglob API
    const char* glob_pattern = "test*";
    const char* test_string = "test_data";
    int glob_result = sqlite3_strglob(glob_pattern, test_string);
    
    // Prepare a statement in the second database to verify backup
    if (db2) {
        const char* select_sql = "SELECT * FROM test;";
        rc = sqlite3_prepare_v3(db2, select_sql, -1, 0, &stmt2, nullptr);
        if (rc == SQLITE_OK && stmt2) {
            // Execute and step through results
            while ((rc = sqlite3_step(stmt2)) == SQLITE_ROW) {
                // Process row if needed
            }
        }
    }
    
    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close_v2(db);
    if (db2) sqlite3_close_v2(db2);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}