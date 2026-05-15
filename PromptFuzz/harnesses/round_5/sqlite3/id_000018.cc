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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"sqlite3VdbeSorterInit":[[102833,35,102833,68,0,0,4,1]],"btreeBeginTrans":[[73475,13,73475,26,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_busy_timeout","sqlite3_wal_autocheckpoint","sqlite3_collation_needed","sqlite3_exec","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_busy_timeout","sqlite3_wal_autocheckpoint","sqlite3_collation_needed","sqlite3_exec","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"visited":0}
/**/

// Forward declaration of the collation needed callback
static void  id_000018_collation_needed_callback(void* pArg, sqlite3* db, int eTextRep, const char* zName);

// User-defined function that uses sqlite3_result_text16
static void  id_000018_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Simple function that returns the first argument as text16
    if (argc >= 1) {
        const void* text = sqlite3_value_text16(argv[0]);
        int bytes = sqlite3_value_bytes16(argv[0]);
        sqlite3_result_text16(context, text, bytes, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_null(context);
    }
}

// Collation function that compares two strings
static int  id_000018_custom_collation(void* pArg, int n1, const void* p1, int n2, const void* p2) {
    // Simple byte-by-byte comparison
    int min_len = n1 < n2 ? n1 : n2;
    int cmp = memcmp(p1, p2, min_len);
    if (cmp == 0) {
        return n1 - n2;
    }
    return cmp;
}

// Collation needed callback implementation
static void  id_000018_collation_needed_callback(void* pArg, sqlite3* db, int eTextRep, const char* zName) {
    // Register a custom collation when needed
    sqlite3_create_collation(db, zName, eTextRep, NULL,  id_000018_custom_collation);
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file with the input data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Use the input file name for database
    const char* db_filename = "input_file";
    
    // Write input data to the input file
    FILE* input_db = fopen(db_filename, "wb");
    if (input_db) {
        fwrite(data, 1, size, input_db);
        assert_file_closed(&input_db);;
    }
    
    sqlite3* db = nullptr;
    int rc = 0;
    
    // 1. Open database using sqlite3_open_v2
    rc = sqlite3_open_v2(db_filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove(db_filename);
        remove("output_file");
        assert_file_closed(&input_db);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Set busy timeout
    rc = sqlite3_busy_timeout(db, 1000); // 1 second timeout
    
    // 3. Set WAL autocheckpoint
    rc = sqlite3_wal_autocheckpoint(db, 1000); // Checkpoint after 1000 pages
    
    // 4. Register collation needed callback
    rc = sqlite3_collation_needed(db, nullptr,  id_000018_collation_needed_callback);
    
    // Create a simple table and insert some data to trigger collation usage
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Insert some data
    const char* insert_sql = "INSERT INTO test_table (name) VALUES ('test');";
    sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    
    // Create a custom function that uses sqlite3_result_text16
    rc = sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF16, nullptr, 
                                  id_000018_custom_function, nullptr, nullptr);
    
    // Execute a query that uses the custom function
    const char* query_sql = "SELECT custom_func(name) FROM test_table;";
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row if needed
        }
        sqlite3_finalize(stmt);
    }
    
    // Execute a query that might trigger collation needed callback
    const char* collation_query = "SELECT name FROM test_table ORDER BY name COLLATE unknown_collation;";
    sqlite3_exec(db, collation_query, nullptr, nullptr, nullptr);
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove(db_filename);
    remove("output_file");
    
    assert_file_closed(&input_db);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}