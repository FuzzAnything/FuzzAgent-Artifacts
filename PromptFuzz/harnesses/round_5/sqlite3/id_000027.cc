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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_shutdown","sqlite3_drop_modules","sqlite3_autovacuum_pages","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_result_blob64","sqlite3_result_blob64","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_drop_modules","sqlite3_autovacuum_pages","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_result_blob64","sqlite3_result_blob64","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Get SQLite version
    const char *version = sqlite3_libversion();
    
    // Get compile option (just get first one)
    const char *compile_option = sqlite3_compileoption_get(0);
    
    // Create a temporary database file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a virtual table module and then drop it
    const char *module_name = "test_module";
    const char *keep_modules[] = {nullptr}; // Keep no modules
    (void)sqlite3_drop_modules(db, keep_modules);
    
    // Set up autovacuum pages callback (dummy implementation)
    auto autovacuum_callback = [](void *, const char *, unsigned int, unsigned int, unsigned int) -> unsigned int {
        return 0;
    };
    (void)sqlite3_autovacuum_pages(db, autovacuum_callback, nullptr, nullptr);
    
    // Create a simple table
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB)";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test (id, data) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind parameters
        sqlite3_bind_int(stmt, 1, 1);
        
        // Use input data for blob binding if we have enough data
        if (size > 0) {
            // Use first 8 bytes as zeroblob size if available
            sqlite3_uint64 zeroblob_size = 0;
            if (size >= sizeof(sqlite3_uint64)) {
                memcpy(&zeroblob_size, data, sizeof(sqlite3_uint64));
                // Limit to reasonable size to avoid excessive memory usage
                if (zeroblob_size > 1024 * 1024) {
                    zeroblob_size = 1024 * 1024;
                }
            } else {
                zeroblob_size = size;
            }
            
            // Bind zeroblob
            (void)sqlite3_bind_zeroblob64(stmt, 2, zeroblob_size);
        } else {
            sqlite3_bind_zeroblob64(stmt, 2, 0);
        }
        
        // Execute statement
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Create a custom function that uses sqlite3_result_blob64
    auto custom_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        static const char dummy_data[] = "test";
        // Use input data size if available through closure
        sqlite3_result_blob64(ctx, dummy_data, sizeof(dummy_data), SQLITE_TRANSIENT);
    };
    
    // Register the custom function
    sqlite3_create_function(db, "myfunc", 0, SQLITE_UTF8, nullptr, 
                           custom_func, nullptr, nullptr);
    
    // Execute a query using the custom function
    sqlite3_stmt *select_stmt = nullptr;
    const char *select_sql = "SELECT myfunc()";
    if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(select_stmt);
        sqlite3_finalize(select_stmt);
    }
    
    // Get error code
    int errcode = sqlite3_errcode(db);
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    // Consume input data by using it in a simple hash
    // This ensures all input bytes are processed
    uint32_t hash = 0;
    for (size_t i = 0; i < size; i++) {
        hash = (hash * 31) + data[i];
    }
    // Use the hash to prevent optimization
    if (hash == 0xdeadbeef) {
        printf("Impossible condition\n");
    }
    
    return 0;
}