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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 2
//<Quality> {"density":33,"unique_branches":{"sqlite3MisuseError":[[14098,27,14098,28,1,0,4,1]],"sqlite3CreateFunc":[[179481,21,179481,30,0,0,4,0]]},"library_calls":["sqlite3_keyword_count","sqlite3_errstr","sqlite3_open","sqlite3_close","sqlite3_create_module","sqlite3_create_function_v2","sqlite3_collation_needed","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_bind_zeroblob64","sqlite3_clear_bindings","sqlite3_prepare_v2","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_column_int64","sqlite3_value_dup","sqlite3_value_int64","sqlite3_value_free","sqlite3_prepare_v2","sqlite3_bind_value","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_reset_auto_extension","sqlite3_close"],"critical_calls":["sqlite3_keyword_count","sqlite3_errstr","sqlite3_open","sqlite3_create_module","sqlite3_create_function_v2","sqlite3_collation_needed","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_bind_zeroblob64","sqlite3_clear_bindings","sqlite3_prepare_v2","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_column_int64","sqlite3_value_dup","sqlite3_value_int64","sqlite3_value_free","sqlite3_prepare_v2","sqlite3_bind_value","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_reset_auto_extension","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Custom function implementations for testing
static void  id_000026_test_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        sqlite3_result_int64(context, sqlite3_value_int64(argv[0]) * 2);
    } else {
        sqlite3_result_error_nomem(context);
    }
}

static void  id_000026_test_agg_step(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        sqlite3_int64* sum = (sqlite3_int64*)sqlite3_aggregate_context(context, sizeof(sqlite3_int64));
        if (sum) {
            *sum += sqlite3_value_int64(argv[0]);
        }
    }
}

static void  id_000026_test_agg_final(sqlite3_context* context) {
    sqlite3_int64* sum = (sqlite3_int64*)sqlite3_aggregate_context(context, sizeof(sqlite3_int64));
    if (sum) {
        sqlite3_result_int64(context, *sum);
    } else {
        sqlite3_result_int64(context, 0);
    }
}

// Custom module for testing
static int  id_000026_test_create(sqlite3* db, void* pAux, int argc, const char* const* argv,
                      sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int  id_000026_test_connect(sqlite3* db, void* pAux, int argc, const char* const* argv,
                       sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int  id_000026_test_best_index(sqlite3_vtab* pVtab, sqlite3_index_info* pIdxInfo) {
    return SQLITE_OK;
}

static int  id_000026_test_disconnect(sqlite3_vtab* pVtab) {
    return SQLITE_OK;
}

static int  id_000026_test_destroy(sqlite3_vtab* pVtab) {
    return SQLITE_OK;
}

static sqlite3_module  id_000026_test_module = {
    0,                          // iVersion
     id_000026_test_create,               // xCreate
     id_000026_test_connect,              // xConnect
     id_000026_test_best_index,           // xBestIndex
     id_000026_test_disconnect,           // xDisconnect
     id_000026_test_destroy,              // xDestroy
    0, 0, 0, 0, 0, 0, 0, 0     // Other methods (simplified)
};

// Collation needed callback
static void  id_000026_collation_needed_callback(void* pCtx, sqlite3* db, int eTextRep, const char* zName) {
    // Do nothing for testing
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open a file-like handle for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for input
    int in_fd = fuzz_fileno(in_file);
    
    // Test keyword count API
    int keyword_count = sqlite3_keyword_count();
    
    // Test error string API
    const char* err_str = sqlite3_errstr(SQLITE_NOMEM);
    
    // Open database (in-memory for safety)
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    
    // Create custom module
    rc = sqlite3_create_module(db, " id_000026_test_module", & id_000026_test_module, nullptr);
    
    // Create custom function
    rc = sqlite3_create_function_v2(
        db,
        " id_000026_test_func",
        1,
        SQLITE_UTF8,
        nullptr,
         id_000026_test_func,
         id_000026_test_agg_step,
         id_000026_test_agg_final,
        nullptr
    );
    
    // Set collation needed callback
    rc = sqlite3_collation_needed(db, nullptr,  id_000026_collation_needed_callback);
    
    // Create table and insert data using exec
    const char* create_sql = 
        "CREATE TABLE test_table(id INTEGER PRIMARY KEY, data BLOB, text TEXT, int_val INTEGER);";
    char* err_msg = nullptr;
    
    rc = sqlite3_exec(db, create_sql, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Prepare insert statement
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = 
        "INSERT INTO test_table(data, text, int_val) VALUES(?, ?, ?);";
    
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind blob (use part of input data)
        size_t blob_size = size > 100 ? 100 : size;
        rc = sqlite3_bind_blob(stmt, 1, data, blob_size, SQLITE_STATIC);
        
        // Bind text (use another part of input data)
        size_t text_size = size > 200 ? 100 : (size > 100 ? size - 100 : 0);
        if (text_size > 0) {
            rc = sqlite3_bind_text(stmt, 2, (const char*)data + 100, text_size, SQLITE_STATIC);
        } else {
            rc = sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
        }
        
        // Bind integer (use first 4 bytes as integer)
        int int_val = size > 0 ? data[0] : 0;
        rc = sqlite3_bind_int(stmt, 3, int_val);
        
        // Execute insert
        rc = sqlite3_step(stmt);
        
        // Reset statement for reuse
        sqlite3_reset(stmt);
        
        // Bind zeroblob
        rc = sqlite3_bind_zeroblob64(stmt, 1, 1024);
        
        // Clear bindings
        sqlite3_clear_bindings(stmt);
        
        // Prepare second statement for transfer test
        sqlite3_stmt* stmt2 = nullptr;
        const char* select_sql = "SELECT * FROM test_table;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
        
        if (stmt2) {
            // Transfer bindings (should transfer nothing since cleared, but tests API)
            rc = sqlite3_transfer_bindings(stmt, stmt2);
            
            sqlite3_finalize(stmt2);
        }
        
        // Finalize first statement
        sqlite3_finalize(stmt);
    }
    
    // Query data using prepared statement
    sqlite3_stmt* query_stmt = nullptr;
    const char* query_sql = 
        "SELECT data, text, int_val,  id_000026_test_func(int_val) FROM test_table;";
    
    rc = sqlite3_prepare_v2(db, query_sql, -1, &query_stmt, nullptr);
    if (rc == SQLITE_OK && query_stmt) {
        while (sqlite3_step(query_stmt) == SQLITE_ROW) {
            // Get column values
            sqlite3_value* blob_val = sqlite3_column_value(query_stmt, 0);
            sqlite3_int64 int64_val = sqlite3_column_int64(query_stmt, 2);
            
            // Duplicate and free a value
            sqlite3_value* dup_val = sqlite3_value_dup(blob_val);
            if (dup_val) {
                // Test value int64 extraction
                sqlite3_int64 test_val = sqlite3_value_int64(dup_val);
                sqlite3_value_free(dup_val);
            }
            
            // Test bind value API
            sqlite3_stmt* bind_stmt = nullptr;
            const char* bind_sql = "SELECT ?;";
            rc = sqlite3_prepare_v2(db, bind_sql, -1, &bind_stmt, nullptr);
            if (bind_stmt) {
                // Use the column value as binding
                rc = sqlite3_bind_value(bind_stmt, 1, blob_val);
                if (rc == SQLITE_OK) {
                    sqlite3_step(bind_stmt);
                }
                sqlite3_finalize(bind_stmt);
            }
        }
        sqlite3_finalize(query_stmt);
    }
    
    // Clean up
    if (out_file) assert_file_closed(&out_file);;
    if (in_file) assert_file_closed(&in_file);;
    
    // Reset auto extensions if any
    sqlite3_reset_auto_extension();
    
    // Close database
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}