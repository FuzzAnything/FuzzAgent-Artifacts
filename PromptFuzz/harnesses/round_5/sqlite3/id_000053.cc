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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"sqlite3_value_blob":[[23623,26,23623,47,3,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_shutdown","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_autovacuum_pages","sqlite3_cancel_auto_extension","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_compileoption_get","sqlite3_open","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_autovacuum_pages","sqlite3_cancel_auto_extension","sqlite3_drop_modules","sqlite3_errcode","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom function to be used with sqlite3_create_function
static void  id_000053_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        const void* data = sqlite3_value_blob(argv[0]);
        int size = sqlite3_value_bytes(argv[0]);
        if (size > 1000) {  // Arbitrary size limit for demonstration
            sqlite3_result_error_toobig(context);
        } else {
            sqlite3_result_blob(context, data, size, SQLITE_TRANSIENT);
        }
    }
}

// Callback for sqlite3_exec
static int  id_000053_exec_callback(void* data, int argc, char** argv, char** colNames) {
    return 0;
}

// Callback for sqlite3_autovacuum_pages
static unsigned int  id_000053_autovacuum_callback(void* pager, const char* schema,
                                        unsigned int pageSize,
                                        unsigned int freePages,
                                        unsigned int totalPages) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // 2. Get library version and compile options
    const char* version = sqlite3_libversion();
    const char* compile_option = sqlite3_compileoption_get(0);
    
    // 3. Write input data to a file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Copy data from input to output file
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 6. Open database using the output file
    sqlite3* db = nullptr;
    int rc = sqlite3_open("output_file", &db);
    if (rc != SQLITE_OK) {
        remove("output_file");
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // 7. Create a custom function
    sqlite3_create_function(
        db, "custom_func", 1, SQLITE_UTF8, nullptr,
         id_000053_custom_function, nullptr, nullptr
    );
    
    // 8. Prepare a statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    const char* sql = "SELECT custom_func(?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // 9. Bind zeroblob to the statement
        sqlite3_bind_zeroblob64(stmt, 1, 1024); // 1KB zeroblob
        
        // 10. Execute the statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Process rows if needed
        }
        
        // 11. Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // 12. Execute a simple SQL command
    char* errmsg = nullptr;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER);",
                  id_000053_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // 13. Set autovacuum pages callback
    sqlite3_autovacuum_pages(db,  id_000053_autovacuum_callback, nullptr, nullptr);
    
    // 14. Cancel auto-extension (using a dummy function)
    sqlite3_cancel_auto_extension(nullptr);
    
    // 15. Drop modules
    const char* keep_modules[] = {"virtualtable", nullptr};
    sqlite3_drop_modules(db, keep_modules);
    
    // 16. Check error code
    int errcode = sqlite3_errcode(db);
    
    // 17. Close database
    sqlite3_close(db);
    
    // 18. Clean up output file
    remove("output_file");
    
    // 19. Shutdown SQLite
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}