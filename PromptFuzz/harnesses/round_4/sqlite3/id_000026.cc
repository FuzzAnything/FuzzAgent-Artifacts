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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 9.5, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_pointer","sqlite3_stmt_busy","sqlite3_reset","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_pointer","sqlite3_stmt_busy","sqlite3_reset","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_close"],"visited":1}
/**/


extern "C" {
}

// Dummy callback for sqlite3_exec
static int  id_000026_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

// Dummy function for sqlite3_create_function
static void  id_000026_dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_int(ctx, 0);
}

// Dummy module for sqlite3_create_module
static sqlite3_module  id_000026_dummy_module = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<111) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
	//fuzzer shim end}




    // Create a null-terminated string from the input data
    char* input_str = nullptr;
    if (size > 0) {
        input_str = static_cast<char*>(malloc(size + 1));
        if (!input_str) {
	return 0;
	}
	
        memcpy(input_str, data, size);
        input_str[size] = '\0';
    } else {
        input_str = static_cast<char*>(malloc(1));
        if (!input_str) {
	return 0;
	}
	
        input_str[0] = '\0';
    }

    // Open an in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        free(input_str);
        return 0;
    }

    // Create a function
    rc = sqlite3_create_function(db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, nullptr,  id_000026_dummy_func, nullptr, nullptr);
    
    // Create a module
    rc = sqlite3_create_module(db, fuzz_str_4, & id_000026_dummy_module, nullptr);
    
    // Create and populate a sqlite3_str object
    sqlite3_str* str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // Use sqlite3_str_appendall (assuming it's available via macro or inline)
        // Since it's not in the provided list, we'll use sqlite3_str_append instead
        sqlite3_str_append(str_obj, input_str, static_cast<int>(size));
        char* str_result = sqlite3_str_finish(str_obj);
        if (str_result) sqlite3_free(str_result);
    }

    // Execute SQL using the input as SQL statement
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, input_str,  id_000026_exec_callback, nullptr, &errmsg);
    if (errmsg) sqlite3_free(errmsg);

    // Prepare a statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare_v2(db, fuzz_str_5, -1, &stmt, &tail);
    
    if (stmt) {
        // Bind parameters
        static int dummy_int = 42;
        static const char* dummy_text = "test";
        
        rc = sqlite3_bind_int(stmt, fuzz_int32_t_6, fuzz_int32_t_7);
        rc = sqlite3_bind_text(stmt, fuzz_int32_t_9, fuzz_str_8, fuzz_int32_t_10, SQLITE_STATIC);
        rc = sqlite3_bind_pointer(stmt, fuzz_int32_t_12, &dummy_int, fuzz_str_11, nullptr);
        
        // Check if statement is busy
        int busy = sqlite3_stmt_busy(stmt);
        
        // Reset statement
        rc = sqlite3_reset(stmt);
        
        // Finalize statement
        rc = sqlite3_finalize(stmt);
    }

    // Get client data (set some first)
    static int client_data_value = 100;
    rc = sqlite3_set_clientdata(db, fuzz_str_13, &client_data_value, nullptr);
    void* client_data = sqlite3_get_clientdata(db, fuzz_str_14);

    // Close database
    rc = sqlite3_close(db);
    
    // Cleanup
    free(input_str);
    
    return 0;
}