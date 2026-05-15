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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"sqlite3_interrupt":[[179430,7,179430,32,0,0,4,1]],"sqlite3_soft_heap_limit64":[[29932,7,29932,15,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_exec","sqlite3_prepare","sqlite3_bind_blob64","sqlite3_step","sqlite3_expired","sqlite3_create_function_v2","sqlite3_user_data","sqlite3_realloc64","sqlite3_result_blob64","sqlite3_result_null","sqlite3_user_data","sqlite3_realloc64","sqlite3_result_blob64","sqlite3_result_null","sqlite3_prepare","sqlite3_step","sqlite3_expired","sqlite3_finalize","sqlite3_finalize","sqlite3_create_function_v2","sqlite3_aggregate_count","sqlite3_result_int","sqlite3_aggregate_count","sqlite3_result_int","sqlite3_prepare","sqlite3_step","sqlite3_finalize","sqlite3_interrupt","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_soft_heap_limit64","sqlite3_exec","sqlite3_prepare","sqlite3_bind_blob64","sqlite3_step","sqlite3_expired","sqlite3_create_function_v2","sqlite3_result_null","sqlite3_result_blob64","sqlite3_realloc64","sqlite3_user_data","sqlite3_result_null","sqlite3_result_blob64","sqlite3_realloc64","sqlite3_user_data","sqlite3_prepare","sqlite3_step","sqlite3_expired","sqlite3_finalize","sqlite3_finalize","sqlite3_create_function_v2","sqlite3_result_int","sqlite3_aggregate_count","sqlite3_result_int","sqlite3_aggregate_count","sqlite3_prepare","sqlite3_step","sqlite3_finalize","sqlite3_interrupt","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<205) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize SQLite
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set soft heap limit
    sqlite3_int64 heap_limit = size > 0 ? static_cast<sqlite3_int64>(size * 2) : 1024;
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(heap_limit);

    // Create a table for testing
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK && size > 0) {
        // Prepare statement for inserting data
        sqlite3_stmt *stmt = nullptr;
        const char *insert_sql = "INSERT INTO test(data) VALUES(?);";
        const char *tail = nullptr;
        
        // Use sqlite3_prepare to create a statement
        rc = sqlite3_prepare(db, fuzz_str_2, -1, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt) {
            // Bind the input data as a blob
            rc = sqlite3_bind_blob64(stmt, fuzz_int32_t_3, data, size, SQLITE_STATIC);
            
            if (rc == SQLITE_OK) {
                // Execute the statement
                rc = sqlite3_step(stmt);
                
                // Check if statement is expired (should return false for valid statement)
                int expired = sqlite3_expired(stmt);
                
                // Create a custom function that uses sqlite3_result_blob64
                struct FuncContext {
                    const uint8_t *data;
                    size_t size;
                };
                
                FuncContext func_ctx = {data, size};
                
                // Define a scalar function that returns blob data
                rc = sqlite3_create_function_v2(
                    db,
                    fuzz_str_4,
                    fuzz_int32_t_5,  // number of arguments
                    fuzz_int32_t_6,
                    &func_ctx,
                    [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
                        FuncContext *fc = static_cast<FuncContext*>(sqlite3_user_data(ctx));
                        if (fc && fc->data && fc->size > 0) {
                            // Allocate memory using sqlite3_realloc64
                            void *blob_copy = sqlite3_realloc64(nullptr, fc->size);
                            if (blob_copy) {
                                memcpy(blob_copy, fc->data, fc->size);
                                // Use sqlite3_result_blob64 to return the blob
                                sqlite3_result_blob64(ctx, blob_copy, fc->size, sqlite3_free);
                            }
                        } else {
                            sqlite3_result_null(ctx);
                        }
                    },
                    nullptr,  // xStep
                    nullptr,  // xFinal
                    nullptr   // xDestroy
                );
                
                // Use the custom function
                if (rc == SQLITE_OK) {
                    sqlite3_stmt *select_stmt = nullptr;
                    const char *select_sql = "SELECT fuzz_blob();";
                    rc = sqlite3_prepare(db, fuzz_str_7, -1, &select_stmt, nullptr);
                    
                    if (rc == SQLITE_OK && select_stmt) {
                        // Execute to trigger the custom function
                        sqlite3_step(select_stmt);
                        
                        // Check if this statement is expired
                        expired = sqlite3_expired(select_stmt);
                        
                        sqlite3_finalize(select_stmt);
                    }
                }
                
                // Clean up the insert statement
                sqlite3_finalize(stmt);
            }
        }
        
        // Test sqlite3_aggregate_count with an aggregate function
        rc = sqlite3_create_function_v2(
            db,
            fuzz_str_8,
            fuzz_int32_t_9,
            fuzz_int32_t_10,
            nullptr,
            nullptr,  // xFunc
            [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
                // Step function - do nothing
            },
            [](sqlite3_context *ctx) {
                // Final function - use sqlite3_aggregate_count
                int count = sqlite3_aggregate_count(ctx);
                sqlite3_result_int(ctx, count);
            },
            nullptr
        );
        
        if (rc == SQLITE_OK) {
            sqlite3_stmt *agg_stmt = nullptr;
            const char *agg_sql = "SELECT test_agg(id) FROM test;";
            rc = sqlite3_prepare(db, fuzz_str_11, -1, &agg_stmt, nullptr);
            
            if (rc == SQLITE_OK && agg_stmt) {
                sqlite3_step(agg_stmt);
                sqlite3_finalize(agg_stmt);
            }
        }
        
        // Test sqlite3_vtab_nochange by creating a virtual table
        // (This is simplified - actual virtual table implementation would be more complex)
        struct TestVTab {
            sqlite3_vtab base;
            int nochange_flag;
        };
        
        // Interrupt the database operation
        sqlite3_interrupt(db);
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, size < 1024 ? size : 1024, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;

    // Remove temporary output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}