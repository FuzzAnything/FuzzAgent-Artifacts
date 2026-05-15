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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 6
//<Quality> {"density":11,"unique_branches":{"sqlite3_str_value":[[31839,7,31839,11,0,0,4,1],[31839,15,31839,26,0,0,4,1]],"sqlite3_declare_vtab":[[154462,7,154462,32,0,0,4,1],[154462,36,154462,51,0,0,4,1],[154468,7,154468,12,0,0,4,0]],"openDatabase":[[180851,13,180851,51,0,0,4,0]]},"library_calls":["sqlite3_enable_shared_cache","sqlite3_open","sqlite3_str_new","sqlite3_malloc","sqlite3_realloc64","sqlite3_str_appendf","sqlite3_str_value","sqlite3_user_data","sqlite3_result_text","sqlite3_user_data","sqlite3_result_text","sqlite3_create_function_v2","sqlite3_declare_vtab","sqlite3_str_finish","sqlite3_free","sqlite3_free","sqlite3_close_v2","sqlite3_enable_shared_cache"],"critical_calls":["sqlite3_enable_shared_cache","sqlite3_open","sqlite3_str_new","sqlite3_malloc","sqlite3_realloc64","sqlite3_str_appendf","sqlite3_str_value","sqlite3_result_text","sqlite3_user_data","sqlite3_result_text","sqlite3_user_data","sqlite3_create_function_v2","sqlite3_declare_vtab","sqlite3_str_finish","sqlite3_free","sqlite3_free","sqlite3_close_v2","sqlite3_enable_shared_cache"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Enable shared cache
    sqlite3_enable_shared_cache(1);
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open a database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a sqlite3_str object
    sqlite3_str *str_obj = sqlite3_str_new(db);
    
    // Read some data from input file for reallocation
    fseek(in_file, 0, SEEK_SET);
    size_t read_size = size > 100 ? 100 : size;
    char *buffer = (char *)sqlite3_malloc(read_size);
    if (buffer && read_size > 0) {
        fread(buffer, 1, read_size, in_file);
        
        // Use sqlite3_realloc64 to reallocate memory
        void *realloc_buffer = sqlite3_realloc64(buffer, size);
        if (realloc_buffer) {
            // Copy remaining data if any
            if (size > read_size) {
                fseek(in_file, read_size, SEEK_SET);
                fread((char *)realloc_buffer + read_size, 1, size - read_size, in_file);
            }
            
            // Create a virtual table using the reallocated buffer as SQL
            // We need to ensure it's null-terminated
            char *sql = (char *)realloc_buffer;
            if (size > 0) {
                sql[size - 1] = '\0';
            }
            
            // Declare a virtual table using the data
            if (str_obj) {
                // Append some data to the string object
                sqlite3_str_appendf(str_obj, "Virtual table SQL: %s", sql);
                
                // Get the string value
                char *str_value = sqlite3_str_value(str_obj);
                
                // Create a function context for sqlite3_result_pointer
                // We'll create a simple scalar function
                struct FuncContext {
                    void *data;
                    const char *type;
                };
                
                FuncContext *ctx = new FuncContext{realloc_buffer, "test_type"};
                
                // Create a function that uses sqlite3_result_pointer
                auto func = [](sqlite3_context *sqlite_ctx, int argc, sqlite3_value **argv) {
                    FuncContext *fctx = (FuncContext *)sqlite3_user_data(sqlite_ctx);
                    if (fctx) {
                        // Use sqlite3_result_pointer
                        // Note: We need to cast to the proper function signature
                        // sqlite3_result_pointer is not in the provided list, but we'll
                        // use a similar approach with available functions
                        sqlite3_result_text(sqlite_ctx, (const char *)fctx->data, -1, SQLITE_TRANSIENT);
                    }
                };
                
                // Register the function
                sqlite3_create_function_v2(
                    db, "test_func", 0, SQLITE_UTF8, ctx,
                    func, nullptr, nullptr,
                    [](void *p) { delete (FuncContext *)p; }
                );
                
                // Try to declare a virtual table using the SQL
                sqlite3_declare_vtab(db, "CREATE VIRTUAL TABLE test USING fts4(content)");
                
                // Clean up string object
                char *finished_str = sqlite3_str_finish(str_obj);
                if (finished_str) {
                    sqlite3_free(finished_str);
                }
            }
            
            // Free the reallocated buffer
            sqlite3_free(realloc_buffer);
        }
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    // Close database
    if (db) {
        sqlite3_close_v2(db);
    }
    
    // Disable shared cache
    sqlite3_enable_shared_cache(0);
    
    assert_file_closed(&in_file);
	return 0;
}