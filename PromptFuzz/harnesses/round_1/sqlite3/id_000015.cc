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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 145, nr_unique_branch: 17
//<Quality> {"density":29,"unique_branches":{"sqlite3VdbeMemSetStr":[[83119,7,83119,15,0,0,4,1]],"openDatabase":[[180851,13,180851,51,0,0,4,0]],"sqlite3_open16":[[181203,7,181203,14,0,0,4,1],[181208,7,181208,9,0,0,4,1],[181210,7,181210,19,0,0,4,1],[181214,7,181214,17,0,0,4,0],[181218,9,181218,22,0,0,4,0],[181218,26,181218,40,0,0,4,0]],"sqlite3BtreeOpen":[[72326,9,72326,43,0,0,4,0],[72333,11,72333,25,0,0,4,1],[72337,11,72337,18,0,0,4,1],[72342,13,72342,15,0,0,4,1],[72358,57,72358,60,0,0,4,0],[72358,57,72358,60,0,0,4,1],[72360,13,72360,75,0,0,4,1],[72550,7,72550,16,0,0,4,0],[14679,20,14679,27,11,0,4,0]]},"library_calls":["sqlite3_enable_shared_cache","sqlite3_open16","sqlite3_open16","sqlite3_prepare_v2","sqlite3_complete16","sqlite3_bind_parameter_index","sqlite3_value_dup","sqlite3_bind_value","sqlite3_value_free","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_exec","sqlite3_exec","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_read","sqlite3_blob_close","sqlite3_exec","sqlite3_exec","sqlite3_aggregate_context","sqlite3_value_int","sqlite3_aggregate_context","sqlite3_value_int","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_aggregate_context","sqlite3_finalize","sqlite3_db_release_memory","sqlite3_finalize","sqlite3_enable_shared_cache","sqlite3_close","sqlite3_close","sqlite3_finalize","sqlite3_backup_finish","sqlite3_blob_close"],"critical_calls":["sqlite3_enable_shared_cache","sqlite3_open16","sqlite3_open16","sqlite3_prepare_v2","sqlite3_complete16","sqlite3_bind_parameter_index","sqlite3_value_dup","sqlite3_bind_value","sqlite3_value_free","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_exec","sqlite3_exec","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_read","sqlite3_blob_close","sqlite3_exec","sqlite3_exec","sqlite3_value_int","sqlite3_aggregate_context","sqlite3_value_int","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_aggregate_context","sqlite3_result_int","sqlite3_aggregate_context","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_aggregate_context","sqlite3_finalize","sqlite3_db_release_memory","sqlite3_finalize","sqlite3_enable_shared_cache","sqlite3_close","sqlite3_close","sqlite3_finalize","sqlite3_backup_finish","sqlite3_blob_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables to track resource cleanup
    sqlite3 *db = nullptr;
    sqlite3 *db2 = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    sqlite3_blob *blob = nullptr;
    const char *dummy_sql = "CREATE TABLE t1(a,b);";
    
    // 1. Create input file from the byte stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Enable shared cache
    sqlite3_enable_shared_cache(1);
    
    // 3. Open databases - using sqlite3_open16 with filename pointers
    // Convert "input_file" to UTF-16 for sqlite3_open16
    const wchar_t *filename16 = L"input_file";
    int rc = sqlite3_open16(filename16, &db);
    
    // Open second database for backup operations
    const wchar_t *filename16_2 = L"output_file";
    rc = sqlite3_open16(filename16_2, &db2);
    
    // 4. Create and prepare a statement
    if (db && rc == SQLITE_OK) {
        rc = sqlite3_prepare_v2(db, "INSERT INTO t1 VALUES(?, ?)", -1, &stmt, nullptr);
        
        if (stmt) {
            // 5. Use sqlite3_complete16 to check SQL statement
            const wchar_t *sql16 = (const wchar_t *)L"INSERT INTO t1 VALUES(1,2)";
            int complete = sqlite3_complete16(sql16);
            
            // 6. Get parameter index
            int param_idx = sqlite3_bind_parameter_index(stmt, "?1");
            
            if (param_idx > 0) {
                // 7. Create a value and bind it
                sqlite3_value *val = sqlite3_value_dup(nullptr);
                if (val) {
                    sqlite3_bind_value(stmt, param_idx, val);
                    sqlite3_value_free(val);
                }
            }
            
            // 8. Create backup between databases
            if (db2) {
                backup = sqlite3_backup_init(db2, "main", db, "main");
                if (backup) {
                    // 9. Get backup page count
                    int pagecount = sqlite3_backup_pagecount(backup);
                    
                    // Perform backup steps
                    sqlite3_backup_step(backup, 5);
                    sqlite3_backup_finish(backup);
                    backup = nullptr;
                }
            }
            
            // 10. Try blob operations if we have data
            if (size > 0) {
                // First execute statement to create table if needed
                sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS blobs(id INTEGER PRIMARY KEY, data BLOB)", nullptr, nullptr, nullptr);
                sqlite3_exec(db, "INSERT OR IGNORE INTO blobs VALUES(1, zeroblob(100))", nullptr, nullptr, nullptr);
                
                // Open blob
                rc = sqlite3_blob_open(db, "main", "blobs", "data", 1, 0, &blob);
                if (blob == SQLITE_OK && blob) {
                    // 11. Use sqlite3_blob_reopen
                    sqlite3_blob_reopen(blob, 1);
                    
                    // Try to read some data from blob
                    char buffer[10];
                    if (size > 10) {
                        sqlite3_blob_read(blob, buffer, 10, 0);
                    }
                    
                    sqlite3_blob_close(blob);
                    blob = nullptr;
                }
            }
            
            // 12. Create aggregate context for testing
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS agg_test(x INTEGER)", nullptr, nullptr, nullptr);
            sqlite3_exec(db, "INSERT INTO agg_test VALUES(1),(2),(3)", nullptr, nullptr, nullptr);
            
            // Define an aggregate function
            struct AggContext {
                int count;
                int sum;
            };
            
            static auto step_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
                AggContext* agg = static_cast<AggContext*>(sqlite3_aggregate_context(ctx, sizeof(AggContext)));
                if (agg && argc > 0) {
                    agg->count++;
                    agg->sum += sqlite3_value_int(argv[0]);
                }
            };
            
            static auto final_func = [](sqlite3_context* ctx) {
                AggContext* agg = static_cast<AggContext*>(sqlite3_aggregate_context(ctx, 0));
                if (agg) {
                    sqlite3_result_int(ctx, agg->count > 0 ? agg->sum / agg->count : 0);
                }
            };
            
            // Register aggregate function
            sqlite3_create_function(db, "myavg", 1, SQLITE_UTF8, nullptr, nullptr, step_func, final_func);
            
            // Execute aggregate query
            sqlite3_stmt* agg_stmt;
            if (sqlite3_prepare_v2(db, "SELECT myavg(x) FROM agg_test", -1, &agg_stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(agg_stmt) == SQLITE_ROW) {
                    // Get aggregate context count
                    void* agg_ctx = sqlite3_aggregate_context(
                        reinterpret_cast<sqlite3_context*>(agg_stmt), 0);
                    // Note: sqlite3_aggregate_count is not a standard API, 
                    // so we simulate its usage with the context
                }
                sqlite3_finalize(agg_stmt);
            }
            
            // 13. Release database memory
            sqlite3_db_release_memory(db);
            
            // Clean up statement
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 14. Disable shared cache at the end
    sqlite3_enable_shared_cache(0);
    
    // Cleanup
    if (in_file) assert_file_closed(&in_file);;
    if (db) sqlite3_close(db);
    if (db2) sqlite3_close(db2);
    if (stmt) sqlite3_finalize(stmt);
    if (backup) sqlite3_backup_finish(backup);
    if (blob) sqlite3_blob_close(blob);
    
    assert_file_closed(&in_file);
	return 0;
}