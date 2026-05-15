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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 7
//<Quality> {"density":22,"unique_branches":{"sqlite3FkActions":[[131593,7,131593,43,0,0,4,0],[131595,44,131595,49,0,0,4,1]],"sqlite3FkDropTable":[[130905,7,130905,37,0,0,4,0],[130911,9,130911,37,0,0,4,0],[130918,32,130918,33,0,0,4,1],[130921,11,130921,13,0,0,4,0],[18323,29,18323,57,2,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_create_function","sqlite3_sourceid","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_create_function","sqlite3_sourceid","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_exec","sqlite3_close"],"visited":0}
/**/


// Custom function implementations
static void  id_000046_myfunc(sqlite3_context* context, int argc, sqlite3_value** argv) {
    sqlite3_result_int(context, 42); // Always return 42
}

static void  id_000046_myfunc2(sqlite3_context* context, int argc, sqlite3_value** argv) {
    int sum = 0;
    for (int i = 0; i < argc; i++) {
        sum += sqlite3_value_int(argv[i]);
    }
    sqlite3_result_int(context, sum);
}

// Rollback hook callback
static void  id_000046_rollback_hook_callback(void* arg) {
    // Nothing to do here, just a placeholder
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_stmt* stmt2 = nullptr;
    char* errmsg = nullptr;
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    
    // Create input file from fuzzer data
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    (void)fd_in; // Mark as used
    (void)fd_out; // Mark as used
    
    // 1. Open database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        cleanup:
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        if (db) sqlite3_close(db);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Set rollback hook
    sqlite3_rollback_hook(db,  id_000046_rollback_hook_callback, nullptr);
    
    // 3. Create custom functions (calling create_function twice as shown in the list)
    sqlite3_create_function(db, " id_000046_myfunc", 1, SQLITE_UTF8, nullptr, 
                            id_000046_myfunc, nullptr, nullptr);
    sqlite3_create_function(db, " id_000046_myfunc2", 2, SQLITE_UTF8, nullptr,
                            id_000046_myfunc2, nullptr, nullptr);
    
    // Get source ID
    const char* sourceid = sqlite3_sourceid();
    (void)sourceid; // Mark as used
    
    // 4. Execute SQL commands
    const char* create_table = 
        "CREATE TABLE test(id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test(data) VALUES(x'4142434445');"
        "SELECT  id_000046_myfunc(5);"
        "SELECT  id_000046_myfunc2(3, 4);";
    
    if (sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
    }
    
    // 5. Prepare statement
    const char* sql = "INSERT INTO test(data) VALUES(?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        goto cleanup;
    }
    
    // 6. Bind parameters
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count > 0) {
        // Use the fuzzer input as blob data
        sqlite3_bind_blob(stmt, 1, data, 
                         (size < 1000) ? (int)size : 1000, 
                         SQLITE_STATIC);
    }
    
    // 7. Execute statement
    sqlite3_step(stmt);
    
    // Prepare another statement for reading
    const char* select_sql = "SELECT data FROM test;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt2) == SQLITE_ROW) {
            int col_type = sqlite3_column_type(stmt2, 0);
            if (col_type == SQLITE_BLOB) {
                const void* blob = sqlite3_column_blob(stmt2, 0);
                int blob_size = sqlite3_column_bytes(stmt2, 0);
                (void)blob; // Mark as used
                (void)blob_size; // Mark as used
            }
        }
    }
    
    // 8. Finalize statements
    if (stmt) sqlite3_finalize(stmt);
    if (stmt2) sqlite3_finalize(stmt2);
    
    // 9. WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);
    
    // 10. Database configuration
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, nullptr);
    
    // 11. Test control
    sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0x00000100);
    
    // 12. Get error code
    int errcode = sqlite3_errcode(db);
    (void)errcode; // Mark as used
    
    // Cleanup
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Final database operations
    if (db) {
        // Execute one more operation before closing
        sqlite3_exec(db, "DROP TABLE test;", nullptr, nullptr, nullptr);
        sqlite3_close(db);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}