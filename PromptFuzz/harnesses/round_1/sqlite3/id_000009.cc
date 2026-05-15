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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 15
//<Quality> {"density":11,"unique_branches":{"sqlite3_snapshot_get":[[182483,7,182483,32,0,0,4,1],[182489,7,182489,24,0,0,4,1]],"sqlite3_extended_errcode":[[180325,7,180325,9,0,0,4,0],[180325,13,180325,44,0,0,4,1],[180328,7,180328,10,0,0,4,1],[180328,14,180328,30,0,0,4,1]],"sqlite3VdbeMemHandleBom":[[34292,7,34292,16,0,0,4,1],[34295,9,34295,17,0,0,4,0],[34295,21,34295,29,0,0,4,0],[34295,21,34295,29,0,0,4,1],[34298,9,34298,17,0,0,4,0],[34298,21,34298,29,0,0,4,0],[34298,21,34298,29,0,0,4,1],[34303,7,34303,10,0,0,4,0],[34305,9,34305,22,0,0,4,0]]},"library_calls":["sqlite3_os_init","sqlite3_open","sqlite3_close","sqlite3_db_readonly","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_snapshot_get","sqlite3_extended_errcode","sqlite3_realloc64","sqlite3_realloc64","sqlite3_free","sqlite3_reset_auto_extension","sqlite3_snapshot_free","sqlite3_close"],"critical_calls":["sqlite3_os_init","sqlite3_open","sqlite3_db_readonly","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_snapshot_get","sqlite3_extended_errcode","sqlite3_realloc64","sqlite3_realloc64","sqlite3_free","sqlite3_reset_auto_extension","sqlite3_snapshot_free","sqlite3_close"],"visited":0}
/**/


// Forward declaration for the custom SQL function
static void  id_000009_custom_func(sqlite3_context* context, int argc, sqlite3_value** argv);

// Structure to hold input data for the custom function
struct FuncData {
    const uint8_t* data;
    size_t size;
};

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
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
    sqlite3_snapshot* snapshot = nullptr;
    const char* zTail = nullptr;
    void* realloc_ptr = nullptr;
    int rc;
    
    // Use fmemopen to create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    if (fd_in == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get file descriptor for writing
    int fd_out = fuzz_fileno(out_file);
    if (fd_out == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 1: Initialize SQLite OS layer
    rc = sqlite3_os_init();
    
    // Step 2: Open a database (using input file name)
    rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK) {
        // Cleanup and return
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        if (db) sqlite3_close(db);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 3: Check if database is read-only
    int is_readonly = sqlite3_db_readonly(db, "main");
    
    // Step 4: Create custom function using input data
    FuncData func_data = {data, size};
    rc = sqlite3_create_function_v2(
        db,
        "custom_fuzz_func",
        1,                    // nArg - number of arguments
        SQLITE_UTF8,         // eTextRep - encoding
        &func_data,          // pApp - application data pointer
         id_000009_custom_func,         // xFunc - scalar function implementation
        nullptr,             // xStep - not an aggregate
        nullptr,             // xFinal - not an aggregate
        nullptr              // xDestroy - cleanup function
    );
    
    // Step 5: Prepare and execute a statement
    const char* sql = "SELECT custom_fuzz_func(1)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &zTail);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row if needed
        }
        sqlite3_finalize(stmt);
    }
    
    // Step 6: Try to get a snapshot (may fail if not in WAL mode)
    sqlite3_snapshot_get(db, "main", &snapshot);
    
    // Step 7: Check extended error code
    int extended_err = sqlite3_extended_errcode(db);
    
    // Step 8: Use realloc64 with input data
    if (size > 0) {
        // Allocate memory
        realloc_ptr = sqlite3_realloc64(nullptr, size);
        if (realloc_ptr) {
            // Copy input data
            memcpy(realloc_ptr, data, size);
            // Reallocate to double size
            void* new_ptr = sqlite3_realloc64(realloc_ptr, size * 2);
            if (new_ptr) {
                realloc_ptr = new_ptr;
            }
            // Free memory
            sqlite3_free(realloc_ptr);
        }
    }
    
    // Step 9: Reset auto extensions
    sqlite3_reset_auto_extension();
    
    // Write some data to output file using the file descriptors
    if (size > 0 && size < 1024) {
        fwrite(data, 1, size, out_file);
        fflush(out_file);
    }
    
    // Cleanup
    if (snapshot) {
        sqlite3_snapshot_free(snapshot);
    }
    
    // Close database
    if (db) {
        sqlite3_close(db);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}

// Implementation of custom function
static void  id_000009_custom_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        // Get application data
        FuncData* func_data = static_cast<FuncData*>(sqlite3_user_data(context));
        
        if (func_data && func_data->data && func_data->size > 0) {
            // Return input data as text16
            sqlite3_result_text16(context, 
                                 func_data->data, 
                                 func_data->size, 
                                 SQLITE_TRANSIENT);
        } else {
            // Return default text
            sqlite3_result_text16(context, L"fuzz", 4, SQLITE_TRANSIENT);
        }
    }
}