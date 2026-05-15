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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 255, nr_unique_branch: 38
//<Quality> {"density":15,"unique_branches":{"sqlite3_error_offset":[[180260,47,180260,58,0,0,4,1]],"sqlite3LockAndPrepare":[[141066,7,141066,32,0,0,4,0]],"sqlite3SafetyCheckOk":[[35969,7,35969,36,0,0,4,0],[35970,9,35970,39,0,0,4,0],[14098,27,14098,28,2,0,4,1]],"sqlite3_overload_function":[[179762,7,179762,32,0,0,4,0],[179769,7,179769,9,0,0,4,0]],"invokeProfileCallback":[[89543,7,89543,19,0,0,4,1],[89547,7,89547,40,0,0,4,0]],"unixFullPathname":[[44432,7,44432,14,0,0,4,0],[44432,18,44432,30,0,0,4,0]],"sqlite3VdbeExec":[[100996,9,100996,41,0,0,4,1],[101002,9,101002,24,0,0,4,1]],"sqlite3_bind_pointer":[[91231,7,91231,20,0,0,4,1],[91234,13,91234,24,0,0,4,0]],"unixOpen":[[44101,17,44101,30,0,0,4,1],[44101,34,44101,45,0,0,4,0]],"sqlite3_unlock_notify":[[182811,7,182811,32,0,0,4,0]],"sqlite3_exec":[[134864,7,134864,32,0,0,4,0]],"vdbeCommit":[[86901,11,86901,41,0,0,4,1]],"appendOnePathElement":[[44332,7,44332,20,0,0,4,0],[44333,9,44333,17,0,0,4,0],[44333,9,44333,17,0,0,4,1],[44334,9,44334,22,0,0,4,0],[44334,9,44334,22,0,0,4,1],[44334,26,44334,34,0,0,4,0],[44334,26,44334,34,0,0,4,1],[44335,11,44335,25,0,0,4,0],[44335,11,44335,25,0,0,4,1],[44337,16,44337,48,0,0,4,0],[44337,16,44337,48,0,0,4,1],[44350,7,44350,27,0,0,4,1],[44356,11,44356,24,0,0,4,0]],"sqlite3PcacheMakeDirty":[[54137,7,54137,48,0,0,4,1]],"sqlite3PagerOpen":[[61199,11,61199,32,0,0,4,1],[61215,9,61215,22,0,0,4,1],[61224,9,61224,22,0,0,4,0]],"sqlite3_trace_v2":[[179814,7,179814,32,0,0,4,0]]},"library_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_trace_v2","sqlite3_overload_function","sqlite3_exec","sqlite3_prepare","sqlite3_bind_int","sqlite3_bind_text64","sqlite3_bind_pointer","sqlite3_step","sqlite3_finalize","sqlite3_unlock_notify","sqlite3_error_offset","sqlite3_vtab_rhs_value","sqlite3_free_filename","sqlite3_os_end","sqlite3_close_v2"],"critical_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_trace_v2","sqlite3_overload_function","sqlite3_exec","sqlite3_prepare","sqlite3_bind_int","sqlite3_bind_text64","sqlite3_bind_pointer","sqlite3_step","sqlite3_finalize","sqlite3_unlock_notify","sqlite3_error_offset","sqlite3_vtab_rhs_value","sqlite3_free_filename","sqlite3_os_end","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    // 1. Create filename using input data
    const char* db_name = reinterpret_cast<const char*>(data);
    size_t db_name_len = size > 100 ? 100 : size;
    char* temp_db_name = static_cast<char*>(malloc(db_name_len + 1));
    if (!temp_db_name) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    memcpy(temp_db_name, db_name, db_name_len);
    temp_db_name[db_name_len] = '\0';

    sqlite3_filename filename = sqlite3_create_filename(
        temp_db_name,  // Database
        "journal_file", // Journal
        "wal_file",     // WAL
        0,              // nParam
        nullptr         // azParam
    );

    // 2. Open database
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    
    // 3. Set up trace callback
    unsigned int trace_mask = SQLITE_TRACE_STMT | SQLITE_TRACE_PROFILE;
    sqlite3_trace_v2(db, trace_mask, 
        [](unsigned int, void*, void*, void*) -> int {
            return 0;
        }, 
        nullptr
    );

    // 4. Overload a function
    sqlite3_overload_function(db, "substr", 2);

    // 5. Prepare a statement and bind parameters
    sqlite3_stmt* stmt = nullptr;
    const char* create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data TEXT)";
    sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);

    const char* insert_sql = "INSERT INTO test VALUES (?, ?)";
    if (sqlite3_prepare(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind integer
        sqlite3_bind_int(stmt, 1, static_cast<int>(size));
        
        // Bind text64 using input data
        const char* text_data = reinterpret_cast<const char*>(data);
        size_t text_len = size > 50 ? 50 : size;
        sqlite3_bind_text64(stmt, 2, text_data, text_len, 
                           SQLITE_TRANSIENT, SQLITE_UTF8);
        
        // Bind pointer
        void* ptr = malloc(16);
        if (ptr) {
            sqlite3_bind_pointer(stmt, 3, ptr, "test_pointer",
                [](void* p) { free(p); });
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // 6. Set up unlock notification
    sqlite3_unlock_notify(db, 
        [](void** apArg, int nArg) {
            // Notification callback
        }, 
        nullptr
    );

    // 7. Get error offset (if any error occurred)
    int error_offset = sqlite3_error_offset(db);
    (void)error_offset;

    // 8. Attempt to use vtab_rhs_value (requires virtual table setup)
    // We'll create a minimal virtual table structure
    sqlite3_index_info idx_info;
    memset(&idx_info, 0, sizeof(idx_info));
    sqlite3_value* rhs_val = nullptr;
    sqlite3_vtab_rhs_value(&idx_info, 0, &rhs_val);

    // 9. Clean up
    free(temp_db_name);
    sqlite3_free_filename(filename);
    
    if (db) {
        // Call os_end (note: this is global cleanup, use with caution)
        sqlite3_os_end();
        sqlite3_close_v2(db);
    }

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}