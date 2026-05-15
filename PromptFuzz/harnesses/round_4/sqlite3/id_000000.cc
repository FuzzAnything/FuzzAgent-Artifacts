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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 33
//<Quality> {"density":22,"unique_branches":{"sqlite3CreateFunc":[[179511,5,179511,22,0,0,4,0],[179530,5,179530,21,0,0,4,1]],"sqlite3ExprCodeFactorable":[[113291,7,113291,28,0,0,4,0],[113291,32,113291,67,0,0,4,0]],"sqlite3VdbeDeleteAuxData":[[87638,35,87638,69,0,0,4,0]],"cellSizePtrTableLeaf":[[71277,7,71277,22,0,0,4,1],[71278,7,71278,22,0,0,4,1],[71279,7,71279,22,0,0,4,1],[71280,7,71280,22,0,0,4,1],[71281,7,71281,22,0,0,4,1],[71282,7,71282,22,0,0,4,1]],"matchQuality":[[126158,7,126158,48,0,0,4,1],[126160,13,126160,40,0,0,4,1]],"sqlite3VdbeMemTranslate":[[34047,7,34047,14,9,0,4,0],[34049,12,34049,22,9,0,4,0],[34049,12,34049,22,9,0,4,1],[34049,26,34049,45,9,0,4,0],[34049,26,34049,45,9,0,4,1],[34052,9,34052,15,9,0,4,0],[34052,9,34052,15,9,0,4,1],[34053,12,34053,34,9,0,4,0],[34053,12,34053,34,9,0,4,1],[34054,12,34054,34,9,0,4,1],[33995,7,33995,16,10,0,4,1]],"btreeParseCellPtr":[[71043,11,71043,18,0,0,4,1],[71045,13,71045,20,0,0,4,1],[71047,15,71047,22,0,0,4,1],[71049,17,71049,24,0,0,4,1],[71051,19,71051,26,0,0,4,1],[71053,21,71053,28,0,0,4,1]],"sqlite3Insert":[[132533,15,132533,29,0,0,4,0],[132834,16,132834,30,0,0,4,1],[132835,11,132835,26,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_create_function","sqlite3_close","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_vtab_in_next","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_extended_errcode","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_trace_v2","sqlite3_create_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_vtab_in_next","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_extended_errcode","sqlite3_close"],"visited":1}
/**/


// Trace callback function for sqlite3_trace_v2
static int  id_000000_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    // Simple trace callback that does nothing
    return 0;
}

// Custom SQL function implementation
static void  id_000000_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        // Use sqlite3_value_text16 to get text value
        const void* text = sqlite3_value_text16(argv[0]);
        if (text) {
            // Set auxiliary data
            char* aux_data = (char*)malloc(10);
            if (aux_data) {
                // Use sqlite3_randomness for the auxiliary data
                sqlite3_randomness(10, aux_data);
                sqlite3_set_auxdata(context, 0, aux_data, free);
            }
            
            // Return the text value
            sqlite3_result_text16(context, text, -1, SQLITE_TRANSIENT);
            return;
        }
    }
    sqlite3_result_null(context);
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<264) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_13, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Create a memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(fuzz_str_1, &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    // Set up trace using sqlite3_trace_v2
    sqlite3_trace_v2(db, fuzz_uint32_t_2,  id_000000_trace_callback, nullptr);
    
    // Create a custom function
    rc = sqlite3_create_function(
        db,
        fuzz_str_3,
        fuzz_int32_t_4,  // nArg
        fuzz_int32_t_5,  // eTextRep
        nullptr,  // pApp
         id_000000_custom_function,  // xFunc
        nullptr,  // xStep
        nullptr   // xFinal
    );
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, fuzz_str_6, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement to insert data
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test_table (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_7, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind the input data as a blob
        rc = sqlite3_bind_blob(stmt, fuzz_int32_t_8, data, 
                               static_cast<int>(size > 1024 ? 1024 : size), 
                               SQLITE_STATIC);
        
        if (rc == SQLITE_OK) {
            // Execute the statement
            sqlite3_step(stmt);
        }
        
        // Reset and finalize the statement
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Prepare another statement to use our custom function
    sqlite3_stmt* query_stmt = nullptr;
    const char* query_sql = "SELECT custom_func(data) FROM test_table;";
    rc = sqlite3_prepare_v2(db, fuzz_str_9, -1, &query_stmt, nullptr);
    
    if (rc == SQLITE_OK && query_stmt) {
        // Execute the query
        while (sqlite3_step(query_stmt) == SQLITE_ROW) {
            // Get the value from the result
            sqlite3_value* value = sqlite3_column_value(query_stmt, fuzz_int32_t_10);
            if (value) {
                // Try to use sqlite3_vtab_in_next (though it's typically for virtual tables)
                sqlite3_value* out_val = nullptr;
                sqlite3_vtab_in_next(value, &out_val);
                // Note: This will typically fail as value is not from a virtual table IN constraint
            }
        }
        
        sqlite3_finalize(query_stmt);
    }
    
    // Prepare a statement with bind_int64
    sqlite3_stmt* int_stmt = nullptr;
    const char* int_sql = "INSERT INTO test_table (id) VALUES (?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_11, -1, &int_stmt, nullptr);
    
    if (rc == SQLITE_OK && int_stmt) {
        // Use part of input data to create an int64 value
        sqlite3_int64 int_value = 0;
        if (size >= sizeof(sqlite3_int64)) {
            memcpy(&int_value, data, sizeof(sqlite3_int64));
        } else if (size > 0) {
            int_value = static_cast<sqlite3_int64>(data[0]);
        }
        
        // Bind the int64 value
        sqlite3_bind_int64(int_stmt, fuzz_int32_t_12, fuzz_int64_t_13);
        
        // Execute
        sqlite3_step(int_stmt);
        sqlite3_finalize(int_stmt);
    }
    
    // Check extended error code
    int extended_err = sqlite3_extended_errcode(db);
    (void)extended_err; // Use the value to avoid unused variable warning
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}