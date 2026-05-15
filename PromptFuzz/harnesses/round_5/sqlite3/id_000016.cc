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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"bindText":[[91157,13,91157,32,0,0,4,0],[91157,36,91157,58,0,0,4,1]]},"library_calls":["sqlite3_thread_cleanup","sqlite3_open","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_blob","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_db_release_memory","sqlite3_db_filename","sqlite3_filename_database","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_thread_cleanup"],"critical_calls":["sqlite3_thread_cleanup","sqlite3_open","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_db_release_memory","sqlite3_db_filename","sqlite3_filename_database","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_thread_cleanup"],"visited":0}
/**/


// Custom user-defined function for sqlite3_result_error_toobig
static void  id_000016_test_error_toobig(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // If any argument is too large, trigger the error
    for (int i = 0; i < argc; i++) {
        if (sqlite3_value_bytes(argv[i]) > 1000) {
            sqlite3_result_error_toobig(context);
            return;
        }
    }
    sqlite3_result_null(context);
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Clean up thread-specific resources
    sqlite3_thread_cleanup();
    
    // Open database in memory
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a custom function that uses sqlite3_result_error_toobig
    sqlite3_create_function(db, "test_toobig", 1, SQLITE_UTF8, nullptr, 
                            id_000016_test_error_toobig, nullptr, nullptr);
    
    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "CREATE TABLE test (id INTEGER, data BLOB); "
                      "INSERT INTO test VALUES (1, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind the input data as blob
    if (size > 0) {
        sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size), SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_blob(stmt, 1, nullptr, 0, SQLITE_TRANSIENT);
    }
    
    // Execute the statement
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Try to release memory from database
    sqlite3_db_release_memory(db);
    
    // Get database filename (though in-memory db returns null)
    sqlite3_filename filename = sqlite3_db_filename(db, "main");
    if (filename) {
        const char *db_name = sqlite3_filename_database(filename);
        // db_name may be null for in-memory database
        (void)db_name;
    }
    
    // Create another statement to test our custom function
    stmt = nullptr;
    sql = "SELECT test_toobig(data) FROM test";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Call thread cleanup again to ensure clean state
    sqlite3_thread_cleanup();
    
    return 0;
}