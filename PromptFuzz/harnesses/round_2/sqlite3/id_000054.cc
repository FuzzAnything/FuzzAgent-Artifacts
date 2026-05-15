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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 76.5, nr_unique_branch: 59
//<Quality> {"density":17,"unique_branches":{"sqlite3DeleteTable":[[121011,7,121011,26,0,0,4,1]],"sqlite3_db_status":[[23826,7,23826,32,0,0,4,1],[23826,36,23826,47,0,0,4,1],[23826,50,23826,63,0,0,4,1],[23832,5,23832,40,0,0,4,0],[23832,5,23832,40,0,0,4,1],[23834,11,23834,20,0,0,4,1],[23855,5,23855,39,0,0,4,0],[23855,5,23855,39,0,0,4,1],[23856,5,23856,45,0,0,4,0],[23856,5,23856,45,0,0,4,1],[23857,5,23857,45,0,0,4,0],[23857,5,23857,45,0,0,4,1],[23865,11,23865,20,0,0,4,1],[23876,5,23876,43,0,0,4,1],[23877,5,23877,36,0,0,4,0],[23877,5,23877,36,0,0,4,1],[23881,16,23881,25,0,0,4,0],[23881,16,23881,25,0,0,4,1],[23883,13,23883,16,0,0,4,0],[23883,13,23883,16,0,0,4,1],[23886,15,23886,52,0,0,4,1],[23903,5,23903,37,0,0,4,0],[23903,5,23903,37,0,0,4,1],[23911,16,23911,25,0,0,4,0],[23911,16,23911,25,0,0,4,1],[23927,54,23927,55,0,0,4,1],[23930,53,23930,54,0,0,4,0],[23930,53,23930,54,0,0,4,1],[23949,5,23949,35,0,0,4,0],[23949,5,23949,35,0,0,4,1],[23956,28,23956,33,0,0,4,1],[23973,5,23973,37,0,0,4,1],[23976,5,23976,35,0,0,4,0],[23976,5,23976,35,0,0,4,1],[23977,5,23977,36,0,0,4,0],[23977,5,23977,36,0,0,4,1],[23978,5,23978,37,0,0,4,0],[23978,5,23978,37,0,0,4,1],[23984,16,23984,25,0,0,4,0],[23984,16,23984,25,0,0,4,1],[23985,13,23985,27,0,0,4,0],[23985,13,23985,27,0,0,4,1],[24001,5,24001,38,0,0,4,1],[24007,5,24007,12,0,0,4,1],[14098,27,14098,28,7,0,4,0],[14098,27,14098,28,7,0,4,1],[14098,27,14098,28,8,0,4,0],[14098,27,14098,28,8,0,4,1],[14098,27,14098,28,9,0,4,0],[14098,27,14098,28,9,0,4,1],[14155,25,14155,46,16,0,4,0],[14155,26,14155,29,16,0,4,0]],"sqlite3DeleteColumnNames":[[120924,9,120924,28,0,0,4,1]],"sqlite3DbFreeNN":[[30264,9,30264,25,0,0,4,0]],"sqlite3PagerCacheStat":[[63302,7,63302,12,0,0,4,1]],"sqlite3_msize":[[30202,10,30202,11,0,0,4,1]],"sqlite3LookasideUsed":[[23810,7,23810,17,0,0,4,0]],"sqlite3DbNNFreeNN":[[30303,7,30303,23,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_db_status","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_finalize","sqlite3_snprintf","sqlite3_exec","sqlite3_db_status","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_db_status","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_finalize","sqlite3_snprintf","sqlite3_exec","sqlite3_db_status","sqlite3_close"],"visited":1}
/**/


// Custom function that uses sqlite3_result_zeroblob64
static void  id_000054_fuzzblob_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    if (argc == 1 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        sqlite3_int64 size = sqlite3_value_int64(argv[0]);
        // Limit size to prevent excessive memory usage
        if (size > 0 && size <= 1024 * 1024) {  // Max 1MB
            sqlite3_result_zeroblob64(ctx, size);
        } else {
            sqlite3_result_error(ctx, "Invalid zeroblob size", -1);
        }
    } else {
        sqlite3_result_error(ctx, "Invalid arguments", -1);
    }
}

// Custom function to demonstrate sqlite3_result_pointer (if available)
static void  id_000054_pointer_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    static int dummy = 42;
    // Note: sqlite3_result_pointer is not in the provided list, but we use a similar pattern
    // with sqlite3_result_blob for demonstration
    sqlite3_result_blob(ctx, &dummy, sizeof(dummy), SQLITE_TRANSIENT);
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	  // Need at least some data
    
    // 1. Create an in-memory database
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // 2. Create a table
    char* errmsg = nullptr;
    const char* create_table_sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    
    // 3. Register custom functions
    // Use first byte to determine function behavior
    int function_type = data[0] % 2;
    
    // Register fuzzblob function (uses sqlite3_result_zeroblob64)
    sqlite3_create_function(
        db, "fuzzblob", 1, SQLITE_UTF8, nullptr,
         id_000054_fuzzblob_func, nullptr, nullptr
    );
    
    // 4. Use the input data to determine parameters
    // Use second and third bytes for zeroblob size (limited)
    uint32_t blob_size = 0;
    if (size >= 4) {
        blob_size = (data[1] << 8) | data[2];
        blob_size = blob_size % 4096;  // Limit to 4KB for safety
    }
    
    // Use fourth byte for db_status operation
    int db_op = (size >= 5) ? (data[3] % 10) : 0;  // Limited to 10 operations
    
    // 5. Insert data using our custom function
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO t1(data) VALUES(fuzzblob(?));";
    
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, blob_size);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 6. Use sqlite3_db_status
    int current = 0, highwater = 0;
    sqlite3_db_status(db, db_op, &current, &highwater, 0);
    
    // 7. Use sqlite3_stmt_explain (if we have a statement)
    stmt = nullptr;
    const char* select_sql = "SELECT * FROM t1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Use sqlite3_stmt_explain if available (simulated with sqlite3_stmt_readonly)
        int is_readonly = sqlite3_stmt_readonly(stmt);
        (void)is_readonly;  // Use the result
        sqlite3_finalize(stmt);
    }
    
    // 8. Use sqlite3_vsnprintf for formatted output
    char buffer[256];
    sqlite3_snprintf(
        sizeof(buffer), buffer,
        "DB Status: current=%d, highwater=%d, blob_size=%u",
        current, highwater, blob_size
    );
    
    // 9. Demonstrate file operations as requested
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Read some data from the memory file
        uint8_t read_buf[16];
        size_t bytes_read = fread(read_buf, 1, sizeof(read_buf), in_file);
        (void)bytes_read;  // Use the result
        assert_file_closed(&in_file);;
    }
    
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(buffer, 1, strlen(buffer), out_file);
        assert_file_closed(&out_file);;
    }
    
    // 10. Cleanup
    // Drop the table to clean up
    sqlite3_exec(db, "DROP TABLE t1;", nullptr, nullptr, nullptr);
    
    // Additional cleanup: use sqlite3_db_status with reset flag
    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current, &highwater, 1);
    
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}