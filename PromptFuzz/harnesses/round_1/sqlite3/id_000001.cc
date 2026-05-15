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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"sqlite3_backup_step":[[81566,16,81566,23,0,0,4,0]],"sqlite3_soft_heap_limit64":[[29932,7,29932,15,0,0,4,0]],"sqlite3Insert":[[132532,15,132532,19,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_busy_handler","sqlite3_threadsafe","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_decltype","sqlite3_column_decltype","sqlite3_column_int64","sqlite3_column_text","sqlite3_reset","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_busy_handler","sqlite3_threadsafe","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_decltype","sqlite3_column_decltype","sqlite3_column_int64","sqlite3_column_text","sqlite3_reset","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_finalize","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    
    // Initialize with memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set soft heap limit
    sqlite3_int64 heap_limit = (size > 0) ? (sqlite3_int64)data[0] * 1024 : 1024 * 1024;
    sqlite3_soft_heap_limit64(heap_limit);
    
    // 2. Set busy handler
    static int busy_counter = 0;
    auto busy_callback = [](void*, int count) -> int {
        return (count < 5) ? 1 : 0; // Retry 5 times
    };
    sqlite3_busy_handler(db, busy_callback, nullptr);
    
    // 3. Check if library is threadsafe
    int is_threadsafe = sqlite3_threadsafe();
    
    // Create a simple table and insert data
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, value TEXT)";
    sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    
    // Prepare INSERT statement with parameter binding
    const char *insert_sql = "INSERT INTO test (id, value) VALUES (?, ?)";
    if (sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        // 4. Bind integer value to first parameter
        sqlite3_int64 bind_value = (size > 4) ? 
            (sqlite3_int64)((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]) : 
            12345;
        sqlite3_bind_int64(stmt, 1, bind_value);
        
        // Bind text value (using a portion of input data)
        const char *text_value = (size > 8) ? 
            reinterpret_cast<const char*>(data + 4) : "default";
        int text_len = (size > 8) ? std::min((int)size - 4, 100) : 7;
        sqlite3_bind_text(stmt, 2, text_value, text_len, SQLITE_TRANSIENT);
        
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    
    // Prepare SELECT statement
    const char *select_sql = "SELECT id, value FROM test";
    if (stmt) sqlite3_finalize(stmt);
    stmt = nullptr;
    
    if (sqlite3_prepare_v3(db, select_sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // 5. Get column decltype
            const char *decltype1 = sqlite3_column_decltype(stmt, 0);
            const char *decltype2 = sqlite3_column_decltype(stmt, 1);
            
            // Read values
            sqlite3_int64 id = sqlite3_column_int64(stmt, 0);
            const unsigned char *text = sqlite3_column_text(stmt, 1);
        }
        sqlite3_reset(stmt);
    }
    
    // Create a backup to another memory database
    sqlite3 *db2 = nullptr;
    if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
        backup = sqlite3_backup_init(db2, "main", db, "main");
        if (backup) {
            sqlite3_backup_step(backup, -1);
            sqlite3_backup_finish(backup);
        }
        sqlite3_close(db2);
    }
    
    // Try to create a snapshot comparison scenario
    // Note: sqlite3_snapshot_cmp requires snapshots from WAL mode
    // We'll simulate this by comparing placeholder snapshots
    sqlite3_snapshot *snap1 = nullptr;
    sqlite3_snapshot *snap2 = nullptr;
    
    // In real usage, we would obtain snapshots from database
    // For fuzzing, we'll create minimal mock comparison
    if (size > 0) {
        // Use data to simulate comparison result
        int cmp_result = (size > 1) ? (data[0] - data[1]) : 0;
        // Note: Actual snapshot comparison would need valid snapshot objects
        // sqlite3_snapshot_cmp(snap1, snap2);
    }
    
    // Clean up
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}