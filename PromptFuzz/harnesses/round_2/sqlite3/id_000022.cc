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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 415.66666, nr_unique_branch: 75
//<Quality> {"density":43,"unique_branches":{"downgradeAllSharedCacheTableLocks":[[70259,7,70259,22,0,0,4,1]],"sqlite3_bind_value":[[91277,5,91277,24,0,0,4,1],[91281,5,91281,22,0,0,4,1],[91288,5,91288,21,0,0,4,0],[91289,11,91289,35,0,0,4,1],[91296,5,91296,21,0,0,4,1],[91301,5,91301,12,0,0,4,1]],"findOrCreateAggInfoColumn":[[114582,12,114582,31,0,0,4,1],[114592,7,114592,10,0,0,4,1],[114604,7,114604,25,0,0,4,1],[114620,7,114620,28,0,0,4,0],[114627,7,114627,27,0,0,4,0]],"accessPayload":[[74904,11,74904,27,0,0,4,0],[74913,13,74913,36,0,0,4,1]],"exprRefToSrcList":[[114404,7,114404,27,0,0,4,0],[114410,16,114410,20,0,0,4,0],[114411,14,114411,20,0,0,4,0],[114412,11,114412,44,0,0,4,0]],"zeroblobFunc":[[128777,7,128777,10,0,0,4,0],[128779,7,128779,9,0,0,4,0]],"sqlite3VdbeMemAboutToChange":[[82989,9,82989,29,0,0,4,0],[82991,11,82991,46,0,0,4,1]],"sqlite3_result_zeroblob64":[[90121,7,90121,51,0,0,4,0]],"btreeParseCellPtr":[[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,2,0,4,0]],"cellSizePtrTableLeaf":[[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,2,0,4,0]],"sqlite3_value_type":[[89785,15,89785,36,0,0,4,1]],"sqlite3Select":[[149822,14,149822,33,0,0,4,0],[149824,11,149824,19,0,0,4,1]],"sqlite3VdbeIntValue":[[82585,48,82585,58,0,0,4,1]],"sqlite3_value_dup":[[89818,7,89818,37,0,0,4,0],[89821,9,89821,53,0,0,4,1]],"sqlite3ReferencesSrcList":[[114467,7,114467,21,0,0,4,0]],"sqlite3_transfer_bindings":[[91407,7,91407,29,0,0,4,1],[91411,7,91411,19,0,0,4,1],[91415,7,91415,21,0,0,4,1]],"sqlite3ExprCodeTarget":[[112370,5,112370,23,0,0,4,0],[112375,11,112375,41,0,0,4,1],[112388,11,112388,32,0,0,4,1],[112390,17,112390,40,0,0,4,1],[112406,17,112406,33,0,0,4,1]],"sqlite3_result_error_toobig":[[90149,7,90149,14,0,0,4,1]],"sqlite3VdbeMemGrow":[[14098,27,14098,28,1,0,4,0]],"sqlite3_result_error_code":[[90134,7,90134,14,0,0,4,1],[90136,19,90136,26,0,0,4,0],[90138,7,90138,18,0,0,4,0],[90140,7,90140,35,0,0,4,1]],"sqlite3VdbeExec":[[95764,15,95764,45,0,0,4,0],[100822,7,100822,25,0,0,4,1],[100843,9,100843,24,0,0,4,0],[100849,9,100849,11,0,0,4,0]],"sqlite3ErrStr":[[179261,5,179261,20,0,0,4,0],[179265,5,179265,21,0,0,4,0],[179269,5,179269,12,0,0,4,1],[179271,28,179271,40,0,0,4,1],[179271,50,179271,61,0,0,4,1]],"sqlite3_column_value":[[90869,7,90869,29,0,0,4,0]],"allocateSpace":[[14098,27,14098,28,10,0,4,0]],"isSimpleCount":[[146741,7,146741,65,0,0,4,0]],"sqlite3VdbeMemExpandBlob":[[82362,7,82362,41,0,0,4,0]],"getOverflowPage":[[74703,7,74703,22,0,0,4,1],[74723,7,74723,20,0,0,4,0],[74724,42,74724,53,0,0,4,0],[74726,9,74726,22,0,0,4,0],[74732,7,74732,13,0,0,4,1],[74737,11,74737,26,0,0,4,1]],"analyzeAggregate":[[114687,5,114687,19,0,0,4,0],[114695,18,114695,34,0,0,4,0],[114697,15,114697,44,0,0,4,0],[14098,27,14098,28,11,0,4,1],[14098,27,14098,28,12,0,4,0],[14098,27,14098,28,13,0,4,1],[14155,25,14155,46,14,0,4,0],[14155,26,14155,29,14,0,4,0]]},"library_calls":["sqlite3_keyword_count","sqlite3_errstr","sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_bind_value","sqlite3_value_free","sqlite3_finalize","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_bind_int","sqlite3_step","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_result_int64","sqlite3_result_int64","sqlite3_aggregate_context","sqlite3_result_int64","sqlite3_result_int64","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_reset","sqlite3_clear_bindings","sqlite3_bind_text","sqlite3_bind_int","sqlite3_reset","sqlite3_clear_bindings","sqlite3_transfer_bindings","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_keyword_count","sqlite3_errstr","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_bind_value","sqlite3_value_free","sqlite3_finalize","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_bind_int","sqlite3_step","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_value_int64","sqlite3_aggregate_context","sqlite3_result_int64","sqlite3_result_int64","sqlite3_aggregate_context","sqlite3_result_int64","sqlite3_result_int64","sqlite3_aggregate_context","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_reset","sqlite3_clear_bindings","sqlite3_bind_text","sqlite3_bind_int","sqlite3_reset","sqlite3_clear_bindings","sqlite3_transfer_bindings","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	
    
    // Use sqlite3_keyword_count
    int keyword_count = sqlite3_keyword_count();
    
    // Use sqlite3_errstr with first byte as error code
    int err_code = data[0] % 256;
    const char* err_str = sqlite3_errstr(err_code);
    
    // Create in-memory database
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create table
    const char* create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB, value INTEGER)";
    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare first statement for inserting data
    sqlite3_stmt* stmt1 = nullptr;
    const char* insert_sql = "INSERT INTO test_table (data, value) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement for inserting data (different values)
    sqlite3_stmt* stmt2 = nullptr;
    const char* insert_sql2 = "INSERT INTO test_table (data, value) VALUES (zeroblob(?), ?)";
    if (sqlite3_prepare_v2(db, insert_sql2, -1, &stmt2, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Bind parameters to first statement
    // Use second byte for zeroblob size (modulo 1024 to avoid huge allocations)
    sqlite3_uint64 zeroblob_size = (data[1] % 1024) + 1;
    int bind_idx = 1;
    
    // Create a value for binding
    sqlite3_value* bind_val = nullptr;
    sqlite3_stmt* temp_stmt = nullptr;
    const char* temp_sql = "SELECT ?";
    if (sqlite3_prepare_v2(db, temp_sql, -1, &temp_stmt, nullptr) == SQLITE_OK) {
        // Bind input data as blob to temp statement
        if (sqlite3_bind_blob(temp_stmt, 1, data, static_cast<int>(size), SQLITE_STATIC) == SQLITE_OK) {
            if (sqlite3_step(temp_stmt) == SQLITE_ROW) {
                bind_val = sqlite3_column_value(temp_stmt, 0);
                // sqlite3_bind_value needs a copy of the value
                if (bind_val) {
                    // Copy the value for binding
                    sqlite3_value* copied_val = sqlite3_value_dup(bind_val);
                    if (copied_val) {
                        // Use sqlite3_bind_value
                        sqlite3_bind_value(stmt1, bind_idx, copied_val);
                        sqlite3_value_free(copied_val);
                        bind_idx++;
                    }
                }
            }
        }
        sqlite3_finalize(temp_stmt);
    }
    
    // Use sqlite3_bind_zeroblob64 on stmt2
    if (sqlite3_bind_zeroblob64(stmt2, 1, zeroblob_size) == SQLITE_OK) {
        // Execute stmt2 to create the zeroblob entry
        sqlite3_step(stmt2);
        sqlite3_reset(stmt2);
    }
    
    // Bind a simple integer value to stmt1
    if (bind_idx == 2) { // Only if first bind succeeded
        sqlite3_bind_int(stmt1, 2, keyword_count); // Use keyword count as value
    }
    
    // Execute first statement
    sqlite3_step(stmt1);
    
    // Now prepare a query statement to get values for sqlite3_result_int64
    sqlite3_stmt* query_stmt = nullptr;
    const char* query_sql = "SELECT value FROM test_table";
    if (sqlite3_prepare_v2(db, query_sql, -1, &query_stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(query_stmt) == SQLITE_ROW) {
            // Get int64 value from column
            sqlite3_int64 col_value = sqlite3_column_int64(query_stmt, 0);
            
            // Create a user-defined function context to use sqlite3_result_int64
            // We'll create a simple aggregate function
            struct AggregateContext {
                sqlite3_int64 sum;
            };
            
            // Function step callback
            auto step_callback = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
                AggregateContext* agg = static_cast<AggregateContext*>(sqlite3_aggregate_context(ctx, sizeof(AggregateContext)));
                if (agg && argc > 0) {
                    agg->sum += sqlite3_value_int64(argv[0]);
                }
            };
            
            // Function final callback
            auto final_callback = [](sqlite3_context* ctx) {
                AggregateContext* agg = static_cast<AggregateContext*>(sqlite3_aggregate_context(ctx, 0));
                if (agg) {
                    // Use sqlite3_result_int64
                    sqlite3_result_int64(ctx, agg->sum);
                } else {
                    sqlite3_result_int64(ctx, 0);
                }
            };
            
            // Create the aggregate function
            if (sqlite3_create_function_v2(
                db,
                "test_agg",
                1,
                SQLITE_UTF8,
                nullptr,
                nullptr,
                step_callback,
                final_callback,
                nullptr
            ) == SQLITE_OK) {
                // Execute a query using the aggregate function
                sqlite3_stmt* agg_stmt = nullptr;
                const char* agg_sql = "SELECT test_agg(value) FROM test_table";
                if (sqlite3_prepare_v2(db, agg_sql, -1, &agg_stmt, nullptr) == SQLITE_OK) {
                    sqlite3_step(agg_stmt);
                    sqlite3_finalize(agg_stmt);
                }
            }
            
            break; // Just process first row
        }
        sqlite3_finalize(query_stmt);
    }
    
    // Use sqlite3_transfer_bindings
    // First, bind some values to stmt1
    sqlite3_reset(stmt1);
    sqlite3_clear_bindings(stmt1);
    
    // Bind simple values
    if (size > 10) {
        // Bind first 10 bytes as text
        sqlite3_bind_text(stmt1, 1, reinterpret_cast<const char*>(data), 
                         static_cast<int>(size > 10 ? 10 : size), SQLITE_TRANSIENT);
        // Bind an integer
        sqlite3_bind_int(stmt1, 2, static_cast<int>(data[0]));
        
        // Now transfer bindings from stmt1 to stmt2
        // Clear stmt2 bindings first
        sqlite3_reset(stmt2);
        sqlite3_clear_bindings(stmt2);
        
        // Use sqlite3_transfer_bindings
        sqlite3_transfer_bindings(stmt1, stmt2);
        
        // Execute stmt2 with transferred bindings
        sqlite3_step(stmt2);
    }
    
    // Clean up
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    
    // Drop the function we created
    const char* drop_func_sql = "SELECT 1";
    sqlite3_exec(db, drop_func_sql, nullptr, nullptr, nullptr); // Dummy execution
    
    // Close database
    sqlite3_close(db);
    
    // To avoid unused variable warning
    (void)err_str;
    
    return 0;
}