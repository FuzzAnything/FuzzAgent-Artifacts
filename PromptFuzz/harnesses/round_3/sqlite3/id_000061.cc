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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 364, nr_unique_branch: 70
//<Quality> {"density":13,"unique_branches":{"sqlite3PagerMovepage":[[63579,7,63579,23,0,0,4,1],[63602,7,63602,36,0,0,4,1],[63619,7,63619,35,0,0,4,1],[63640,9,63640,25,0,0,4,1],[63657,7,63657,23,0,0,4,1]],"sqlite3VdbeMemCopy":[[83049,9,83049,37,0,0,4,0]],"getOverflowPage":[[74703,7,74703,22,0,0,4,0],[74708,42,74708,67,0,0,4,1],[74712,9,74712,36,0,0,4,0],[74712,9,74712,36,0,0,4,1],[74714,11,74714,24,0,0,4,0],[74714,28,74714,51,0,0,4,0],[74714,28,74714,51,0,0,4,1],[74714,55,74714,65,0,0,4,0],[74723,7,74723,20,0,0,4,1],[74737,11,74737,26,0,0,4,0],[69381,34,69381,71,2,0,4,1]],"pcache1Rekey":[[55612,10,55612,22,0,0,4,0]],"syncJournal":[[60783,11,60783,17,0,0,4,0],[60783,21,60783,54,0,0,4,0],[60786,13,60786,26,0,0,4,1]],"sqlite3_value_blob":[[23623,26,23623,47,3,0,4,0]],"sqlite3PcacheDrop":[[54123,7,54123,27,0,0,4,0]],"sqlite3VdbeExec":[[93965,13,93965,35,0,0,4,0],[93966,9,93966,39,0,0,4,1],[99049,7,99049,9,0,0,4,0],[100992,7,100992,33,0,0,4,1],[23623,26,23623,47,671,0,4,0]],"replaceFunc":[[128820,7,128820,21,0,0,4,0]],"sqlite3_errmsg16":[[180295,9,180295,13,0,0,4,1]],"fillInCell":[[76825,11,76825,26,0,0,4,0],[76829,43,76829,70,0,0,4,1],[76845,11,76845,26,0,0,4,0],[76845,30,76845,43,0,0,4,0],[76846,21,76846,31,0,0,4,0],[76846,21,76846,31,0,0,4,1],[76848,13,76848,15,0,0,4,1],[69381,34,69381,71,10,0,4,1]],"pager_write":[[62504,11,62504,45,0,0,4,1]],"jsonAppendValue":[[203226,5,203226,12,0,0,4,0],[203227,11,203227,21,0,0,4,0]],"sqlite3RunVacuum":[[153484,7,153484,20,0,0,4,0]],"substrFunc":[[127777,7,127777,26,0,0,4,0],[127780,9,127780,13,0,0,4,1],[127829,7,127829,26,0,0,4,1],[127840,9,127840,18,0,0,4,0],[127842,11,127842,15,0,0,4,1]],"sqlite3BtreeDelete":[[79505,9,79506,78,0,0,4,1],[79507,9,79507,24,0,0,4,1],[79622,9,79622,20,0,0,4,0],[79627,11,79627,33,0,0,4,1]],"modifyPagePointer":[[73578,7,73578,30,0,0,4,0],[73580,9,73580,38,0,0,4,1]],"balance_nonroot":[[77951,9,77951,26,0,0,4,0],[77953,11,77953,13,0,0,4,1]],"jsonResult":[[203243,7,203243,17,0,0,4,1]],"clearDatabasePage":[[79859,13,79859,56,0,0,4,1]],"sqlite3PcacheFetchStress":[[54006,9,54006,12,0,0,4,1],[54006,30,54006,58,0,0,4,0],[54010,9,54010,13,0,0,4,0],[54011,35,54011,38,0,0,4,0],[54011,42,54011,51,0,0,4,0],[54011,42,54011,51,0,0,4,1]],"sqlite3ValueBytes":[[83963,9,83963,28,0,0,4,0]],"sqlite3VdbeMemGrow":[[82202,40,82202,62,0,0,4,1]],"pager_write_pagelist":[[60877,9,60877,29,0,0,4,1]],"relocatePage":[[73682,9,73682,20,0,0,4,0],[73684,11,73684,24,0,0,4,1]],"pagerStress":[[61067,9,61067,35,0,0,4,0]],"sqlite3PcacheMove":[[54222,7,54222,29,0,0,4,1]]},"library_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_close","sqlite3_str_appendall","sqlite3_str_appendall","sqlite3_trace","sqlite3_str_new","sqlite3_malloc","sqlite3_malloc","sqlite3_complete16","sqlite3_free","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_column_text","sqlite3_stmt_status","sqlite3_finalize","sqlite3_free","sqlite3_errmsg16","sqlite3_str_appendall","sqlite3_str_finish","sqlite3_free","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_str_appendall","sqlite3_str_appendall","sqlite3_trace","sqlite3_str_new","sqlite3_malloc","sqlite3_malloc","sqlite3_complete16","sqlite3_free","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_column_text","sqlite3_stmt_status","sqlite3_finalize","sqlite3_free","sqlite3_errmsg16","sqlite3_str_appendall","sqlite3_str_finish","sqlite3_free","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<102) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // 1. Check if SQLite is compiled with thread safety
    int threadsafe = sqlite3_threadsafe();
    
    // 2. Create a temporary database file
    const char* db_filename = "fuzz_temp.db";
    sqlite3* db = nullptr;
    
    // 3. Open database connection
    if (sqlite3_open(db_filename, &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // 4. Set up trace callback to monitor SQL execution
    struct TraceData {
        FILE* out_file;
        sqlite3_str* str;
    };
    TraceData trace_data = {nullptr, nullptr};
    FILE* out_file = fopen("output_file", "wb");
    trace_data.out_file = out_file;
    
    auto trace_callback = [](void* user_data, const char* sql) {
        TraceData* td = static_cast<TraceData*>(user_data);
        if (td && td->str) {
            sqlite3_str_appendall(td->str, sql);
        }
    };
    
    sqlite3_trace(db, trace_callback, &trace_data);
    
    // 5. Create a string builder for error messages
    trace_data.str = sqlite3_str_new(db);
    
    // 6. Convert input data to null-terminated string for SQL
    char* sql_input = nullptr;
    if (size > 0) {
        sql_input = static_cast<char*>(sqlite3_malloc(size + 1));
        if (sql_input) {
            memcpy(sql_input, data, size);
            sql_input[size] = '\0';
            
            // 7. Check if input is a complete SQL statement (UTF-16 version)
            // Convert to UTF-16 for checking
            int utf16_len = (size + 1) * 2;
            void* sql16 = sqlite3_malloc(utf16_len);
            if (sql16) {
                // Simple byte-to-UTF16 conversion (simplified for fuzzing)
                memset(sql16, 0, utf16_len);
                memcpy(sql16, data, size > utf16_len/2 ? utf16_len/2 : size);
                int is_complete = sqlite3_complete16(sql16);
                sqlite3_free(sql16);
            }
        }
    }
    
    // 8. Execute SQL from input
    char* err_msg = nullptr;
    int exec_result = SQLITE_OK;
    
    if (sql_input) {
        // First, create a simple table
        const char* create_table_sql = 
            "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);";
        exec_result = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, &err_msg);
        
        if (exec_result == SQLITE_OK) {
            // Try to execute the input SQL
            exec_result = sqlite3_exec(db, sql_input, nullptr, nullptr, &err_msg);
        }
        
        // 9. Prepare a statement with the input
        sqlite3_stmt* stmt = nullptr;
        const char* tail = nullptr;
        
        if (sqlite3_prepare_v2(db, sql_input, -1, &stmt, &tail) == SQLITE_OK && stmt) {
            // 10. Bind a zero blob using input size
            if (size > 0) {
                sqlite3_uint64 blob_size = static_cast<sqlite3_uint64>(size);
                sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_2, blob_size);
            }
            
            // 11. Execute the prepared statement
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // 12. Get column text if available
                const unsigned char* col_text = sqlite3_column_text(stmt, fuzz_int32_t_3);
                if (col_text && out_file) {
                    fwrite(col_text, 1, strlen(reinterpret_cast<const char*>(col_text)), out_file);
                }
            }
            
            // 13. Get statement status
            int vm_step_count = sqlite3_stmt_status(stmt, fuzz_int32_t_4, fuzz_int32_t_5);
            
            // 14. Finalize the statement
            sqlite3_finalize(stmt);
        }
        
        sqlite3_free(sql_input);
    }
    
    // 15. Get error message in UTF-16 format if there was an error
    if (exec_result != SQLITE_OK) {
        const void* err16 = sqlite3_errmsg16(db);
        if (trace_data.str && err16) {
            // Append error message to string builder
            sqlite3_str_appendall(trace_data.str, fuzz_str_6);
        }
    }
    
    // 16. Finish the string builder and write to file
    if (trace_data.str) {
        char* finished_str = sqlite3_str_finish(trace_data.str);
        if (finished_str && out_file) {
            fwrite(finished_str, 1, strlen(finished_str), out_file);
            sqlite3_free(finished_str);
        }
    }
    
    // 17. Clean up
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // 18. Close database
    if (db) {
        sqlite3_close(db);
    }
    
    // 19. Remove temporary database file
    remove(db_filename);
    
    assert_file_closed(&out_file);
	return 0;
}