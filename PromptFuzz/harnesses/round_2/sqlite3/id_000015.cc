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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 7
//<Quality> {"density":21,"unique_branches":{"sqlite3CreateFunc":[[179511,5,179511,22,0,0,4,0],[179548,7,179548,8,0,0,4,0],[179548,12,179548,58,0,0,4,0],[179548,62,179548,75,0,0,4,0],[179549,9,179549,24,0,0,4,1],[14098,27,14098,28,31,0,4,0]],"sqlite3FindFunction":[[126292,21,126292,49,0,0,4,1]]},"library_calls":["sqlite3_libversion_number","sqlite3_hard_heap_limit64","sqlite3_sleep","sqlite3_open","sqlite3_errmsg","sqlite3_close","sqlite3_create_function16","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_null","sqlite3_step","sqlite3_column_int","sqlite3_column_int","sqlite3_column_int","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_libversion_number","sqlite3_hard_heap_limit64","sqlite3_sleep","sqlite3_open","sqlite3_create_function16","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_column_int","sqlite3_column_int","sqlite3_column_int","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_close"],"visited":0}
/**/


// Define sqlite3_create_function16 since it's mentioned but not in the provided list
extern "C" int sqlite3_create_function16(
    sqlite3 *db,
    const void *zFunctionName,
    int nArg,
    int eTextRep,
    void *pApp,
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void (*xStep)(sqlite3_context*, int, sqlite3_value**),
    void (*xFinal)(sqlite3_context*)
);

// Custom function implementation
static void  id_000015_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        // Use sqlite3_value_encoding API
        int encoding = sqlite3_value_encoding(argv[0]);
        sqlite3_result_int(context, encoding);
    } else {
        sqlite3_result_null(context);
    }
}

// Another custom function for step/finalize (aggregate function)
static void  id_000015_custom_step(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Do nothing in step
}

static void  id_000015_custom_final(sqlite3_context* context) {
    sqlite3_result_int(context, 42); // Return a fixed value
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE* for writing output data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used to avoid warnings
    (void)out_fd; // Mark as used to avoid warnings
    
    // 1. Call sqlite3_libversion_number()
    int version = sqlite3_libversion_number();
    fprintf(out_file, "SQLite version: %d\n", version);
    
    // 2. Set hard heap limit
    sqlite3_int64 heap_limit = 1024 * 1024; // 1 MB
    if (size > 0) {
        // Use first byte to determine heap limit (within reasonable bounds)
        heap_limit = (data[0] % 10 + 1) * 1024 * 1024; // 1-10 MB
    }
    sqlite3_int64 old_limit = sqlite3_hard_heap_limit64(heap_limit);
    fprintf(out_file, "Heap limit set to: %lld, old was: %lld\n", 
            (long long)heap_limit, (long long)old_limit);
    
    // 3. Sleep for a short time based on input size
    int sleep_ms = (int)(size % 100); // Sleep 0-99 ms
    sqlite3_sleep(sleep_ms);
    fprintf(out_file, "Slept for: %d ms\n", sleep_ms);
    
    // 4. Open an in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(out_file, "Failed to open database: %s\n", sqlite3_errmsg(db));
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        if (db) sqlite3_close(db);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Register a custom function using sqlite3_create_function16
    // Convert function name to UTF-16
    const wchar_t* func_name = L"my_encoding";
    rc = sqlite3_create_function16(
        db,
        func_name,
        1,  // Number of arguments
        SQLITE_UTF16 | SQLITE_DETERMINISTIC,
        nullptr,  // Application data
         id_000015_custom_function,  // xFunc
        nullptr,  // xStep (not used for scalar function)
        nullptr   // xFinal (not used for scalar function)
    );
    
    // Also register an aggregate function
    const wchar_t* agg_name = L"my_aggregate";
    rc = sqlite3_create_function16(
        db,
        agg_name,
        1,
        SQLITE_UTF16,
        nullptr,
        nullptr,  // No xFunc for aggregate
         id_000015_custom_step,
         id_000015_custom_final
    );
    
    fprintf(out_file, "Functions registered\n");
    
    // 6. Create a table and insert data from the input
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_data (id INTEGER PRIMARY KEY, content BLOB);";
    char* err_msg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(out_file, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    // Insert data from input
    if (size > 0) {
        sqlite3_stmt* stmt;
        const char* insert_sql = "INSERT INTO test_data (content) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_bind_blob(stmt, 1, data, (int)(size > 1000 ? 1000 : size), SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            fprintf(out_file, "Inserted %zu bytes into database\n", size > 1000 ? 1000 : size);
        }
    }
    
    // 7. Test the custom function
    sqlite3_stmt* test_stmt;
    const char* test_sql = "SELECT my_encoding(?), my_encoding(123), my_encoding('test');";
    rc = sqlite3_prepare_v2(db, test_sql, -1, &test_stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind some data from input
        if (size > 10) {
            sqlite3_bind_text(test_stmt, 1, (const char*)data, 
                             (int)(size > 100 ? 100 : size), SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(test_stmt, 1);
        }
        
        if (sqlite3_step(test_stmt) == SQLITE_ROW) {
            int encoding1 = sqlite3_column_int(test_stmt, 0);
            int encoding2 = sqlite3_column_int(test_stmt, 1);
            int encoding3 = sqlite3_column_int(test_stmt, 2);
            fprintf(out_file, "Encodings: %d, %d, %d\n", encoding1, encoding2, encoding3);
        }
        sqlite3_finalize(test_stmt);
    }
    
    // 8. Test the aggregate function
    const char* agg_sql = "SELECT my_aggregate(id) FROM test_data;";
    rc = sqlite3_exec(db, agg_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        fprintf(out_file, "Aggregate error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    // 9. Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file to avoid accumulating files during fuzzing
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}