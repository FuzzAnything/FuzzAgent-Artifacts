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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_close","sqlite3_exec","sqlite3_close","sqlite3_create_function","sqlite3_create_function","sqlite3_close","sqlite3_get_table","sqlite3_extended_errcode","sqlite3_keyword_count","sqlite3_exec","sqlite3_free_table","sqlite3_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_exec","sqlite3_create_function","sqlite3_create_function","sqlite3_get_table","sqlite3_extended_errcode","sqlite3_keyword_count","sqlite3_exec","sqlite3_free_table","sqlite3_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to achieve the described event:

*/


extern "C" {
}

// Custom user data structure to hold our context
struct FuzzUserData {
    const uint8_t* data;
    size_t size;
    size_t offset;
    sqlite3* db;
    sqlite3_stmt* stmt;
};

// Callback function for sqlite3_get_table
static int  id_000026_table_callback(void* user_data, int argc, char** argv, char** col_names) {
    FuzzUserData* fud = static_cast<FuzzUserData*>(user_data);
    
    // Use sqlite3_keyword_count() - called at least once
    int keyword_count = sqlite3_keyword_count();
    
    // Process each column
    for (int i = 0; i < argc && fud->offset < fud->size; i++) {
        if (argv[i]) {
            // Consume some input data
            size_t len = strlen(argv[i]);
            if (fud->offset < fud->size) {
                fud->offset += (len < (fud->size - fud->offset)) ? len : (fud->size - fud->offset);
            }
        }
    }
    
    return 0;
}

// Custom function to demonstrate sqlite3_result_error_code
static void  id_000026_test_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Use sqlite3_result_error_code
    sqlite3_result_error_code(context, SQLITE_ERROR);
}

// Custom function to demonstrate sqlite3_vtab_in_first
static void  id_000026_vtab_test_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        sqlite3_value* first_val = nullptr;
        // Try to get first value from vtab IN constraint
        int rc = sqlite3_vtab_in_first(argv[0], &first_val);
        if (rc == SQLITE_OK && first_val) {
            // Process the value
            const unsigned char* text = sqlite3_value_text(first_val);
            if (text) {
                // Do something with the text
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize variables
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char** pazResult = nullptr;
    int pnRow = 0;
    int pnColumn = 0;
    char* pzErrmsg = nullptr;
    int rc;
    
    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    // Create a test table
    const char* create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Insert some test data
    const char* insert_sql = 
        "INSERT INTO test_table (name, value) VALUES ('test1', x'010203'), ('test2', x'040506');";
    rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a test function that uses sqlite3_result_error_code
    rc = sqlite3_create_function(
        db,
        "test_func",
        1,
        SQLITE_UTF8,
        nullptr,
         id_000026_test_function,
        nullptr,
        nullptr
    );
    
    // Create another function that uses sqlite3_vtab_in_first
    rc = sqlite3_create_function(
        db,
        "vtab_test_func",
        1,
        SQLITE_UTF8,
        nullptr,
         id_000026_vtab_test_function,
        nullptr,
        nullptr
    );
    
    // Prepare user data
    FuzzUserData user_data;
    user_data.data = data;
    user_data.size = size;
    user_data.offset = 0;
    user_data.db = db;
    user_data.stmt = nullptr;
    
    // Convert input data to a null-terminated string for SQL query
    // Ensure we don't overflow and keep the string null-terminated
    size_t query_len = (size < 1024) ? size : 1023;
    char* query = static_cast<char*>(malloc(query_len + 1));
    if (!query) {
        sqlite3_close(db);
        return 0;
    }
    
    memcpy(query, data, query_len);
    query[query_len] = '\0';
    
    // Use sqlite3_get_table with the input data as SQL query
    // Note: In real usage, this would be dangerous with untrusted input!
    rc = sqlite3_get_table(db, query, &pazResult, &pnRow, &pnColumn, &pzErrmsg);
    
    // Consume the input data by advancing offset
    user_data.offset += query_len;
    
    // Use sqlite3_extended_errcode to get extended error code
    int extended_err = sqlite3_extended_errcode(db);
    
    // Use sqlite3_keyword_count (also called in callback)
    int keyword_count = sqlite3_keyword_count();
    
    // Try to use the custom functions
    const char* func_sql = "SELECT test_func(1), vtab_test_func(1);";
    rc = sqlite3_exec(db, func_sql, nullptr, nullptr, nullptr);
    
    // Cleanup sqlite3_get_table results
    if (pazResult) {
        sqlite3_free_table(pazResult);
    }
    
    if (pzErrmsg) {
        sqlite3_free(pzErrmsg);
    }
    
    // Cleanup
    free(query);
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}