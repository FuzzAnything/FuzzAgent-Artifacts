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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"sqlite3_result_error16":[[89953,7,89953,14,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_user_data","sqlite3_result_error16","sqlite3_result_error16","sqlite3_user_data","sqlite3_result_error16","sqlite3_result_error16","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_randomness","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_result_error16","sqlite3_result_error16","sqlite3_user_data","sqlite3_result_error16","sqlite3_result_error16","sqlite3_user_data","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_randomness","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Open an in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }

    // Create a virtual table context (simulated)
    sqlite3_context *context = nullptr;
    
    // Use input data for error message
    // Ensure the error message doesn't exceed available data
    int error_msg_len = size > 100 ? 100 : (int)size;
    
    // Call sqlite3_result_error16 with part of input data as error message
    // We need a valid sqlite3_context* for this API, but we can't create one directly
    // Instead, we'll create a user-defined function to get a context
    struct FuncData {
        const uint8_t *data;
        size_t size;
    };
    
    FuncData func_data = {data, size};
    
    // Create a scalar function to get a sqlite3_context
    auto func_callback = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        FuncData *fd = static_cast<FuncData*>(sqlite3_user_data(ctx));
        
        // Use sqlite3_result_error16 with input data
        if (fd && fd->size > 0) {
            // Use first part of data as UTF-16 error message (simplified)
            // In real scenario, data should be proper UTF-16
            sqlite3_result_error16(ctx, fd->data, 
                                  fd->size > 100 ? 100 : (int)fd->size);
        } else {
            sqlite3_result_error16(ctx, u"Default error", -1);
        }
    };
    
    // Register the function
    if (sqlite3_create_function_v2(db, "fuzz_func", 0, SQLITE_UTF8, 
                                   &func_data, func_callback, 
                                   nullptr, nullptr, nullptr) == SQLITE_OK) {
        // Execute the function to trigger the callback
        sqlite3_stmt *stmt = nullptr;
        if (sqlite3_prepare_v2(db, "SELECT fuzz_func()", -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    // Generate randomness using input data size
    if (size >= sizeof(int)) {
        int n = *(int*)data % 256;  // Use first 4 bytes as N parameter
        if (n < 0) n = -n;
        if (n > 256) n = 256;
        
        uint8_t *random_buf = new uint8_t[n];
        sqlite3_randomness(n, random_buf);
        delete[] random_buf;
    }

    // Note: sqlite3_vtab_on_conflict, sqlite3_system_errno, and sqlite3_os_end
    // are not in the provided API list, so we cannot call them
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}