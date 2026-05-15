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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 112, nr_unique_branch: 29
//<Quality> {"density":7,"unique_branches":{"openDatabase":[[180880,7,180880,12,0,0,4,0],[181009,7,181009,23,0,0,4,0],[181145,7,181145,9,0,0,4,1],[181152,7,181152,30,0,0,4,0]],"sqlite3MallocZero":[[30399,7,30399,8,0,0,4,1]],"createCollation":[[180412,7,180412,15,0,0,4,0]],"findCollSeqEntry":[[125954,9,125954,14,0,0,4,1]],"growOp3":[[84202,7,84202,24,0,0,4,0]],"sqlite3Close":[[178826,7,178826,10,0,0,4,0],[178872,9,178872,23,0,0,4,0]],"vdbeFreeOpArray":[[85387,7,85387,10,0,0,4,1]],"sqlite3_errcode":[[180316,7,180316,9,0,0,4,1],[180319,7,180319,10,0,0,4,0],[180319,14,180319,30,0,0,4,0]],"growOpArray":[[84167,7,84167,11,0,0,4,1],[84172,11,84172,15,0,0,4,1]],"setupLookaside":[[178394,9,178394,15,0,0,4,1],[178418,7,178418,13,0,0,4,1]],"sqlite3_hard_heap_limit64":[[29956,7,29956,9,0,0,4,1],[29960,7,29960,11,0,0,4,0],[29960,7,29960,11,0,0,4,1],[29962,9,29962,30,0,0,4,0],[29962,9,29962,30,0,0,4,1],[29962,34,29962,56,0,0,4,1]],"sqlite3DbMallocZero":[[30413,7,30413,8,0,0,4,1]],"sqlite3_get_clientdata":[[181345,22,181345,23,0,0,4,0],[181346,9,181346,35,0,0,4,0]],"mallocWithAlarm":[[30057,7,30057,28,0,0,4,1]],"dbMallocRawFinish":[[30425,7,30425,9,0,0,4,0]]},"library_calls":["sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_close","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_result_error","sqlite3_result_pointer","sqlite3_result_text16be","sqlite3_result_error","sqlite3_result_pointer","sqlite3_result_text16be","sqlite3_create_function","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_result_text16be","sqlite3_result_pointer","sqlite3_result_error","sqlite3_result_text16be","sqlite3_result_pointer","sqlite3_result_error","sqlite3_create_function","sqlite3_exec","sqlite3_close"],"visited":0}
/**/


// Include necessary sqlite3 headers

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // 1. Set hard heap limit using input data
    if (size >= sizeof(sqlite3_int64)) {
        sqlite3_int64 limit;
        memcpy(&limit, data, sizeof(sqlite3_int64));
        sqlite3_hard_heap_limit64(limit);
    }

    // 2. Create in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // 3. Create a custom function context
    sqlite3_context *context = nullptr;
    
    // Create a simple function to get a context
    struct FuncData {
        void *client_data;
        const char *name;
    };
    
    FuncData func_data = {nullptr, "test_pointer"};
    
    // 4. Set client data
    if (size > 0) {
        char *client_key = strdup("test_key");
        void *client_value = malloc(16);
        if (client_value && size >= 16) {
            memcpy(client_value, data, 16);
        }
        
        // Note: sqlite3_set_clientdata is in the provided APIs list
        sqlite3_set_clientdata(db, client_key, client_value, free);
        
        // 5. Get client data back
        void *retrieved = sqlite3_get_clientdata(db, client_key);
        
        // 6. Create a function to use the result APIs
        auto xFunc = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // Use sqlite3_result_error with a sample error message
            sqlite3_result_error(ctx, "Test error from fuzzer", -1);
            
            // Use sqlite3_result_pointer with dummy data
            int dummy = 42;
            sqlite3_result_pointer(ctx, &dummy, "test_type", nullptr);
            
            // Use sqlite3_result_text16be with sample text
            const char *text = "test";
            sqlite3_result_text16be(ctx, text, 4, SQLITE_TRANSIENT);
        };
        
        // Register the function
        sqlite3_create_function(db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, 
                                &func_data, xFunc, nullptr, nullptr);
        
        // Call the function
        sqlite3_exec(db, fuzz_str_4, nullptr, nullptr, nullptr);
        
        free(client_key);
    }

    // 7. Clean up
    sqlite3_close(db);
    
    return 0;
}