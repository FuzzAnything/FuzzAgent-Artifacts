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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 143, nr_unique_branch: 31
//<Quality> {"density":26,"unique_branches":{"sqlite3VdbeCheckMemInvariants":[[82003,9,82003,68,0,0,4,0]],"sqlite3VdbeNextOpcode":[[86223,7,86223,14,0,0,4,1],[86265,9,86265,16,0,0,4,1],[86284,9,86284,17,0,0,4,1],[86295,11,86295,36,0,0,4,0],[86295,11,86295,36,0,0,4,1],[86296,11,86296,33,0,0,4,0],[86296,11,86296,33,0,0,4,1],[86296,37,86296,42,0,0,4,1]],"columnName":[[90955,9,90955,18,0,0,4,0]],"sqlite3VdbeMemValidStrRep":[[82124,7,82124,26,0,0,4,0],[82126,9,82126,31,0,0,4,1]],"sqlite3VdbeExec":[[96975,11,96975,34,0,0,4,0],[96976,13,96976,33,0,0,4,0]],"sqlite3_stmt_explain":[[91455,7,91455,15,0,0,4,1],[91458,7,91458,31,0,0,4,1],[91460,13,91460,20,0,0,4,1],[91460,24,91460,31,0,0,4,1],[91462,13,91462,55,0,0,4,1],[91464,13,91464,44,0,0,4,1],[91466,13,91466,24,0,0,4,1],[91475,7,91475,17,0,0,4,0]],"sqlite3VdbeMemSetPointer":[[82907,20,82907,26,0,0,4,0],[82911,16,82911,27,0,0,4,0]],"sqlite3VdbeList":[[86351,24,86351,37,0,0,4,1],[86351,41,86351,75,0,0,4,1],[86372,7,86372,20,0,0,4,1],[86394,11,86394,24,0,0,4,0]],"sqlite3_bind_pointer":[[91231,7,91231,20,0,0,4,0]],"sqlite3_get_clientdata":[[181345,22,181345,23,0,0,4,0],[181346,9,181346,35,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_close","sqlite3_shutdown","sqlite3_set_clientdata","sqlite3_create_collation","sqlite3_close","sqlite3_shutdown","sqlite3_close","sqlite3_shutdown","sqlite3_close","sqlite3_shutdown","sqlite3_exec","sqlite3_close","sqlite3_shutdown","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_close","sqlite3_shutdown","sqlite3_get_clientdata","sqlite3_stmt_explain","sqlite3_step","sqlite3_column_origin_name16","sqlite3_column_bytes16","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_strglob","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_set_clientdata","sqlite3_create_collation","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_get_clientdata","sqlite3_stmt_explain","sqlite3_step","sqlite3_column_origin_name16","sqlite3_column_bytes16","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_step","sqlite3_strglob","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":1}
/**/


// Collation callback function for sqlite3_create_collation
static int  id_000030_customCollation(void* pArg, int n1, const void* v1, int n2, const void* v2) {
    // Simple binary comparison
    int len = n1 < n2 ? n1 : n2;
    int result = memcmp(v1, v2, len);
    if (result == 0) {
        result = n1 - n2;
    }
    return result;
}

// Callback for pointer binding cleanup
static void  id_000030_pointerCleanup(void* ptr) {
    // Nothing to do for this fuzzing example
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create an in-memory database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Set client data first (we'll get it back later)
    // Use a dummy key and value
    const char* key = "fuzz_key";
    void* value = (void*)0x12345678;
    sqlite3_set_clientdata(db, key, value, nullptr);
    
    // Create a collation using the custom function
    rc = sqlite3_create_collation(db, "CUSTOM_COLLATION", SQLITE_UTF8, nullptr,  id_000030_customCollation);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a simple table using the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_close(db);
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read a portion of input to create table name and column names
    // We'll use the first 256 bytes or less if input is smaller
    size_t read_size = size > 256 ? 256 : size;
    char* buffer = (char*)malloc(read_size);
    if (!buffer) {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    fread(buffer, 1, read_size, in_file);
    assert_file_closed(&in_file);;
    
    // Create a simple table with the input data
    char create_sql[512];
    snprintf(create_sql, sizeof(create_sql), 
             "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB)");
    
    rc = sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        free(buffer);
        sqlite3_close(db);
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Insert some data using the input buffer
    char insert_sql[] = "INSERT INTO test_table (data) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind the input data as a blob
        sqlite3_bind_blob(stmt, 1, buffer, read_size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Prepare a SELECT statement to use various APIs
    char select_sql[] = "SELECT * FROM test_table";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        free(buffer);
        sqlite3_close(db);
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get client data - API 1
    void* retrieved_data = sqlite3_get_clientdata(db, key);
    (void)retrieved_data; // Prevent unused variable warning
    
    // Use sqlite3_stmt_explain - API 5
    rc = sqlite3_stmt_explain(stmt, 2); // 2 = EXPLAIN QUERY PLAN
    // Note: sqlite3_stmt_explain modifies the statement to be an EXPLAIN
    
    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Get column origin name in UTF-16 - API 2
        const void* origin_name = sqlite3_column_origin_name16(stmt, 0);
        (void)origin_name;
        
        // Get column bytes in UTF-16 - API 7
        int bytes16 = sqlite3_column_bytes16(stmt, 0);
        (void)bytes16;
    }
    
    // Prepare another statement for binding pointer
    sqlite3_finalize(stmt);
    char select2_sql[] = "SELECT id FROM test_table WHERE id > ?";
    rc = sqlite3_prepare_v2(db, select2_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind a pointer - API 4
        int dummy_value = 42;
        sqlite3_bind_pointer(stmt, 1, &dummy_value, "integer",  id_000030_pointerCleanup);
        
        // Execute and step
        sqlite3_step(stmt);
    }
    
    // Use sqlite3_strglob - API 6
    // Test if a simple pattern matches a string
    const char* pattern = "test*";
    const char* test_string = "test_string";
    int glob_result = sqlite3_strglob(pattern, test_string);
    (void)glob_result;
    
    // Clean up
    sqlite3_finalize(stmt);
    free(buffer);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	return 0;
}