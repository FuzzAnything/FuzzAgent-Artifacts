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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 11
//<Quality> {"density":15,"unique_branches":{"mallocWithAlarm":[[30057,7,30057,28,0,0,4,1]],"sqlite3MallocInit":[[29976,7,29976,39,0,0,4,1]],"sqlite3PcacheInitialize":[[53842,7,53842,43,0,0,4,1]],"sqlite3_backup_finish":[[81765,7,81765,20,0,0,4,0],[81768,12,81768,18,0,0,4,1]],"sqlite3_backup_step":[[81566,27,81566,35,0,0,4,1],[81581,11,81581,34,0,0,4,1],[81583,17,81583,31,0,0,4,0]],"vfsUnlink":[[26027,13,26027,26,0,0,4,0],[26031,24,26031,38,0,0,4,1],[26034,9,26034,23,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_memory_highwater","sqlite3_memory_alarm","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_snapshot_recover","sqlite3_close","sqlite3_total_changes64","sqlite3_db_filename","sqlite3_finalize","sqlite3_close","sqlite3_release_memory","sqlite3_reset_auto_extension","sqlite3_shutdown","sqlite3_initialize"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_memory_highwater","sqlite3_memory_alarm","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_snapshot_recover","sqlite3_close","sqlite3_total_changes64","sqlite3_db_filename","sqlite3_finalize","sqlite3_close","sqlite3_release_memory","sqlite3_reset_auto_extension","sqlite3_shutdown","sqlite3_initialize"],"visited":0}
/**/


// Custom memory alarm callback
static void  id_000031_memory_alarm_callback(void *pArg, sqlite3_int64 nUsed, int n) {
    // Do nothing in fuzzing context
    (void)pArg;
    (void)nUsed;
    (void)n;
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<170) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Create a memory file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a table and insert some data
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    // Prepare a statement for inserting data
    const char *insert_sql = "INSERT INTO test(data) VALUES(?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_2, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind zero blob using input size (modulo to reasonable size)
        sqlite3_uint64 blob_size = size % 1024;  // Limit size for fuzzing safety
        rc = sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_3, blob_size);
        
        if (rc == SQLITE_OK) {
            // Execute the statement
            rc = sqlite3_step(stmt);
        }
        
        // Reset statement for potential reuse
        sqlite3_reset(stmt);
    }
    
    // Get memory highwater mark
    sqlite3_int64 highwater = sqlite3_memory_highwater(fuzz_int32_t_4);  // Reset after reading
    
    // Set up memory alarm
    rc = sqlite3_memory_alarm( id_000031_memory_alarm_callback, nullptr, fuzz_int64_t_5);
    
    // Create a user-defined function context to use auxdata APIs
    // We'll create a simple scalar function
    struct FuncContext {
        int counter;
    };
    
    // Create a backup to another database (in-memory)
    sqlite3 *db2 = nullptr;
    sqlite3_open(fuzz_str_6, &db2);
    if (db2) {
        backup = sqlite3_backup_init(db2, fuzz_str_7, db, fuzz_str_8);
        if (backup) {
            sqlite3_backup_step(backup, fuzz_int32_t_9);
            sqlite3_backup_finish(backup);
        }
        
        // Try snapshot recovery on the backup database
        rc = sqlite3_snapshot_recover(db2, fuzz_str_10);
        
        sqlite3_close(db2);
    }
    
    // Get total changes
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    
    // Get database filename (for in-memory db this returns ":memory:")
    const char *db_filename = sqlite3_db_filename(db, fuzz_str_11);
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Close database
    sqlite3_close(db);
    
    // Free any remaining memory
    sqlite3_release_memory(fuzz_int32_t_12);
    
    // Reset auto extensions if any were loaded
    sqlite3_reset_auto_extension();
    
    // Shutdown sqlite (will be reinitialized automatically if needed)
    sqlite3_shutdown();
    
    // Reinitialize for next iteration
    sqlite3_initialize();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}