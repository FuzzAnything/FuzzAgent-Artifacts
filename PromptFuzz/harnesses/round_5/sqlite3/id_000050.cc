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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"sqlite3_bind_pointer":[[91231,7,91231,20,0,0,4,0]],"sqlite3VdbeExec":[[14098,27,14098,28,448,0,4,0]]},"library_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_free_filename","sqlite3_exec","sqlite3_prepare","sqlite3_close_v2","sqlite3_free_filename","sqlite3_bind_text64","sqlite3_bind_pointer","sqlite3_step","sqlite3_reset","sqlite3_trace_v2","sqlite3_unlock_notify","sqlite3_overload_function","sqlite3_vtab_rhs_value","sqlite3_exec","sqlite3_error_offset","sqlite3_os_end","sqlite3_value_free","sqlite3_finalize","sqlite3_close_v2","sqlite3_free_filename"],"critical_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_exec","sqlite3_prepare","sqlite3_bind_text64","sqlite3_bind_pointer","sqlite3_step","sqlite3_reset","sqlite3_trace_v2","sqlite3_unlock_notify","sqlite3_overload_function","sqlite3_vtab_rhs_value","sqlite3_exec","sqlite3_error_offset","sqlite3_os_end","sqlite3_value_free","sqlite3_finalize","sqlite3_close_v2","sqlite3_free_filename"],"visited":0}
/**/


// Callback function for trace
static int  id_000050_traceCallback(unsigned int mask, void* context, void* p1, void* p2) {
    return 0;
}

// Callback function for unlock notify
static void  id_000050_unlockNotifyCallback(void** apArg, int nArg) {
    // Do nothing
}

// Callback function for pointer destructor
static void  id_000050_pointerDestructor(void* ptr) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize variables
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_filename filename = nullptr;
    sqlite3_index_info index_info = {};
    sqlite3_value* rhs_value = nullptr;
    
    // Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create filename using sqlite3_create_filename
    const char* database = "input_file";
    const char* journal = "input_file-journal";
    const char* wal = "input_file-wal";
    const char* params[] = {nullptr};
    
    filename = sqlite3_create_filename(database, journal, wal, 0, params);
    if (!filename) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Open database using the created filename
    int rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_free_filename(filename);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Create a simple table
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT)", nullptr, nullptr, nullptr);
    
    // Prepare a statement
    rc = sqlite3_prepare(db, "INSERT INTO test VALUES (?, ?)", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
        sqlite3_free_filename(filename);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Use sqlite3_bind_text64 with data from input
    if (size > 0) {
        // Convert data to string (limited to 1000 chars to avoid overflow)
        size_t text_len = size > 1000 ? 1000 : size;
        char* text_data = new char[text_len + 1];
        memcpy(text_data, data, text_len);
        text_data[text_len] = '\0';
        
        sqlite3_bind_text64(stmt, 2, text_data, text_len, 
                           [](void* ptr) { delete[] static_cast<char*>(ptr); }, 
                           SQLITE_UTF8);
    }
    
    // Use sqlite3_bind_pointer
    int dummy_int = 42;
    sqlite3_bind_pointer(stmt, 1, &dummy_int, "dummy_type",  id_000050_pointerDestructor);
    
    // Execute the statement
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    // Use sqlite3_trace_v2
    sqlite3_trace_v2(db, SQLITE_TRACE_STMT,  id_000050_traceCallback, nullptr);
    
    // Use sqlite3_unlock_notify
    sqlite3_unlock_notify(db,  id_000050_unlockNotifyCallback, nullptr);
    
    // Use sqlite3_overload_function
    sqlite3_overload_function(db, "test_function", 2);
    
    // Use sqlite3_vtab_rhs_value (simulating virtual table scenario)
    // This is a simplified example - in real usage this would be in a virtual table implementation
    sqlite3_vtab_rhs_value(&index_info, 0, &rhs_value);
    
    // Use sqlite3_error_offset
    // First create an error by trying to execute invalid SQL
    sqlite3_exec(db, "INVALID SQL STATEMENT", nullptr, nullptr, nullptr);
    int error_offset = sqlite3_error_offset(db);
    
    // Use sqlite3_os_end (at the very end)
    // Note: This shuts down SQLite's OS interface - use with caution
    // In a fuzzer, we typically don't call this as it affects subsequent tests
    // But we're including it as requested
    sqlite3_os_end();
    
    // Clean up
    if (rhs_value) {
        sqlite3_value_free(rhs_value);
    }
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close_v2(db);
    }
    if (filename) {
        sqlite3_free_filename(filename);
    }
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}