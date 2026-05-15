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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 5
//<Quality> {"density":26,"unique_branches":{"sqlite3_soft_heap_limit64":[[29920,7,29920,10,0,0,4,0],[29924,28,29924,44,0,0,4,0],[29924,48,29924,52,0,0,4,0],[29932,7,29932,15,0,0,4,1],[13869,56,13869,59,9,0,4,1]]},"library_calls":["sqlite3_soft_heap_limit64","sqlite3_open","sqlite3_create_window_function","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_parameter_count","sqlite3_vtab_collation","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_value","sqlite3_str_finish","sqlite3_finalize","sqlite3_drop_modules","sqlite3_close"],"critical_calls":["sqlite3_soft_heap_limit64","sqlite3_open","sqlite3_create_window_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_vtab_collation","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_value","sqlite3_str_finish","sqlite3_finalize","sqlite3_drop_modules","sqlite3_close"],"visited":0}
/**/


// Window function callbacks
static void  id_000002_windowStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Check if any argument is too large
    for (int i = 0; i < argc; i++) {
        if (sqlite3_value_bytes16(argv[i]) > 1000) {
            sqlite3_result_error_toobig(ctx);
            return;
        }
    }
    // Simple aggregation: sum integers
    sqlite3_int64 *sum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (sum) {
        if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
            *sum += sqlite3_value_int64(argv[0]);
        }
    }
}

static void  id_000002_windowFinal(sqlite3_context *ctx) {
    sqlite3_int64 *sum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, 0);
    if (sum) {
        sqlite3_result_int64(ctx, *sum);
    } else {
        sqlite3_result_int64(ctx, 0);
    }
}

static void  id_000002_windowValue(sqlite3_context *ctx) {
    sqlite3_result_error16(ctx, u"Value not supported", -1);
}

static void  id_000002_windowInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Simple inverse: subtract integers
    sqlite3_int64 *sum = (sqlite3_int64*)sqlite3_aggregate_context(ctx, sizeof(sqlite3_int64));
    if (sum && argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        *sum -= sqlite3_value_int64(argv[0]);
    }
}

static void  id_000002_windowDestroy(void *p) {
    // Cleanup if needed
    sqlite3_free(p);
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<151) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    // Set soft heap limit
    sqlite3_int64 heap_limit = size > 8 ? *(sqlite3_int64*)data : 1000000;
    sqlite3_soft_heap_limit64(heap_limit);
    
    // Open database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create window function
    const char *func_name = "fuzz_window";
    int n_arg = 1;
    int e_text_rep = SQLITE_UTF8;
    void *p_app = nullptr;
    
    if (sqlite3_create_window_function(
        db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, p_app,
         id_000002_windowStep,  id_000002_windowFinal,  id_000002_windowValue,  id_000002_windowInverse,  id_000002_windowDestroy
    ) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a table
    const char *create_sql = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "CREATE VIRTUAL TABLE vtab USING fts5(data);";
    char *err_msg = nullptr;
    if (sqlite3_exec(db, create_sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare statement with window function
    sqlite3_stmt *stmt = nullptr;
    const char *select_sql = 
        "SELECT id, data, fuzz_window(id) OVER (ORDER BY id) FROM test";
    if (sqlite3_prepare_v2(db, fuzz_str_4, fuzz_int32_t_5, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Bind parameters (though none in this query)
    int param_count = sqlite3_bind_parameter_count(stmt);
    // Just for demonstration, we'll bind if there were parameters
    
    // Use virtual table collation (simulated context)
    // Note: sqlite3_vtab_collation requires sqlite3_index_info*
    // We'll create a minimal structure for demonstration
    struct sqlite3_index_info idx_info;
    // This is just to use the API, actual usage requires proper setup
    const char *collation = sqlite3_vtab_collation(&idx_info, fuzz_int32_t_6);
    
    // Insert data from input
    sqlite3_stmt *insert_stmt = nullptr;
    const char *insert_sql = "INSERT INTO test (data) VALUES (?)";
    if (sqlite3_prepare_v2(db, fuzz_str_7, fuzz_int32_t_8, &insert_stmt, nullptr) == SQLITE_OK) {
        // Use first part of data as blob
        size_t insert_size = size > 100 ? 100 : size;
        sqlite3_bind_blob(insert_stmt, fuzz_int32_t_9, data, insert_size, SQLITE_STATIC);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Execute window function query
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int col_count = sqlite3_data_count(stmt);
        for (int i = 0; i < col_count; i++) {
            if (sqlite3_column_type(stmt, fuzz_int32_t_10) == SQLITE_BLOB) {
                const void *blob = sqlite3_column_blob(stmt, fuzz_int32_t_11);
                int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_12);
                // Process blob data if needed
            }
        }
    }
    
    // Use sqlite3_str_appendf
    sqlite3_str *str = sqlite3_str_new(db);
    if (str) {
        sqlite3_str_appendf(str, "Fuzzed %zu bytes with heap limit %lld", 
                          size, heap_limit);
        char *str_result = sqlite3_str_value(str);
        // Could use str_result here
        sqlite3_str_finish(str);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    
    // Drop modules to clean up virtual table
    const char *keep[] = {nullptr};
    sqlite3_drop_modules(db, keep);
    
    sqlite3_close(db);
    
    return 0;
}