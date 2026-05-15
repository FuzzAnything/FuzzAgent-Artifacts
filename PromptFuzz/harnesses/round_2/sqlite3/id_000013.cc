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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 432, nr_unique_branch: 74
//<Quality> {"density":24,"unique_branches":{"sqlite3BtreeCount":[[80111,7,80111,23,0,0,4,1],[80119,10,80119,23,0,0,4,0],[80119,27,80119,38,0,0,4,0],[80128,9,80128,20,0,0,4,0],[80142,9,80142,20,0,0,4,0],[80144,13,80144,27,0,0,4,0]],"countOfViewOptimization":[[148373,7,148373,47,0,0,4,1],[148375,7,148375,24,0,0,4,1],[148376,7,148376,23,0,0,4,1],[148379,7,148379,14,0,0,4,0],[18919,34,18919,55,3,0,4,1]],"sqlite3_status":[[23778,7,23778,18,0,0,4,1],[23778,22,23778,35,0,0,4,1],[23781,7,23781,12,0,0,4,0]],"isSimpleCount":[[146727,7,146727,28,0,0,4,1],[146728,7,146728,25,0,0,4,1],[146729,7,146729,17,0,0,4,1],[146736,7,146736,23,0,0,4,1],[146739,7,146739,33,0,0,4,1],[146740,7,146740,32,0,0,4,1],[146741,7,146741,65,0,0,4,1],[14098,27,14098,28,4,0,4,1],[14098,27,14098,28,5,0,4,1],[18919,34,18919,55,6,0,4,1]],"yy_reduce":[[174769,7,174769,15,0,0,4,0]],"sqlite3Realloc":[[30348,20,30349,36,0,0,4,0],[30351,11,30351,27,0,0,4,0],[30351,31,30351,62,0,0,4,1]],"explainSimpleCount":[[148206,7,148206,25,0,0,4,1]],"sqlite3Select":[[148901,30,148901,45,0,0,4,0],[149650,11,149650,49,0,0,4,0],[149683,13,149683,22,0,0,4,1],[149684,13,149684,41,0,0,4,0],[149685,34,149685,38,0,0,4,1],[149695,13,149695,18,0,0,4,1],[149702,13,149702,21,0,0,4,1]],"sqlite3_db_handle":[[91429,10,91429,15,0,0,4,0]],"resolveExprStep":[[14098,27,14098,28,61,0,4,0]],"sqlite3MallocAlarm":[[30033,7,30033,29,0,0,4,1]],"sqlite3_soft_heap_limit":[[29936,7,29936,10,0,0,4,1]],"sqlite3_soft_heap_limit64":[[29916,7,29916,9,0,0,4,1],[29920,7,29920,10,0,0,4,1],[29924,7,29924,23,0,0,4,0],[29924,28,29924,44,0,0,4,0],[29924,28,29924,44,0,0,4,1],[29924,48,29924,52,0,0,4,1],[29932,7,29932,15,0,0,4,0],[29932,7,29932,15,0,0,4,1],[13869,56,13869,59,9,0,4,0],[13869,56,13869,59,9,0,4,0],[13869,56,13869,59,9,0,4,1]],"sqlite3VdbeSerialGet":[[88087,5,88087,11,0,0,4,0],[88095,5,88095,11,0,0,4,0],[14098,27,14098,28,19,0,4,0],[14098,27,14098,28,19,0,4,1]],"sqlite3VdbeExec":[[95735,17,95735,38,0,0,4,0],[95740,13,95740,38,0,0,4,1],[95856,11,95856,17,0,0,4,0],[14098,27,14098,28,385,0,4,0],[14098,27,14098,28,386,0,4,0],[14098,27,14098,28,387,0,4,0],[14098,27,14098,28,388,0,4,0],[14098,27,14098,28,389,0,4,0],[14098,27,14098,28,390,0,4,0],[14098,27,14098,28,391,0,4,0],[14098,27,14098,28,392,0,4,0],[14098,27,14098,28,393,0,4,0],[14098,27,14098,28,394,0,4,0]],"mallocWithAlarm":[[30059,9,30059,45,0,0,4,0],[30062,11,30062,25,0,0,4,0],[30064,13,30064,44,0,0,4,1]],"serialGet":[[88005,7,88005,21,0,0,4,0],[14098,27,14098,28,4,0,4,0],[14098,27,14098,28,4,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_close","sqlite3_shutdown","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_soft_heap_limit","sqlite3_soft_heap_limit64","sqlite3_prepare_v3","sqlite3_db_handle","sqlite3_step","sqlite3_finalize","sqlite3_status","sqlite3_exec","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_result_int","sqlite3_result_int","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_close_v2","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_soft_heap_limit","sqlite3_soft_heap_limit64","sqlite3_prepare_v3","sqlite3_db_handle","sqlite3_step","sqlite3_finalize","sqlite3_status","sqlite3_exec","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_result_int","sqlite3_result_int","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_close_v2","sqlite3_shutdown"],"visited":0}
/*Looking at the requested event description, I need to create a fuzz driver that uses all the specified SQLite3 APIs together in a coherent scenario. Since `sqlite3_prepare16` and `sqlite3_keyword_name` aren't in the provided API list, I'll use alternative APIs that achieve similar functionality.

*/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if insufficient data
    if (size < 32) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    FILE *in_file = nullptr;
    const char *db_name = ":memory:";
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Open in-memory database
    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a FILE* from input data for potential reading
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_close(db);
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use sqlite3_bind_int64
    // Create a simple table and prepare a statement
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, value INTEGER)";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    const char *insert_sql = "INSERT INTO test(value) VALUES(?)";
    if (sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        // Extract first 8 bytes for int64 binding
        sqlite3_int64 bind_value = 0;
        size_t bytes_to_read = (size > sizeof(sqlite3_int64)) ? sizeof(sqlite3_int64) : size;
        if (fread(&bind_value, 1, bytes_to_read, in_file) == bytes_to_read) {
            sqlite3_bind_int64(stmt, 1, bind_value);
            sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 2. Use sqlite3_soft_heap_limit and sqlite3_soft_heap_limit64
    // Set soft heap limits based on input data
    if (size > 16) {
        // Use first 4 bytes for integer limit
        int limit_value = 0;
        memcpy(&limit_value, data, sizeof(int));
        limit_value = (limit_value & 0xFFFFF) + 1024; // Keep reasonable
        sqlite3_soft_heap_limit(limit_value);
        
        // Use next 8 bytes for 64-bit limit
        sqlite3_int64 limit64_value = 0;
        memcpy(&limit64_value, data + 4, sizeof(sqlite3_int64));
        limit64_value = (limit64_value & 0xFFFFFFF) + 2048; // Keep reasonable
        sqlite3_soft_heap_limit64(limit64_value);
    }
    
    // 3. Use sqlite3_db_handle
    // Prepare another statement and get its database handle
    const char *select_sql = "SELECT * FROM test";
    if (sqlite3_prepare_v3(db, select_sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
        // Verify the handle matches
        if (db_from_stmt != db) {
            // Handle mismatch - not an error, but note it
        }
        
        // Try to step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Do nothing with results for fuzzing
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 4. Simulate memory alarm functionality (not in provided APIs)
    // We'll use sqlite3_status as an alternative to monitor memory
    int current = 0, highwater = 0;
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 0);
    
    // 5. Simulate keyword functionality (not in provided APIs)
    // Use sqlite3_exec with a dynamic SQL statement instead
    if (size > 24) {
        // Extract a small SQL snippet from input
        char sql_snippet[256] = {0};
        size_t snippet_len = (size - 24 > 255) ? 255 : size - 24;
        memcpy(sql_snippet, data + 16, snippet_len);
        sql_snippet[snippet_len] = '\0';
        
        // Create a simple SQL statement
        char dynamic_sql[512];
        snprintf(dynamic_sql, sizeof(dynamic_sql), 
                "CREATE TABLE IF NOT EXISTS fuzz%d(x INTEGER)", 
                (int)(data[0] % 100));
        
        sqlite3_exec(db, dynamic_sql, nullptr, nullptr, nullptr);
    }
    
    // 6. Prepare statement using UTF-16 (simulate prepare16 functionality)
    // We'll use sqlite3_prepare_v3 with regular string since UTF-16 not available
    const char *utf8_sql = "SELECT count(*) FROM test";
    if (sqlite3_prepare_v3(db, utf8_sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 7. Simulate vtab functionality (not in provided APIs)
    // Create a function instead
    sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, nullptr, 
                           [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
                               sqlite3_result_int(ctx, 42);
                           }, nullptr, nullptr);
    
    // Use the function
    const char *func_sql = "SELECT fuzz_func(5)";
    if (sqlite3_prepare_v3(db, func_sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 8. Clean up and finalize
    // Create a backup to consume remaining data
    if (size > 100) {
        sqlite3 *db2 = nullptr;
        if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
            sqlite3_backup *backup = sqlite3_backup_init(db2, "main", db, "main");
            if (backup) {
                sqlite3_backup_step(backup, 1); // Partial backup
                sqlite3_backup_finish(backup);
            }
            sqlite3_close(db2);
        }
    }
    
    // Release all allocated resources
    if (in_file) assert_file_closed(&in_file);;
    
    // Write output file (optional, for demonstration)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some debug info
        fprintf(out_file, "Fuzzing completed. Input size: %zu\n", size);
        assert_file_closed(&out_file);;
    }
    
    // Clean up database
    if (db) {
        // Use close_v2 for better cleanup
        sqlite3_close_v2(db);
    }
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}