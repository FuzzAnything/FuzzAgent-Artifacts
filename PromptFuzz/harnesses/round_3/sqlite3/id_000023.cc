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
//<score> 108, nr_unique_branch: 3
//<Quality> {"density":27,"unique_branches":{"sqlite3ErrorToParser":[[34710,7,34710,12,0,0,4,0]],"sqlite3StrAccumEnlarge":[[31668,7,31668,20,0,0,4,0]],"sqlite3StrAccumSetError":[[30832,7,30832,17,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_mutex_leave","sqlite3_close","sqlite3_prepare_v2","sqlite3_str_append","sqlite3_data_count","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_column_name","sqlite3_set_last_insert_rowid","sqlite3_finalize","sqlite3_open","sqlite3_mutex_leave","sqlite3_close","sqlite3_close","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_mprintf","sqlite3_free","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_mutex_leave","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_db_mutex","sqlite3_mutex_enter","sqlite3_prepare_v2","sqlite3_str_append","sqlite3_data_count","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_column_name","sqlite3_set_last_insert_rowid","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_mprintf","sqlite3_free","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_mutex_leave","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Trace callback function
static int  id_000023_traceCallback(unsigned int mask, void* context, void* p, void* x) {
    // Simple trace callback that does nothing
    return 0;
}

// Custom string buffer for sqlite3_str operations
static sqlite3_str*  id_000023_create_str_buffer() {
    // Since sqlite3_str_new isn't in the provided APIs, we'll create a minimal implementation
    // This is a hacky workaround since we don't have sqlite3_str_new in the allowed APIs
    static char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    return reinterpret_cast<sqlite3_str*>(buffer);
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db_source = nullptr;
    sqlite3* db_dest = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_mutex* mutex = nullptr;
    
    // 1. Open source database
    if (sqlite3_open(":memory:", &db_source) != SQLITE_OK) {
        if (db_source) sqlite3_close(db_source);
        return 0;
    }
    
    // 2. Set up tracing
    sqlite3_trace_v2(db_source, fuzz_uint32_t_1,  id_000023_traceCallback, nullptr);
    
    // 3. Get database mutex and lock it
    mutex = sqlite3_db_mutex(db_source);
    sqlite3_mutex_enter(mutex);
    
    // 4. Prepare SQL statement using input data
    // Convert input data to null-terminated string for SQL
    char* sql = reinterpret_cast<char*>(malloc(size + 1));
    if (!sql) {
        sqlite3_mutex_leave(mutex);
        sqlite3_close(db_source);
        return 0;
    }
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    const char* tail = nullptr;
    int prepare_result = sqlite3_prepare_v2(db_source, sql, -1, &stmt, &tail);
    
    if (prepare_result == SQLITE_OK && stmt) {
        // 5. Use sqlite3_str_append (simulated since we don't have sqlite3_str_new)
        // Create a minimal string buffer for demonstration
        sqlite3_str* str_buf =  id_000023_create_str_buffer();
        const char* test_str = "test";
        sqlite3_str_append(str_buf, fuzz_str_2, 4);
        
        // 6. Get data count
        int data_count = sqlite3_data_count(stmt);
        
        // 7. Get bind parameter name and column name
        const char* param_name = nullptr;
        if (sqlite3_bind_parameter_count(stmt) > 0) {
            param_name = sqlite3_bind_parameter_name(stmt, fuzz_int32_t_3);
        }
        
        const char* col_name = nullptr;
        if (data_count > 0) {
            col_name = sqlite3_column_name(stmt, fuzz_int32_t_4);
        }
        
        // 8. Set last insert rowid
        sqlite3_int64 rowid = 100;
        sqlite3_set_last_insert_rowid(db_source, fuzz_int64_t_5);
        
        // 9. Finalize statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    free(sql);
    
    // 10. Open destination database for backup
    if (sqlite3_open(":memory:", &db_dest) != SQLITE_OK) {
        sqlite3_mutex_leave(mutex);
        sqlite3_close(db_source);
        if (db_dest) sqlite3_close(db_dest);
        return 0;
    }
    
    // 11. Initialize backup
    backup = sqlite3_backup_init(db_dest, fuzz_str_6, db_source, fuzz_str_7);
    
    if (backup) {
        // 12. Get remaining pages
        int remaining = sqlite3_backup_remaining(backup);
        
        // 13. Use sqlite3_mprintf
        char* msg = sqlite3_mprintf("Backup remaining: %d", remaining);
        if (msg) {
            sqlite3_free(msg);
        }
        
        // 14. Perform backup step
        // Use first byte of input data to determine number of pages (with bounds)
        int nPage = (data[0] % 10) + 1; // 1-10 pages
        sqlite3_backup_step(backup, nPage);
        
        // 15. Finish backup
        sqlite3_backup_finish(backup);
    }
    
    // 16. Unlock mutex
    sqlite3_mutex_leave(mutex);
    
    // 17. Close databases
    if (db_dest) sqlite3_close(db_dest);
    if (db_source) sqlite3_close(db_source);
    
    return 0;
}