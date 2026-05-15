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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_column_name","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_mutex_leave","sqlite3_open","sqlite3_backup_init","sqlite3_db_handle","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_changes","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_column_name","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_mutex_leave","sqlite3_open","sqlite3_backup_init","sqlite3_db_handle","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_changes","sqlite3_close","sqlite3_close"],"visited":0}
/**/


// Trace callback function
static int  id_000065_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    // Minimal implementation since we just need to register a callback
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db1 = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_backup* backup = nullptr;
    const char* tail = nullptr;
    int rc = 0;
    
    // Step 1: Create input file with the data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Open first database
    rc = sqlite3_open(":memory:", &db1);
    if (rc != SQLITE_OK || !db1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Set up trace callback
    sqlite3_trace_v2(db1, fuzz_uint32_t_1,  id_000065_trace_callback, nullptr);
    
    // Step 4: Get database mutex and lock it
    sqlite3_mutex* mutex = sqlite3_db_mutex(db1);
    sqlite3_mutex_enter(mutex);
    
    // Step 5: Prepare SQL statement from input data
    // Read SQL from input (use first half for SQL, second half for backup pages)
    size_t sql_size = size / 2;
    if (sql_size > 0) {
        char* sql = (char*)malloc(sql_size + 1);
        if (sql) {
            fseek(in_file, 0, SEEK_SET);
            fread(sql, 1, sql_size, in_file);
            sql[sql_size] = '\0';
            
            rc = sqlite3_prepare_v2(db1, sql, -1, &stmt, &tail);
            free(sql);
            
            if (rc == SQLITE_OK && stmt) {
                // Step 6: Get data count from prepared statement
                int data_count = sqlite3_data_count(stmt);
                
                // Step 7: Get column names if available
                if (data_count > 0) {
                    for (int i = 0; i < data_count; i++) {
                        const char* col_name = sqlite3_column_name(stmt, fuzz_int32_t_2);
                        (void)col_name; // Use variable to avoid compiler warning
                    }
                }
                
                // Step 8: Use scanstatus if available
                if (size > 100) { // Only if we have enough data
                    int idx = data[0] % 10; // Use first byte for index
                    int scan_op = data[1] % 5; // Use second byte for operation
                    int flags = 0;
                    void* pOut = nullptr;
                    sqlite3_stmt_scanstatus_v2(stmt, idx, scan_op, fuzz_int32_t_3, pOut);
                }
                
                // Step 9: Finalize the statement
                sqlite3_finalize(stmt);
                stmt = nullptr;
            }
        }
    }
    
    // Step 10: Unlock mutex
    sqlite3_mutex_leave(mutex);
    
    // Step 11: Open second database for backup
    rc = sqlite3_open(":memory:", &db2);
    if (rc == SQLITE_OK && db2) {
        // Step 12: Create backup from db1 to db2
        backup = sqlite3_backup_init(db2, fuzz_str_4, db1, fuzz_str_5);
        if (backup) {
            // Step 13: Get database handle from backup (simulated)
            // We'll get handle from a hypothetical statement instead
            if (stmt) { // If stmt still exists
                sqlite3* db_from_stmt = sqlite3_db_handle(stmt);
                (void)db_from_stmt; // Use variable
            }
            
            // Step 14: Perform backup step
            // Use second half of data to determine page count
            int nPage = 1; // Default to 1 page
            if (size > sql_size + 1) {
                nPage = data[sql_size] % 10 + 1; // 1-10 pages
            }
            sqlite3_backup_step(backup, fuzz_int32_t_6);
            
            // Step 15: Finish backup
            sqlite3_backup_finish(backup);
        }
        
        // Step 16: Get changes count
        int changes = sqlite3_changes(db1);
        (void)changes; // Use variable
        
        // Step 17: Close second database
        sqlite3_close(db2);
    }
    
    // Step 18: Close first database
    sqlite3_close(db1);
    
    // Step 19: Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}