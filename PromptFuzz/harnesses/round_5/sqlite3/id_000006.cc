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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 14
//<Quality> {"density":18,"unique_branches":{"sqlite3ExprDeleteNN":[[109338,7,109338,23,0,0,4,1]],"sqlite3ColumnsFromExprList":[[143560,13,143560,26,0,0,4,0],[20400,30,20400,72,11,0,4,0]],"selectWindowRewriteExprCb":[[168052,5,168052,21,0,0,4,1],[168068,5,168068,19,0,0,4,0],[168070,11,168070,35,0,0,4,1],[168071,11,168071,18,0,0,4,1],[168080,11,168080,17,0,0,4,0],[168082,13,168082,17,0,0,4,0],[168082,21,168082,46,0,0,4,1],[168085,11,168085,18,0,0,4,0],[168094,27,168094,33,0,0,4,0],[168099,11,168099,35,0,0,4,1]],"sqlite3WindowRewrite":[[168302,26,168302,34,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_shutdown","sqlite3_compileoption_used","sqlite3_open","sqlite3_shutdown","sqlite3_hard_heap_limit64","sqlite3_exec","sqlite3_mprintf","sqlite3_prepare_v2","sqlite3_free","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_profile","sqlite3_create_window_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_int","sqlite3_finalize","sqlite3_db_filename","sqlite3_profile","sqlite3_close","sqlite3_value_free","sqlite3_value_free","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_compileoption_used","sqlite3_open","sqlite3_hard_heap_limit64","sqlite3_exec","sqlite3_mprintf","sqlite3_prepare_v2","sqlite3_free","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_profile","sqlite3_create_window_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_int","sqlite3_finalize","sqlite3_db_filename","sqlite3_profile","sqlite3_close","sqlite3_value_free","sqlite3_value_free","sqlite3_shutdown"],"visited":0}
/**/


// Window function callback implementations
static void  id_000006_window_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Get aggregate context
    int *sum = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
    if (sum && argc > 0) {
        *sum += sqlite3_value_int(argv[0]);
    }
}

static void  id_000006_window_final(sqlite3_context *ctx) {
    int *sum = (int*)sqlite3_aggregate_context(ctx, 0);
    if (sum) {
        sqlite3_result_int(ctx, *sum);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void  id_000006_window_value(sqlite3_context *ctx) {
    int *sum = (int*)sqlite3_aggregate_context(ctx, 0);
    if (sum) {
        sqlite3_result_int(ctx, *sum);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void  id_000006_window_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int *sum = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
    if (sum && argc > 0) {
        *sum -= sqlite3_value_int(argv[0]);
    }
}

static void  id_000006_window_destroy(void *pApp) {
    // Cleanup if needed
}

// Profile callback
static void  id_000006_profile_callback(void *pCtx, const char *sql, sqlite3_uint64 ns) {
    // Can be used for logging profiling information
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_value *value = nullptr;
    sqlite3_value *dup_value = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors (as per requirement 5)
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // 1. Check if a compile option was used
    int compile_option_used = sqlite3_compileoption_used("THREADSAFE");
    (void)compile_option_used;
    
    // 2. Open a database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Set hard heap limit
    sqlite3_int64 heap_limit = 1000000; // 1MB
    sqlite3_int64 old_limit = sqlite3_hard_heap_limit64(heap_limit);
    (void)old_limit;
    
    // 4. Create a value and duplicate it
    // First create a simple table and insert data
    const char *create_table = "CREATE TABLE test (id INTEGER, data TEXT);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    
    // Insert some data using fuzzer input
    char *insert_sql = sqlite3_mprintf("INSERT INTO test VALUES (1, ?)");
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    sqlite3_free(insert_sql);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind the fuzzer data as text (use first part of data)
        int data_len = size > 100 ? 100 : (int)size;
        sqlite3_bind_text(stmt, 1, (const char*)data, data_len, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 5. Set profile callback
    void *old_profile = sqlite3_profile(db,  id_000006_profile_callback, nullptr);
    (void)old_profile;
    
    // 6. Create a window function
    rc = sqlite3_create_window_function(
        db,
        "my_sum",
        1,  // nArg
        SQLITE_UTF8,
        nullptr,  // pApp
         id_000006_window_step,
         id_000006_window_final,
         id_000006_window_value,
         id_000006_window_inverse,
         id_000006_window_destroy
    );
    
    // 7. Use the window function in a query
    const char *query = "SELECT id, my_sum(id) OVER (ORDER BY id) FROM test;";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            int sum_result = sqlite3_column_int(stmt, 1);
            
            // Create a value from the result
            // We'll create a value using sqlite3_result_int in a user-defined function context
            // For demonstration, we'll create a simple value context
            sqlite3_context *ctx = nullptr;  // In real usage, this would come from a UDF
            
            // This shows how sqlite3_result_int would be used
            // sqlite3_result_int(ctx, sum_result);
            
            (void)id;
            (void)sum_result;
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 8. Get database filename (for main database)
    const char *db_filename = sqlite3_db_filename(db, "main");
    (void)db_filename;
    
    // Clean up
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    if (db) {
        // Remove profile callback
        sqlite3_profile(db, nullptr, nullptr);
        sqlite3_close(db);
    }
    
    if (value) sqlite3_value_free(value);
    if (dup_value) sqlite3_value_free(dup_value);
    
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}