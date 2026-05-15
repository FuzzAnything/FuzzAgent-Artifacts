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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"sqlite3_bind_parameter_name":[[91357,7,91357,11,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_name","sqlite3_bind_parameter_name","sqlite3_set_last_insert_rowid","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_mprintf","sqlite3_backup_remaining","sqlite3_free","sqlite3_backup_step","sqlite3_backup_remaining","sqlite3_sleep","sqlite3_backup_finish","sqlite3_close","sqlite3_mutex_leave","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_name","sqlite3_bind_parameter_name","sqlite3_set_last_insert_rowid","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_mprintf","sqlite3_backup_remaining","sqlite3_free","sqlite3_backup_step","sqlite3_backup_remaining","sqlite3_sleep","sqlite3_backup_finish","sqlite3_close","sqlite3_mutex_leave","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Trace callback function
static int  id_000022_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    return 0; // Return 0 to continue
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Create input file from the byte stream
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for backup
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    sqlite3* db = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_backup* backup = nullptr;
    
    // 1. Open first database (in-memory)
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Set up trace
    sqlite3_trace_v2(db, fuzz_uint32_t_1,  id_000022_trace_callback, nullptr);
    
    // 3. Get database mutex and lock it
    sqlite3_mutex* mutex = sqlite3_db_mutex(db);
    if (mutex) {
        sqlite3_mutex_enter(mutex);
    }
    
    // 4. Prepare a statement using some of the input data as SQL
    // We'll use a simple SQL statement and bind parameters from the data
    const char* sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 5. Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if any
        }
        
        // 6. Get data count (should be 0 for CREATE TABLE)
        int data_count = sqlite3_data_count(stmt);
        
        // 7. Get column names (if any)
        if (data_count > 0) {
            for (int i = 0; i < data_count; i++) {
                const char* col_name = sqlite3_column_name(stmt, fuzz_int32_t_2);
            }
        }
        
        // 8. Get bind parameter names (if any)
        const char* param_name = sqlite3_bind_parameter_name(stmt, fuzz_int32_t_3);
        
        // 9. Set last insert rowid
        sqlite3_set_last_insert_rowid(db, fuzz_int64_t_4);
        
        // 10. Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 11. Open second database for backup
    rc = sqlite3_open("output_file", &db2);
    if (rc == SQLITE_OK && db2) {
        // 12. Initialize backup from in-memory to file database
        backup = sqlite3_backup_init(db2, fuzz_str_5, db, fuzz_str_6);
        if (backup) {
            // 13. Format a message using mprintf
            char* msg = sqlite3_mprintf("Starting backup, remaining pages: %d", 
                                       sqlite3_backup_remaining(backup));
            if (msg) {
                sqlite3_free(msg);
            }
            
            // 14. Perform backup step by step
            while ((rc = sqlite3_backup_step(backup, 5)) == SQLITE_OK || 
                   rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                // Check remaining pages
                int remaining = sqlite3_backup_remaining(backup);
                if (remaining == 0) {
                    break;
                }
                
                // Small delay to avoid busy loops
                sqlite3_sleep(fuzz_int32_t_7);
            }
            
            // 15. Finish backup
            sqlite3_backup_finish(backup);
            backup = nullptr;
        }
        
        // 16. Close second database
        sqlite3_close(db2);
        db2 = nullptr;
    }
    
    // 17. Unlock mutex
    if (mutex) {
        sqlite3_mutex_leave(mutex);
    }
    
    // 18. Close first database
    sqlite3_close(db);
    db = nullptr;
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}