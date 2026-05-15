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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 399, nr_unique_branch: 85
//<Quality> {"density":21,"unique_branches":{"btreeReleaseAllCursorPages":[[70434,14,70434,27,0,0,4,0]],"balance_deeper":[[78668,7,78668,20,0,0,4,0],[78675,7,78675,9,0,0,4,1],[69436,27,69436,44,2,0,4,1]],"cellSizePtrNoPayload":[[71245,10,71245,25,0,0,4,0],[71245,10,71245,25,0,0,4,1],[71245,29,71245,39,0,0,4,0]],"moveToRightmost":[[75332,10,75332,38,0,0,4,0],[75336,9,75336,11,0,0,4,1]],"insertCellFast":[[77069,7,77069,24,0,0,4,0]],"decodeFlags":[[71770,7,71770,42,0,0,4,1],[71797,9,71797,33,0,0,4,1],[71804,15,71804,52,0,0,4,0]],"copyNodeContent":[[77777,7,77777,24,0,0,4,0],[77782,25,77782,39,0,0,4,1],[77803,9,77803,22,0,0,4,0],[77804,9,77804,22,0,0,4,1],[69436,27,69436,44,5,0,4,1]],"sqlite3GetVarint":[[35663,7,35663,30,0,0,4,0]],"moveToChild":[[75120,7,75120,42,0,0,4,1],[75131,7,75131,20,0,0,4,0],[75132,8,75132,28,0,0,4,1],[75132,32,75132,68,0,0,4,1],[75137,7,75137,9,0,0,4,1]],"rebuildPage":[[77287,7,77287,24,0,0,4,1],[77290,30,77290,49,0,0,4,1],[77301,15,77301,45,0,0,4,1],[77310,9,77310,25,0,0,4,1],[77314,9,77314,16,0,0,4,0],[14155,25,14155,46,2,0,4,0],[14155,26,14155,29,2,0,4,0],[14823,33,14823,55,3,0,4,1]],"cellSizePtrTableLeaf":[[71275,7,71275,22,0,0,4,0],[71276,7,71276,22,0,0,4,1]],"balance_quick":[[77631,7,77631,22,0,0,4,1],[77641,7,77641,20,0,0,4,0],[77696,12,77696,29,0,0,4,0],[77696,12,77696,29,0,0,4,1],[77696,33,77696,44,0,0,4,0],[77698,12,77698,43,0,0,4,0],[77698,12,77698,43,0,0,4,1],[77698,47,77698,58,0,0,4,0],[77701,9,77701,22,0,0,4,0],[14156,25,14156,46,6,0,4,1],[14156,26,14156,29,6,0,4,1],[69436,27,69436,44,7,0,4,1]],"balance":[[78748,9,78748,28,0,0,4,0],[78748,9,78748,28,0,0,4,1],[78748,32,78748,76,0,0,4,0],[78748,32,78748,76,0,0,4,1],[78754,15,78754,39,0,0,4,0],[78754,15,78754,39,0,0,4,1],[78755,11,78755,27,0,0,4,0],[78755,11,78755,27,0,0,4,1],[78755,31,78755,73,0,0,4,0],[78764,13,78764,26,0,0,4,0],[78775,15,78775,57,0,0,4,1],[78785,11,78785,24,0,0,4,0],[78785,28,78785,44,0,0,4,1],[78788,11,78788,24,0,0,4,0],[78790,13,78790,30,0,0,4,0],[78791,13,78791,32,0,0,4,0],[78792,13,78792,43,0,0,4,0],[78793,13,78793,29,0,0,4,0],[78794,13,78794,33,0,0,4,0],[78858,11,78858,24,0,0,4,0],[78860,7,78860,12,0,0,4,1],[14156,25,14156,46,4,0,4,1],[14156,26,14156,29,4,0,4,1]],"insertCell":[[76974,7,76974,23,0,0,4,1],[76974,27,76974,44,0,0,4,1],[77004,9,77004,11,0,0,4,1],[77023,9,77023,40,0,0,4,0],[77023,9,77023,40,0,0,4,1],[77026,9,77026,31,0,0,4,1],[14156,25,14156,46,2,0,4,1],[14156,26,14156,29,2,0,4,1]],"anotherValidCursor":[[78714,34,78714,40,0,0,4,0],[78714,34,78714,40,0,0,4,1],[78715,9,78715,21,0,0,4,1]],"sqlite3BtreeInsert":[[79282,7,79282,23,0,0,4,0],[79293,9,79293,37,0,0,4,1]],"btreeParseCellPtr":[[71038,7,71038,17,0,0,4,0],[71041,9,71041,16,0,0,4,1]],"zeroPage":[[72017,18,72017,37,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_snprintf","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_int","sqlite3_value_dup","sqlite3_value_bytes16","sqlite3_snprintf","sqlite3_value_free","sqlite3_errcode","sqlite3_snprintf","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_snprintf","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_int","sqlite3_value_dup","sqlite3_value_bytes16","sqlite3_snprintf","sqlite3_value_free","sqlite3_errcode","sqlite3_snprintf","sqlite3_finalize","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite APIs:

*/


// Custom function to use sqlite3_result_zeroblob
static void  id_000009_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc == 1 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        int n = sqlite3_value_int(argv[0]);
        sqlite3_result_zeroblob(context, n);
    } else {
        sqlite3_result_null(context);
    }
}

// Rollback hook callback function
static void  id_000009_rollback_hook_callback(void* pArg) {
    // Nothing to do here, just for demonstration
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_value* value = nullptr;
    char* expanded_sql = nullptr;
    int rc;
    
    // Create input file from fuzzer data
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
    
    // Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set rollback hook
    void* hook_data = malloc(64);
    if (hook_data) {
        sqlite3_rollback_hook(db,  id_000009_rollback_hook_callback, hook_data);
    }
    
    // Register custom function
    rc = sqlite3_create_function(
        db,
        "custom_zeroblob",
        1,
        SQLITE_UTF8,
        nullptr,
         id_000009_custom_function,
        nullptr,
        nullptr
    );
    
    // Create a table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Read data from input file and insert into database
    uint8_t buffer[1024];
    while (!feof(in_file) && !ferror(in_file)) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        if (bytes_read > 0) {
            char insert_sql[2048];
            sqlite3_snprintf(
                sizeof(insert_sql),
                insert_sql,
                "INSERT INTO test (data) VALUES (?)"
            );
            
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
            if (rc == SQLITE_OK) {
                rc = sqlite3_bind_blob(stmt, 1, buffer, bytes_read, SQLITE_STATIC);
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt);
                }
                sqlite3_finalize(stmt);
                stmt = nullptr;
            }
        }
    }
    
    // Prepare a statement that uses the custom function
    const char* query_sql = 
        "SELECT id, custom_zeroblob(LENGTH(data)), sqlite3_value_bytes16(custom_zeroblob(LENGTH(data))) FROM test;";
    
    rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Get expanded SQL
        expanded_sql = sqlite3_expanded_sql(stmt);
        if (expanded_sql) {
            // Write expanded SQL to output file
            fwrite(expanded_sql, 1, strlen(expanded_sql), out_file);
            sqlite3_free(expanded_sql);
        }
        
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Use sqlite3_column_int to get the first column
            int id = sqlite3_column_int(stmt, 0);
            
            // Use sqlite3_value_bytes16 on the result of custom function
            // (In practice, we'd need to create a sqlite3_value from the column,
            // but we'll create one directly for demonstration)
            value = sqlite3_value_dup(nullptr);
            if (value) {
                // Get bytes16 length (will be 0 for zeroblob)
                int bytes16_length = sqlite3_value_bytes16(value);
                
                // Write results to output file
                char result[256];
                sqlite3_snprintf(
                    sizeof(result),
                    result,
                    "ID: %d, Bytes16: %d\n",
                    id,
                    bytes16_length
                );
                fwrite(result, 1, strlen(result), out_file);
                
                sqlite3_value_free(value);
            }
        }
        
        // Check error code
        int errcode = sqlite3_errcode(db);
        char err_msg[256];
        sqlite3_snprintf(
            sizeof(err_msg),
            err_msg,
            "Last error code: %d\n",
            errcode
        );
        fwrite(err_msg, 1, strlen(err_msg), out_file);
        
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    if (hook_data) {
        free(hook_data);
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
	return 0;
}