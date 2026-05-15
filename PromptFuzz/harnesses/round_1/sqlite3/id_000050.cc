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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 5
//<Quality> {"density":25,"unique_branches":{"valueFromValueList":[[90488,7,90488,14,0,0,4,1],[90489,7,90489,33,0,0,4,0],[90489,7,90489,33,0,0,4,1],[90489,37,90489,73,0,0,4,0]],"vdbeColumnFromOverflow":[[92941,9,92941,27,0,0,4,0]]},"library_calls":["sqlite3_soft_heap_limit","sqlite3_open","sqlite3_busy_timeout","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_bind_text","sqlite3_bind_null","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_bytes","sqlite3_column_bytes","sqlite3_malloc64","sqlite3_msize","sqlite3_free","sqlite3_column_value","sqlite3_value_frombind","sqlite3_vtab_in_next","sqlite3_finalize","sqlite3_strnicmp","sqlite3_create_function_v2","sqlite3_aggregate_context","sqlite3_result_error_nomem","sqlite3_aggregate_count","sqlite3_value_int","sqlite3_aggregate_context","sqlite3_result_error_nomem","sqlite3_aggregate_count","sqlite3_value_int","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_result_null","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_result_null","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_soft_heap_limit","sqlite3_open","sqlite3_busy_timeout","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_bytes","sqlite3_column_bytes","sqlite3_malloc64","sqlite3_msize","sqlite3_free","sqlite3_column_value","sqlite3_value_frombind","sqlite3_vtab_in_next","sqlite3_finalize","sqlite3_strnicmp","sqlite3_create_function_v2","sqlite3_result_null","sqlite3_result_int","sqlite3_aggregate_context","sqlite3_result_null","sqlite3_result_int","sqlite3_aggregate_context","sqlite3_value_int","sqlite3_aggregate_count","sqlite3_result_error_nomem","sqlite3_aggregate_context","sqlite3_value_int","sqlite3_aggregate_count","sqlite3_result_error_nomem","sqlite3_aggregate_context","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to process SQLite database output data:

*/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create an in-memory file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;
    
    // 1. Set soft heap limit (use part of size as limit)
    sqlite3_soft_heap_limit(size > 0 ? (int)(size % 10000) : 100);
    
    // 2. Try to open a temporary in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set busy timeout using part of the input size
    sqlite3_busy_timeout(db, size > 0 ? (int)(size % 1000) : 100);
    
    // 4. Create a simple table with blob data
    const char *create_sql = 
        "CREATE TABLE IF NOT EXISTS test_data ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "text_col TEXT);";
    
    rc = sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Insert data from input
    rc = sqlite3_prepare_v2(db, 
        "INSERT INTO test_data (data, text_col) VALUES (?, ?)", 
        -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Use first half of data for blob, second half for text
        size_t blob_size = size / 2;
        size_t text_size = size - blob_size;
        
        if (blob_size > 0) {
            sqlite3_bind_blob(stmt, 1, data, (int)blob_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        if (text_size > 0) {
            sqlite3_bind_text(stmt, 2, (const char*)(data + blob_size), 
                             (int)text_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 6. Query the inserted data
    rc = sqlite3_prepare_v2(db, "SELECT * FROM test_data", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 7. Use sqlite3_column_bytes
            int blob_bytes = sqlite3_column_bytes(stmt, 1);
            int text_bytes = sqlite3_column_bytes(stmt, 2);
            
            // 8. Use sqlite3_msize on allocated memory
            void *alloc_mem = sqlite3_malloc64(100);
            if (alloc_mem) {
                sqlite3_uint64 mem_size = sqlite3_msize(alloc_mem);
                // Do something with the memory
                memset(alloc_mem, 0, 100);
                sqlite3_free(alloc_mem);
            }
            
            // 9. Simulate aggregate context usage
            sqlite3_value *test_value = sqlite3_column_value(stmt, 1);
            if (test_value) {
                // Check if value is from bind
                int from_bind = sqlite3_value_frombind(test_value);
                
                // Simulate vtab_in_next usage (though not in actual vtab context)
                sqlite3_value *next_val = nullptr;
                // Note: This would normally be used in virtual table implementations
                // We call it but it won't have the expected context
                sqlite3_vtab_in_next(test_value, &next_val);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // 10. Test string comparison
    const char *str1 = "test_string";
    const char *str2 = "TEST_STRING";
    int cmp_result = sqlite3_strnicmp(str1, str2, 4); // Compare first 4 chars case-insensitive
    
    // 11. Create and use aggregate context in a custom function
    // Note: This requires creating a custom aggregate function
    // We'll create a simple implementation for demonstration
    
    struct MyAggregateData {
        int count;
        int sum;
    };
    
    // Create a custom aggregate function
    sqlite3_create_function_v2(
        db,
        "my_aggregate",
        1, // Number of arguments
        SQLITE_UTF8,
        nullptr, // pApp data
        nullptr, // xFunc (not used for aggregates)
        [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            // xStep function
            MyAggregateData* agg_data = static_cast<MyAggregateData*>(
                sqlite3_aggregate_context(ctx, sizeof(MyAggregateData)));
            
            if (!agg_data) {
                sqlite3_result_error_nomem(ctx);
                return;
            }
            
            if (sqlite3_aggregate_count(ctx) == 0) {
                // First call - initialize
                agg_data->count = 0;
                agg_data->sum = 0;
            }
            
            agg_data->count++;
            agg_data->sum += sqlite3_value_int(argv[0]);
        },
        [](sqlite3_context* ctx) {
            // xFinal function
            MyAggregateData* agg_data = static_cast<MyAggregateData*>(
                sqlite3_aggregate_context(ctx, sizeof(MyAggregateData)));
            
            if (agg_data && agg_data->count > 0) {
                sqlite3_result_int(ctx, agg_data->sum / agg_data->count);
            } else {
                sqlite3_result_null(ctx);
            }
        },
        nullptr // xDestroy
    );
    
    // Test the aggregate function
    rc = sqlite3_prepare_v2(db, 
        "SELECT my_aggregate(id) FROM test_data", 
        -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Aggregate result available
        }
        sqlite3_finalize(stmt);
    }
    
    // 12. Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}