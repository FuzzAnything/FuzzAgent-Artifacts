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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["sqlite3_auto_extension","sqlite3_open_v2","sqlite3_close","sqlite3_extended_result_codes","sqlite3_uri_key","sqlite3_uri_key","sqlite3_create_function","sqlite3_exec","sqlite3_exec","sqlite3_snapshot_get","sqlite3_exec","sqlite3_snapshot_get","sqlite3_snapshot_cmp","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_open","sqlite3_exec","sqlite3_unlock_notify","sqlite3_exec","sqlite3_close","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_backup_finish","sqlite3_close"],"critical_calls":["sqlite3_auto_extension","sqlite3_open_v2","sqlite3_extended_result_codes","sqlite3_uri_key","sqlite3_uri_key","sqlite3_create_function","sqlite3_exec","sqlite3_exec","sqlite3_snapshot_get","sqlite3_exec","sqlite3_snapshot_get","sqlite3_snapshot_cmp","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_open","sqlite3_exec","sqlite3_unlock_notify","sqlite3_exec","sqlite3_close","sqlite3_open","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_backup_finish","sqlite3_close"],"visited":0}
/*Here's a C++ fuzzer that uses the specified SQLite3 APIs to achieve an event where we:

1. Register an auto-extension
2. Open a database with URI parameters
3. Use extended result codes
4. Compare snapshots
5. Use unlock notifications
6. Set text16 results in a custom function

The fuzzer will process SQLite3 output data as a database file:

*/


// Custom function that uses sqlite3_result_text16
static void  id_000051_custom_text16_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_TEXT) {
        const void* text = sqlite3_value_text16(argv[0]);
        int len = sqlite3_value_bytes16(argv[0]);
        sqlite3_result_text16(context, text, len, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_text16(context, u"default", 14, SQLITE_TRANSIENT);
    }
}

// Auto-extension entry point
static void  id_000051_auto_extension_entry() {
    // Do nothing - just for demonstration
}

// Unlock notification callback
static void  id_000051_unlock_notify_callback(void** args, int count) {
    // Do nothing in fuzzing context
}

// Custom VFS methods for URI handling
static int  id_000051_custom_vfs_open(sqlite3_vfs* vfs, const char* zName, sqlite3_file* file, 
                          int flags, int* pOutFlags) {
    return SQLITE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<397) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_18, fuzz_str_sz_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_snapshot* snapshot1 = nullptr;
    sqlite3_snapshot* snapshot2 = nullptr;
    sqlite3_backup* backup = nullptr;
    
    // 1. Register auto-extension
    sqlite3_auto_extension((void(*)()) id_000051_auto_extension_entry);
    
    // 2. Create a temporary database file with input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output database file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy input data to output file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 3. Open database with URI parameters
    const char* uri_params = "file:output_file?cache=shared&mode=memory";
    int rc = sqlite3_open_v2(fuzz_str_1, &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI,
                            nullptr);
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Enable extended result codes
    sqlite3_extended_result_codes(db, fuzz_int32_t_2);
    
    // 5. Extract URI parameters using sqlite3_uri_key
    sqlite3_filename filename = "file:output_file?cache=shared&mode=memory";
    const char* cache_param = sqlite3_uri_key(fuzz_str_3, fuzz_int32_t_4); // "cache"
    const char* mode_param = sqlite3_uri_key(fuzz_str_5, fuzz_int32_t_6);  // "mode"
    
    // 6. Create a custom function
    sqlite3_create_function(db, fuzz_str_7, fuzz_int32_t_8, fuzz_int32_t_9, 
                           nullptr,  id_000051_custom_text16_func, nullptr, nullptr);
    
    // 7. Create a table and insert some data
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, fuzz_str_10, nullptr, nullptr, nullptr);
    
    const char* insert_data = 
        "INSERT INTO test(data) VALUES('test data');";
    sqlite3_exec(db, fuzz_str_11, nullptr, nullptr, nullptr);
    
    // 8. Get first snapshot
    rc = sqlite3_snapshot_get(db, fuzz_str_12, &snapshot1);
    
    // 9. Make some changes
    const char* update_data = 
        "UPDATE test SET data = 'updated data' WHERE id = 1;";
    sqlite3_exec(db, fuzz_str_13, nullptr, nullptr, nullptr);
    
    // 10. Get second snapshot
    rc = sqlite3_snapshot_get(db, fuzz_str_14, &snapshot2);
    
    // 11. Compare snapshots if both were obtained successfully
    if (snapshot1 && snapshot2 && rc == SQLITE_OK) {
        int cmp_result = sqlite3_snapshot_cmp(snapshot1, snapshot2);
        // cmp_result will be negative since snapshot1 is older
    }
    
    // 12. Test the custom function
    const char* query = "SELECT custom_text16(data) FROM test;";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Function executed successfully
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 13. Set up unlock notification
    // Create a second connection to trigger lock
    sqlite3* db2 = nullptr;
    rc = sqlite3_open("output_file", &db2);
    if (rc == SQLITE_OK) {
        // Begin transaction on second connection
        sqlite3_exec(db2, fuzz_str_15, nullptr, nullptr, nullptr);
        
        // Try to access from first connection (will be locked)
        void* notify_arg = nullptr;
        rc = sqlite3_unlock_notify(db,  id_000051_unlock_notify_callback, notify_arg);
        
        // End transaction on second connection
        sqlite3_exec(db2, fuzz_str_16, nullptr, nullptr, nullptr);
        sqlite3_close(db2);
    }
    
    // 14. Create a backup to test more functionality
    sqlite3* backup_db = nullptr;
    rc = sqlite3_open(":memory:", &backup_db);
    if (rc == SQLITE_OK) {
        backup = sqlite3_backup_init(backup_db, fuzz_str_17, db, fuzz_str_18);
        if (backup) {
            sqlite3_backup_step(backup, fuzz_int32_t_19); // Copy all pages
            sqlite3_backup_finish(backup);
        }
        sqlite3_close(backup_db);
    }
    
    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    if (snapshot1) sqlite3_free(snapshot1);
    if (snapshot2) sqlite3_free(snapshot2);
    if (backup) sqlite3_backup_finish(backup);
    
    // Finalize and close database
    sqlite3_close(db);
    
    // Remove temporary file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}