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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 180.5, nr_unique_branch: 64
//<Quality> {"density":19,"unique_branches":{"freePage2":[[76475,7,76475,15,0,0,4,1],[76558,13,76558,18,0,0,4,1],[76574,7,76574,15,0,0,4,0],[76574,19,76574,72,0,0,4,1],[76587,7,76587,12,0,0,4,1]],"sqlite3VdbeParameterIndex":[[91367,7,91367,11,0,0,4,1],[91367,15,91367,23,0,0,4,1]],"sqlite3VdbeExec":[[93257,10,93257,33,0,0,4,0],[93257,37,93257,53,0,0,4,0],[93260,9,93260,40,0,0,4,1],[101250,10,101250,33,0,0,4,0],[101250,37,101250,53,0,0,4,0],[101252,9,101252,40,0,0,4,1]],"defragmentPage":[[71389,44,71389,61,0,0,4,1]],"findNextHostParameter":[[92040,10,92040,17,0,0,4,0],[92040,10,92040,17,0,0,4,1],[92043,9,92043,31,0,0,4,0],[92043,9,92043,31,0,0,4,1]],"copyNodeContent":[[77782,25,77782,39,0,0,4,0]],"pcache1TruncateUnsafe":[[14109,23,14109,24,4,0,4,1]],"btreePageLookup":[[72089,7,72089,14,0,0,4,1]],"sqlite3BtreeTransferRow":[[79349,20,79349,46,0,0,4,1],[79364,9,79364,33,0,0,4,1],[79369,9,79369,17,0,0,4,1]],"sqlite3PagerDontWrite":[[62690,28,62690,52,0,0,4,1]],"clearCellOverflow":[[76644,20,76644,65,0,0,4,1],[76662,9,76662,14,0,0,4,1]],"unixTruncate":[[41696,9,41696,30,0,0,4,0]],"sqlite3VdbeExpandSql":[[92095,7,92095,22,0,0,4,0],[92096,12,92096,20,0,0,4,0],[92096,12,92096,20,0,0,4,1],[92098,14,92098,32,0,0,4,0],[92098,36,92098,44,0,0,4,0],[92098,36,92098,44,0,0,4,1],[92103,13,92103,23,0,0,4,1],[92106,12,92106,22,0,0,4,0],[92106,12,92106,22,0,0,4,1],[92112,11,92112,20,0,0,4,0],[92112,11,92112,20,0,0,4,1],[92113,11,92113,26,0,0,4,0],[92113,11,92113,26,0,0,4,1],[92114,13,92114,21,0,0,4,0],[92114,13,92114,21,0,0,4,1],[92134,11,92134,33,0,0,4,0],[14098,27,14098,28,2,0,4,0],[14098,27,14098,28,2,0,4,1],[14098,27,14098,28,3,0,4,0],[14098,27,14098,28,3,0,4,1],[14098,27,14098,28,4,0,4,0],[14098,27,14098,28,4,0,4,1],[14098,27,14098,28,5,0,4,0],[14098,27,14098,28,5,0,4,1],[14679,20,14679,27,6,0,4,0],[14679,20,14679,27,6,0,4,1]],"btreePayloadToLocal":[[70957,7,70957,25,0,0,4,0],[70964,12,70964,35,0,0,4,0]],"sqlite3VdbeMemValidStrRep":[[82110,26,82110,42,0,0,4,1]],"whereSortingCost":[[165365,9,165365,28,0,0,4,1]],"pager_end_transaction":[[58500,15,58500,62,0,0,4,0],[58501,11,58501,32,0,0,4,1],[58505,13,58505,26,0,0,4,0],[58505,30,58505,46,0,0,4,0]],"sqlite3BtreeDelete":[[79564,9,79564,23,0,0,4,0],[79566,11,79566,13,0,0,4,1]]},"library_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_free_filename","sqlite3_progress_handler","sqlite3_uri_parameter","sqlite3_status","sqlite3_trace","sqlite3_set_clientdata","sqlite3_create_collation","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_reset","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_free_filename"],"critical_calls":["sqlite3_create_filename","sqlite3_open_v2","sqlite3_progress_handler","sqlite3_uri_parameter","sqlite3_status","sqlite3_trace","sqlite3_set_clientdata","sqlite3_create_collation","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_reset","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_free_filename"],"visited":1}
/**/


// Callback function for sqlite3_exec
static int  id_000066_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

// Progress handler callback
static int  id_000066_progress_handler_callback(void* data) {
    return 0;
}

// Trace callback
static void  id_000066_trace_callback(void* data, const char* sql) {
    // Can be left empty for fuzzing
}

// Collation comparison callback
static int  id_000066_collation_callback(void* data, int len1, const void* str1, int len2, const void* str2) {
    return len1 - len2;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_str* str_obj = nullptr;
    char* err_msg = nullptr;
    
    // 1. Create a database filename
    const char* zDatabase = "fuzz.db";
    const char* zJournal = "fuzz.db-journal";
    const char* zWal = "fuzz.db-wal";
    std::vector<const char*> params;
    
    // Use input data to create parameters for the filename
    const char* param1 = "mode";
    const char* param2 = "memory";
    params.push_back(param1);
    params.push_back(param2);
    
    sqlite3_filename filename = sqlite3_create_filename(
        zDatabase, zJournal, zWal, 
        static_cast<int>(params.size()/2), 
        params.data()
    );
    
    if (!filename) {
	return 0;
	}
	
    
    // 2. Open database
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI;
    int rc = sqlite3_open_v2(filename, &db, flags, nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_free_filename(filename);
        return 0;
    }
    
    // 3. Set progress handler
    sqlite3_progress_handler(db, 1000,  id_000066_progress_handler_callback, nullptr);
    
    // 4. Get URI parameter
    const char* uri_param = sqlite3_uri_parameter(filename, "mode");
    
    // 5. Check status
    int current = 0, highwater = 0;
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 0);
    
    // 6. Set trace callback
    sqlite3_trace(db,  id_000066_trace_callback, nullptr);
    
    // 7. Set client data
    int client_data_value = 42;
    sqlite3_set_clientdata(db, "fuzz_key", &client_data_value, nullptr);
    
    // 8. Create collation
    sqlite3_create_collation(db, "FUZZ_COLLATION", SQLITE_UTF8, 
                            nullptr,  id_000066_collation_callback);
    
    // 9. Execute SQL using input data
    // Create SQL from input (with safety checks)
    size_t sql_len = size > 1024 ? 1024 : size;
    std::vector<char> sql_buf(sql_len + 1);
    memcpy(sql_buf.data(), data, sql_len);
    sql_buf[sql_len] = '\0';
    
    // Execute the SQL
    rc = sqlite3_exec(db, sql_buf.data(),  id_000066_exec_callback, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 10. Prepare and execute a statement
    const char* simple_sql = "SELECT 1";
    rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Use input to determine how many steps to attempt
        size_t max_steps = size > 0 ? (data[0] % 10) + 1 : 1;
        for (size_t i = 0; i < max_steps; ++i) {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                // Get column text
                const unsigned char* col_text = sqlite3_column_text(stmt, 0);
                // Do something with col_text to use it
                if (col_text) {
                    // Force usage to prevent optimization
                    volatile const unsigned char* dummy = col_text;
                    (void)dummy;
                }
            } else if (rc == SQLITE_DONE) {
                break;
            }
        }
        
        // Reset statement
        sqlite3_reset(stmt);
    }
    
    // 11. Create and use sqlite3_str object
    str_obj = sqlite3_str_new(nullptr);
    if (str_obj) {
        // Append part of input data to the string
        size_t append_len = size > 512 ? 512 : size;
        if (append_len > 0) {
            std::vector<char> temp_buf(append_len + 1);
            memcpy(temp_buf.data(), data, append_len);
            temp_buf[append_len] = '\0';
            sqlite3_str_append(str_obj, temp_buf.data(), 
                              static_cast<int>(append_len));
        }
        
        // Finish the string
        char* str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    // 12. Clean up prepared statement
    if (stmt) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 13. Execute another SQL command
    const char* cleanup_sql = "DROP TABLE IF EXISTS temp_fuzz";
    rc = sqlite3_exec(db, cleanup_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 14. Close database
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
    
    // 15. Free filename
    if (filename) {
        sqlite3_free_filename(filename);
    }
    
    return 0;
}