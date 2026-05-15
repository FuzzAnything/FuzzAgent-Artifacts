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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 72
//<Quality> {"density":16,"unique_branches":{"sqlite3_snapshot_open":[[182519,7,182519,32,0,0,4,1],[182524,7,182524,24,0,0,4,1]],"sqlite3_cancel_auto_extension":[[136532,7,136532,15,0,0,4,1],[136535,33,136535,37,0,0,4,0],[136536,9,136536,34,0,0,4,0]],"sqlite3CreateFunc":[[179557,13,179557,22,0,0,4,0],[179557,26,179557,35,0,0,4,1]],"sqlite3_auto_extension":[[136477,7,136477,15,0,0,4,1],[136481,7,136481,9,0,0,4,1],[136492,14,136492,31,0,0,4,1],[136495,9,136495,27,0,0,4,0],[136499,11,136499,18,0,0,4,1]],"sqlite3_test_control":[[181698,11,181698,13,0,0,4,1],[181703,5,181703,35,0,0,4,1],[181713,5,181713,38,0,0,4,1],[181739,5,181739,35,0,0,4,1],[181762,5,181762,38,0,0,4,1],[181781,5,181781,37,0,0,4,1],[181799,5,181799,39,0,0,4,1],[181820,5,181820,45,0,0,4,1],[181842,5,181842,38,0,0,4,1],[181864,5,181864,32,0,0,4,1],[181923,5,181923,32,0,0,4,1],[181940,5,181940,35,0,0,4,1],[181954,5,181954,39,0,0,4,0],[181973,5,181973,41,0,0,4,1],[181989,5,181989,44,0,0,4,1],[182003,5,182003,39,0,0,4,1],[182020,5,182020,45,0,0,4,1],[182030,5,182030,46,0,0,4,1],[182040,5,182040,39,0,0,4,1],[182051,5,182051,37,0,0,4,1],[182062,5,182062,32,0,0,4,1],[182083,5,182083,34,0,0,4,1],[182126,5,182126,40,0,0,4,1],[182141,5,182141,36,0,0,4,1],[182158,5,182158,36,0,0,4,1],[182179,5,182179,32,0,0,4,1],[182199,5,182199,39,0,0,4,1],[182225,5,182225,30,0,0,4,1]],"sqlite3_get_table_cb":[[149899,7,149899,17,0,0,4,0],[149899,7,149899,17,0,0,4,1],[149899,21,149899,28,0,0,4,0],[149904,7,149904,34,0,0,4,1],[149915,7,149915,17,0,0,4,0],[149915,7,149915,17,0,0,4,1],[149917,14,149917,20,0,0,4,0],[149917,14,149917,20,0,0,4,1],[149919,11,149919,15,0,0,4,1],[149922,13,149922,34,0,0,4,1],[149933,7,149933,14,0,0,4,0],[149934,14,149934,20,0,0,4,0],[149934,14,149934,20,0,0,4,1],[149935,11,149935,21,0,0,4,1],[149940,13,149940,17,0,0,4,1]],"sqlite3_free_table":[[150036,7,150036,15,0,0,4,0],[150041,14,150041,17,0,0,4,0],[150041,14,150041,17,0,0,4,1],[150041,29,150041,40,0,0,4,0]],"sqlite3_get_table":[[149976,7,149976,32,0,0,4,1],[149976,36,149976,48,0,0,4,1],[149979,7,149979,15,0,0,4,0],[149980,7,149980,12,0,0,4,0],[149981,7,149981,15,0,0,4,0],[149989,7,149989,22,0,0,4,1],[149997,7,149997,30,0,0,4,1],[150010,7,150010,20,0,0,4,1],[150014,7,150014,27,0,0,4,0],[150017,9,150017,17,0,0,4,1],[150025,7,150025,15,0,0,4,0],[150026,7,150026,12,0,0,4,0]],"sqlite3_expanded_sql":[[91571,7,91571,11,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_free_table","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_auto_extension","sqlite3_cancel_auto_extension","sqlite3_exec","sqlite3_free","sqlite3_get_table","sqlite3_free","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_table_name","sqlite3_create_function","sqlite3_exec","sqlite3_status64","sqlite3_test_control","sqlite3_snapshot_open","sqlite3_free_table","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_auto_extension","sqlite3_cancel_auto_extension","sqlite3_exec","sqlite3_get_table","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_free","sqlite3_step","sqlite3_column_table_name","sqlite3_create_function","sqlite3_exec","sqlite3_status64","sqlite3_test_control","sqlite3_snapshot_open","sqlite3_free_table","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close"],"visited":0}
/**/


// Global variable for auto-extension function
static void  id_000037_dummy_extension() {}

// Global variable to store aggregate count
static int  id_000037_global_aggregate_count = 0;

// Custom aggregate function to use with sqlite3_aggregate_count
static void  id_000037_aggregate_step(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Do nothing - just a dummy aggregate function
}

static void  id_000037_aggregate_final(sqlite3_context* ctx) {
    // Get the aggregate count
     id_000037_global_aggregate_count = sqlite3_aggregate_count(ctx);
    sqlite3_result_int(ctx, 0);
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char** table_result = nullptr;
    int rows = 0, cols = 0;
    char* errmsg = nullptr;
    sqlite3_backup* backup = nullptr;
    
    // 1. Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        cleanup:
        if (table_result) sqlite3_free_table(table_result);
        if (stmt) sqlite3_finalize(stmt);
        if (backup) sqlite3_backup_finish(backup);
        if (db) sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Register an auto-extension then cancel it
    sqlite3_auto_extension((void(*)()) id_000037_dummy_extension);
    sqlite3_cancel_auto_extension((void(*)()) id_000037_dummy_extension);
    
    // 3. Create a simple table and insert some data
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test_table (name) VALUES ('test1'), ('test2'), ('test3');";
    
    if (sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        goto cleanup;
    }
    
    // 4. Use sqlite3_get_table to query the table
    const char* query = "SELECT * FROM test_table;";
    if (sqlite3_get_table(db, query, &table_result, &rows, &cols, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        goto cleanup;
    }
    
    // 5. Prepare a statement and use sqlite3_column_table_name
    const char* select_query = "SELECT id, name FROM test_table;";
    if (sqlite3_prepare_v2(db, select_query, -1, &stmt, nullptr) == SQLITE_OK) {
        // Get expanded SQL
        char* expanded_sql = sqlite3_expanded_sql(stmt);
        if (expanded_sql) {
            fwrite(expanded_sql, 1, strlen(expanded_sql), out_file);
            sqlite3_free(expanded_sql);
        }
        
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get table name for the first column
            const char* table_name = sqlite3_column_table_name(stmt, 0);
            if (table_name) {
                fwrite(table_name, 1, strlen(table_name), out_file);
            }
        }
    }
    
    // 6. Create aggregate function and use sqlite3_aggregate_count
    sqlite3_create_function(db, "my_aggregate", 1, SQLITE_UTF8, nullptr, 
                          nullptr,  id_000037_aggregate_step,  id_000037_aggregate_final);
    
    // Execute a query using the aggregate function
    const char* agg_query = "SELECT my_aggregate(id) FROM test_table;";
    sqlite3_exec(db, agg_query, nullptr, nullptr, nullptr);
    
    // 7. Use sqlite3_status64 to get memory status
    sqlite3_int64 current_mem = 0, highwater_mem = 0;
    sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &current_mem, &highwater_mem, 0);
    
    // 8. Use sqlite3_test_control
    sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0xFFFFFFFF);
    
    // 9. Attempt to use sqlite3_snapshot_open (may fail if no WAL mode)
    sqlite3_snapshot* snapshot = nullptr;
    sqlite3_snapshot_open(db, "main", snapshot);
    
    // Cleanup
    if (table_result) sqlite3_free_table(table_result);
    if (stmt) sqlite3_finalize(stmt);
    if (backup) sqlite3_backup_finish(backup);
    if (db) sqlite3_close(db);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}