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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"sqlite3_serialize":[[53000,9,53000,41,0,0,4,0]],"sqlite3_bind_null":[[91216,7,91216,20,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_close","sqlite3_mutex_alloc","sqlite3_mutex_try","sqlite3_exec","sqlite3_close","sqlite3_mutex_free","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_close","sqlite3_mutex_free","sqlite3_bind_null","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_extended_errcode","sqlite3_serialize","sqlite3_user_data","sqlite3_result_text16le","sqlite3_result_null","sqlite3_result_null","sqlite3_user_data","sqlite3_result_text16le","sqlite3_result_null","sqlite3_result_null","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_free","sqlite3_mutex_leave","sqlite3_mutex_free","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_mutex_alloc","sqlite3_mutex_try","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_extended_errcode","sqlite3_serialize","sqlite3_result_null","sqlite3_result_null","sqlite3_result_text16le","sqlite3_user_data","sqlite3_result_null","sqlite3_result_null","sqlite3_result_text16le","sqlite3_user_data","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_free","sqlite3_mutex_leave","sqlite3_mutex_free","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<50) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // Initialize variables
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_mutex *mutex = nullptr;
    sqlite3_context *ctx = nullptr;
    sqlite3_value *val = nullptr;
    
    // 1. Try to open an in-memory database
    if (sqlite3_open_v2(fuzz_str_1, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // 2. Create a mutex and try to acquire it
    mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
    if (mutex) {
        sqlite3_mutex_try(mutex);  // Use specified API
    }
    
    // 3. Create a table and prepare a statement
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB)";
    if (sqlite3_exec(db, create_table, nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        if (mutex) sqlite3_mutex_free(mutex);
        return 0;
    }
    
    // 4. Prepare an INSERT statement with bind parameters
    const char *insert_sql = "INSERT INTO test (id, data) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        if (stmt) sqlite3_finalize(stmt);
        sqlite3_close(db);
        if (mutex) sqlite3_mutex_free(mutex);
        return 0;
    }
    
    // 5. Use sqlite3_bind_null - bind NULL to first parameter
    if (size > 0) {
        sqlite3_bind_null(stmt, fuzz_int32_t_2);  // Use specified API
    }
    
    // 6. Bind the input data as BLOB to second parameter
    if (size > 0) {
        sqlite3_bind_blob(stmt, fuzz_int32_t_3, data, (int)(size > 1000 ? 1000 : size), SQLITE_TRANSIENT);
    }
    
    // 7. Execute the statement
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    // 8. Get extended error code
    int extended_err = sqlite3_extended_errcode(db);  // Use specified API
    
    // 9. Serialize the database using the input data flags
    sqlite3_int64 serialized_size = 0;
    unsigned int flags = 0;
    if (size > sizeof(unsigned int)) {
        memcpy(&flags, data, sizeof(unsigned int));
        flags &= 0x000000FF;  // Use only valid flag bits
    }
    
    unsigned char *serialized_data = sqlite3_serialize(db, fuzz_str_4, &serialized_size, fuzz_uint32_t_5);  // Use specified API
    
    // 10. Create a simple user-defined function that uses sqlite3_result_text16le
    struct FuncContext {
        const uint8_t *input_data;
        size_t input_size;
    };
    
    FuncContext func_ctx = {data, size};
    
    auto func_callback = [](sqlite3_context *context, int argc, sqlite3_value **argv) {
        FuncContext *ctx = static_cast<FuncContext*>(sqlite3_user_data(context));
        
        if (ctx->input_size > 0) {
            // Use sqlite3_result_text16le with input data
            // Convert first part of input to UTF-16LE (simplified - just copy bytes)
            size_t copy_size = ctx->input_size > 100 ? 100 : ctx->input_size;
            char *utf16le_buf = static_cast<char*>(malloc(copy_size * 2));
            if (utf16le_buf) {
                // Simple byte-to-UTF16LE conversion (not proper, but for fuzzing)
                for (size_t i = 0; i < copy_size; i++) {
                    utf16le_buf[i*2] = ctx->input_data[i];
                    utf16le_buf[i*2 + 1] = 0;
                }
                sqlite3_result_text16le(context, utf16le_buf, (int)(copy_size * 2), free);  // Use specified API
            } else {
                sqlite3_result_null(context);
            }
        } else {
            sqlite3_result_null(context);
        }
    };
    
    // Register the function
    sqlite3_create_function(db, fuzz_str_6, fuzz_int32_t_7, fuzz_int32_t_8, &func_ctx, 
                           func_callback, nullptr, nullptr);
    
    // 11. Call the function in a query
    const char *select_sql = "SELECT test_func()";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 12. Clean up serialized data if allocated
    if (serialized_data) {
        sqlite3_free(serialized_data);
    }
    
    // 13. Release mutex if acquired
    if (mutex) {
        sqlite3_mutex_leave(mutex);
        sqlite3_mutex_free(mutex);
    }
    
    // 14. Clean up database
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}