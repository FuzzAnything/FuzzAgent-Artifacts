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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_shutdown","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_result_blob64","sqlite3_result_blob64","sqlite3_create_function","sqlite3_autovacuum_pages","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_result_blob64","sqlite3_result_blob64","sqlite3_create_function","sqlite3_autovacuum_pages","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Get SQLite library version
    const char *lib_version = sqlite3_libversion();
    
    // Get compile options
    const char *compile_option = sqlite3_compileoption_get(0);
    
    // Create an in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a prepared statement
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Insert statement with zeroblob binding
    sql = "INSERT INTO test (id, data) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind parameters
        sqlite3_bind_int(stmt, 1, 1);
        
        // Use input data size for zeroblob
        if (size > 0) {
            sqlite3_bind_zeroblob64(stmt, 2, static_cast<sqlite3_uint64>(size));
        } else {
            sqlite3_bind_zeroblob64(stmt, 2, 1024); // Default size
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Create a custom function that uses sqlite3_result_blob64
    auto blob_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        static uint8_t static_data[] = {1, 2, 3, 4, 5};
        sqlite3_result_blob64(ctx, static_data, sizeof(static_data), SQLITE_TRANSIENT);
    };
    
    sqlite3_create_function(db, "myblob", 0, SQLITE_UTF8, nullptr, 
                           blob_func, nullptr, nullptr);
    
    // Use the autovacuum pages callback
    auto autovacuum_callback = [](void *pCtx, const char *zSchema, 
                                  unsigned int nPage, unsigned int nFree, 
                                  unsigned int nPageSize) -> unsigned int {
        return nFree > 10 ? 1 : 0;
    };
    
    sqlite3_autovacuum_pages(db, autovacuum_callback, nullptr, nullptr);
    
    // Drop all modules except the essential ones
    const char *keep_modules[] = {"sqlite3", nullptr};
    sqlite3_drop_modules(db, keep_modules);
    
    // Use input data to create a FILE stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some data from the file stream
        uint8_t buffer[256];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        
        // Create an output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(buffer, 1, bytes_read, out_file);
            assert_file_closed(&out_file);;
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Check error code
    int err = sqlite3_errcode(db);
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	return 0;
}