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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 15
//<Quality> {"density":20,"unique_branches":{"sqlite3_drop_modules":[[153788,7,153788,32,0,0,4,1],[153790,44,153790,49,0,0,4,0],[153790,44,153790,49,0,0,4,1],[153793,9,153793,16,0,0,4,0],[153795,17,153795,31,0,0,4,1],[153796,11,153796,25,0,0,4,1]],"sqlite3ExprListCompare":[[113964,16,113964,21,0,0,4,0]],"lookupName":[[18359,25,18359,65,12,0,4,1]],"sqlite3_stmt_scanstatus_reset":[[91996,13,91996,17,0,0,4,0],[91996,13,91996,17,0,0,4,1],[91996,21,91996,30,0,0,4,0],[91996,21,91996,30,0,0,4,1]],"sqlite3VtabCreateModule":[[153694,7,153694,17,0,0,4,0],[153714,7,153714,11,0,0,4,0],[153715,9,153715,19,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_column_name","sqlite3_set_last_insert_rowid","sqlite3_status64","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_mutex_leave","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_backup_step","sqlite3_backup_remaining","sqlite3_drop_modules","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_column_name","sqlite3_set_last_insert_rowid","sqlite3_status64","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_mutex_leave","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_backup_step","sqlite3_backup_remaining","sqlite3_drop_modules","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"visited":0}
/**/


// Trace callback function for sqlite3_trace_v2
static int  id_000067_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    // Simple trace callback that does nothing
    (void)mask;
    (void)context;
    (void)p;
    (void)x;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // Step 1: Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db1 = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_mutex* mutex = nullptr;
    
    // Step 2: Try to open first database
    if (sqlite3_open(":memory:", &db1) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Set up tracing
    sqlite3_trace_v2(db1, fuzz_uint32_t_1,  id_000067_trace_callback, nullptr);
    
    // Step 4: Get database mutex and lock it
    mutex = sqlite3_db_mutex(db1);
    sqlite3_mutex_enter(mutex);
    
    // Step 5: Read SQL from input file and prepare statement
    // We'll use up to 1KB of input data for SQL
    char sql_buffer[1024];
    size_t sql_size = fread(sql_buffer, 1, sizeof(sql_buffer) - 1, in_file);
    sql_buffer[sql_size] = '\0';
    
    const char* tail = nullptr;
    if (sqlite3_prepare_v2(db1, sql_buffer, -1, &stmt, &tail) == SQLITE_OK) {
        // Step 6: Get data count from prepared statement
        int data_count = sqlite3_data_count(stmt);
        
        // Step 7: Get column names if available
        for (int i = 0; i < data_count; i++) {
            const char* col_name = sqlite3_column_name(stmt, fuzz_int32_t_2);
            (void)col_name; // Use variable to avoid unused warning
        }
        
        // Step 8: Set last insert rowid
        sqlite3_set_last_insert_rowid(db1, fuzz_int64_t_3);
        
        // Step 9: Get memory status
        sqlite3_int64 current = 0;
        sqlite3_int64 highwater = 0;
        sqlite3_status64(fuzz_int32_t_4, &current, &highwater, fuzz_int32_t_5);
        
        // Step 10: Reset statement scan status
        sqlite3_stmt_scanstatus_reset(stmt);
        
        // Step 11: Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Step 12: Unlock the mutex
    sqlite3_mutex_leave(mutex);
    
    // Step 13: Open second database for backup
    if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
        // Step 14: Initialize backup
        backup = sqlite3_backup_init(db2, fuzz_str_6, db1, fuzz_str_7);
        if (backup) {
            // Step 15: Check backup remaining pages
            int remaining = sqlite3_backup_remaining(backup);
            
            // Step 16: Perform backup step
            while (remaining > 0) {
                int result = sqlite3_backup_step(backup, fuzz_int32_t_8); // 5 pages at a time
                if (result != SQLITE_OK && result != SQLITE_BUSY && result != SQLITE_LOCKED) {
                    break;
                }
                remaining = sqlite3_backup_remaining(backup);
            }
            
            // Step 17: Drop modules from first database
            const char* keep_modules[] = {nullptr};
            sqlite3_drop_modules(db1, keep_modules);
            
            // Step 18: Finish backup
            sqlite3_backup_finish(backup);
            backup = nullptr;
        }
        
        // Step 19: Close second database
        sqlite3_close(db2);
        db2 = nullptr;
    }
    
    // Step 20: Close first database
    sqlite3_close(db1);
    db1 = nullptr;
    
    // Step 21: Clean up file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}