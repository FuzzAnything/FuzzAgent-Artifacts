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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"sqlite3FkClearTriggerCache":[[130878,9,130878,25,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_trace","sqlite3_collation_needed16","sqlite3_create_collation16","sqlite3_create_module","sqlite3_create_function","sqlite3_create_function","sqlite3_exec","sqlite3_db_filename","sqlite3_uri_key","sqlite3_db_name","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_expired","sqlite3_step","sqlite3_data_count","sqlite3_column_int","sqlite3_reset","sqlite3_finalize","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_free_table","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_trace","sqlite3_collation_needed16","sqlite3_create_collation16","sqlite3_create_module","sqlite3_create_function","sqlite3_create_function","sqlite3_exec","sqlite3_db_filename","sqlite3_uri_key","sqlite3_db_name","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_expired","sqlite3_step","sqlite3_data_count","sqlite3_column_int","sqlite3_reset","sqlite3_finalize","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_free_table","sqlite3_close"],"visited":0}
/**/


// Custom collation comparison function
extern "C" int custom_collation(void* pArg, int n1, const void* s1, int n2, const void* s2) {
    // Simple byte-wise comparison
    int len = n1 < n2 ? n1 : n2;
    int cmp = memcmp(s1, s2, len);
    if (cmp != 0) return cmp;
    return n1 - n2;
}

// Callback for sqlite3_exec
extern "C" int exec_callback(void* data, int argc, char** argv, char** colNames) {
    // Do nothing with the results for fuzzing
    return 0;
}

// Custom function implementation
extern "C" void custom_function(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Return a constant integer
    sqlite3_result_int(ctx, 42);
}

// Custom aggregate step function
extern "C" void aggregate_step(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Increment aggregate count
    void* agg = sqlite3_aggregate_context(ctx, sizeof(int));
    if (agg) {
        int* count = static_cast<int*>(agg);
        (*count)++;
    }
}

// Custom aggregate final function
extern "C" void aggregate_final(sqlite3_context* ctx) {
    void* agg = sqlite3_aggregate_context(ctx, 0);
    if (agg) {
        int* count = static_cast<int*>(agg);
        sqlite3_result_int(ctx, *count);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

// Collation needed callback
extern "C" void collation_needed_callback(void* pArg, sqlite3* db, int eTextRep, const void* zName) {
    // Create a simple collation when needed
    sqlite3_create_collation16(db, zName, eTextRep, nullptr, custom_collation);
}

// Trace callback
extern "C" void trace_callback(void* pArg, const char* sql) {
    // Do nothing with the SQL trace
}

// Dummy module for virtual table
static sqlite3_module  id_000047_dummy_module = {
    0,                          // iVersion
    nullptr,                    // xCreate
    nullptr,                    // xConnect
    nullptr,                    // xBestIndex
    nullptr,                    // xDisconnect
    nullptr,                    // xDestroy
    nullptr,                    // xOpen
    nullptr,                    // xClose
    nullptr,                    // xFilter
    nullptr,                    // xNext
    nullptr,                    // xEof
    nullptr,                    // xColumn
    nullptr,                    // xRowid
    nullptr,                    // xUpdate
    nullptr,                    // xBegin
    nullptr,                    // xSync
    nullptr,                    // xCommit
    nullptr,                    // xRollback
    nullptr,                    // xFindFunction
    nullptr,                    // xRename
    nullptr,                    // xSavepoint
    nullptr,                    // xRelease
    nullptr                     // xRollbackTo
};

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* errmsg = nullptr;
    char** pazResult = nullptr;
    int pnRow = 0, pnColumn = 0;
    
    // Open database
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Enable load extension
    sqlite3_enable_load_extension(db, 1);
    
    // Set up trace
    sqlite3_trace(db, trace_callback, nullptr);
    
    // Set up collation needed callback
    sqlite3_collation_needed16(db, nullptr, collation_needed_callback);
    
    // Create a collation
    const char16_t* collation_name = u"custom_collation";
    sqlite3_create_collation16(db, collation_name, SQLITE_UTF8, nullptr, custom_collation);
    
    // Create dummy module
    sqlite3_create_module(db, " id_000047_dummy_module", & id_000047_dummy_module, nullptr);
    
    // Create custom function
    sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, nullptr, 
                           custom_function, nullptr, nullptr);
    
    // Create aggregate function
    sqlite3_create_function(db, "custom_agg", 1, SQLITE_UTF8, nullptr, 
                           nullptr, aggregate_step, aggregate_final);
    
    // Use input data as SQL commands
    fseek(in_file, 0, SEEK_SET);
    char buffer[4096];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, in_file);
    buffer[bytes_read] = '\0';
    
    // Execute SQL from input
    rc = sqlite3_exec(db, buffer, exec_callback, nullptr, &errmsg);
    
    // Get database filename (for in-memory this returns nullptr)
    sqlite3_filename filename = sqlite3_db_filename(db, "main");
    
    // Try to get URI key (for in-memory db this may not work)
    if (filename) {
        const char* uri_key = sqlite3_uri_key(filename, 0);
    }
    
    // Get database name
    const char* db_name = sqlite3_db_name(db, 0);
    
    // Prepare a statement
    const char* sql = "SELECT 1, custom_func(1), custom_agg(1)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (stmt) {
        // Check if statement is readonly
        int is_readonly = sqlite3_stmt_readonly(stmt);
        
        // Check if statement expired
        int is_expired = sqlite3_expired(stmt);
        
        // Step through statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get data count
            int data_count = sqlite3_data_count(stmt);
            
            // Get column values
            for (int i = 0; i < data_count; i++) {
                int col_val = sqlite3_column_int(stmt, i);
                // Write to output file
                fwrite(&col_val, sizeof(col_val), 1, out_file);
            }
        }
        
        // Reset statement
        sqlite3_reset(stmt);
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Test aggregate context in a separate execution
    char* agg_errmsg = nullptr;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER)", nullptr, nullptr, &agg_errmsg);
    sqlite3_exec(db, "INSERT INTO test VALUES (1), (2), (3)", nullptr, nullptr, &agg_errmsg);
    
    sqlite3_prepare_v2(db, "SELECT custom_agg(id) FROM test", -1, &stmt, nullptr);
    if (stmt) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int agg_result = sqlite3_column_int(stmt, 0);
            fwrite(&agg_result, sizeof(agg_result), 1, out_file);
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    if (errmsg) sqlite3_free(errmsg);
    if (agg_errmsg) sqlite3_free(agg_errmsg);
    if (pazResult) sqlite3_free_table(pazResult);
    
    // Close database
    sqlite3_close(db);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}