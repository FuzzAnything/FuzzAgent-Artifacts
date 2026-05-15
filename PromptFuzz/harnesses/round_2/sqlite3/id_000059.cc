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
//<score> 450, nr_unique_branch: 54
//<Quality> {"density":15,"unique_branches":{"freeP4":[[85347,11,85347,13,0,0,4,1]],"pthreadMutexHeld":[[29003,11,29003,21,0,0,4,1]],"sqlite3InitOne":[[140455,7,140455,18,0,0,4,0]],"sqlite3ErrorToParser":[[34710,7,34710,12,0,0,4,1],[34710,16,34710,40,0,0,4,1]],"sqlite3_errmsg":[[180241,7,180241,23,0,0,4,0]],"sqlite3ExprCheckHeight":[[108724,7,108724,23,0,0,4,0]],"growOpArray":[[84159,7,84159,49,0,0,4,0]],"sqlite3WhereBegin":[[166432,7,166432,19,0,0,4,0],[166898,7,166898,13,0,0,4,0]],"sqlite3ResolveExprListNames":[[107830,9,107830,60,0,0,4,0]],"sqlite3RunParser":[[176948,9,176948,19,0,0,4,0]],"sqlite3Update":[[151897,7,151897,19,0,0,4,0]],"sqlite3VdbeMemSetStr":[[83138,7,83138,19,0,0,4,0],[83139,9,83139,13,0,0,4,0],[83139,17,83139,39,0,0,4,0],[83139,17,83139,39,0,0,4,1],[83140,11,83140,31,0,0,4,0]],"deleteTable":[[120962,7,120962,24,0,0,4,1]],"sqlite3StrAccumEnlarge":[[31663,7,31663,18,0,0,4,0],[31674,9,31674,35,0,0,4,1],[31679,9,31679,27,0,0,4,0],[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,2,0,4,1]],"sqlite3_mutex_held":[[28695,18,28695,57,0,0,4,1]],"enlargeAndAppend":[[31728,7,31728,10,0,0,4,1]],"corruptSchema":[[140242,7,140242,23,0,0,4,1],[140244,13,140244,34,0,0,4,1],[140246,13,140246,53,0,0,4,1],[140258,13,140258,43,0,0,4,1],[140262,24,140262,32,0,0,4,0],[140264,9,140264,15,0,0,4,0],[140264,19,140264,28,0,0,4,0]],"sqlite3FinishCoding":[[120413,7,120413,22,0,0,4,1]],"sqlite3_limit":[[180526,9,180526,37,0,0,4,0],[180528,15,180528,25,0,0,4,0],[180528,29,180528,57,0,0,4,0],[180528,29,180528,57,0,0,4,1]],"sqlite3Pragma":[[139404,11,139404,14,0,0,4,1]],"sqlite3StrAccumSetError":[[30833,7,30833,28,0,0,4,0]],"sqlite3VdbeExec":[[99219,9,99219,25,0,0,4,1]],"sqlite3NestedParse":[[120450,7,120450,14,0,0,4,0],[120454,9,120454,26,0,0,4,0]],"addOp4IntSlow":[[84215,7,84215,29,0,0,4,1]],"sqlite3_str_reset":[[19944,24,19944,72,1,0,4,0]],"sqlite3DbStrNDup":[[30597,7,30597,11,0,0,4,1]],"sqlite3InitCallback":[[140363,9,140363,22,0,0,4,0],[140364,11,140364,33,0,0,4,1],[140367,13,140367,27,0,0,4,0],[140368,13,140368,29,0,0,4,1],[140370,19,140370,39,0,0,4,0],[140370,43,140370,67,0,0,4,0]],"sqlite3VdbeAddOpList":[[85114,37,85114,56,0,0,4,0]],"sqlite3AddColumn":[[121640,7,121640,48,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_memory_alarm","sqlite3_limit","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_database_name16","sqlite3_column_bytes16","sqlite3_reset","sqlite3_db_mutex","sqlite3_mutex_held","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_bytes16","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_memory_alarm","sqlite3_limit","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_database_name16","sqlite3_column_bytes16","sqlite3_reset","sqlite3_db_mutex","sqlite3_mutex_held","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_bytes16","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc;
    
    // Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open a database (use in-memory database to avoid file I/O issues)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set memory alarm callback
    sqlite3_memory_alarm(
        [](void *user_data, sqlite3_int64 used, int threshold) {
            // Simple callback that does nothing
            (void)user_data;
            (void)used;
            (void)threshold;
        },
        nullptr,
        1000  // threshold
    );
    
    // Set database limits using data from input
    // Use first 4 bytes to determine limit values (modulo to keep reasonable)
    if (size >= 8) {
        uint32_t limit_id = (data[0] % 10);  // 0-9 for limit types
        int new_val = (data[1] % 1000);      // Reasonable limit value
        sqlite3_limit(db, limit_id, new_val);
    }
    
    // Create a simple table and query it
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test(name) VALUES('test1'),('test2');"
        "SELECT * FROM test;";
    
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Execute the prepared statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column database name (UTF-16)
            const void *db_name = sqlite3_column_database_name16(stmt, 0);
            (void)db_name;  // Use it to avoid compiler warnings
            
            // Get column bytes (UTF-16)
            int col_bytes = sqlite3_column_bytes16(stmt, 0);
            (void)col_bytes;  // Use it to avoid compiler warnings
        }
        
        sqlite3_reset(stmt);
        
        // Check mutex - we need to get a mutex first
        sqlite3_mutex *mutex = sqlite3_db_mutex(db);
        if (mutex) {
            int is_held = sqlite3_mutex_held(mutex);
            (void)is_held;  // Use it to avoid compiler warnings
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Also test with a blob column to exercise bytes16 function more
    const char *blob_sql = 
        "CREATE TABLE IF NOT EXISTS blob_test(data BLOB);"
        "INSERT INTO blob_test VALUES(x'01020304');"
        "SELECT data FROM blob_test;";
    
    rc = sqlite3_prepare_v2(db, blob_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int blob_bytes = sqlite3_column_bytes16(stmt, 0);
            (void)blob_bytes;
        }
        sqlite3_finalize(stmt);
    }
    
    // Read some data from the input file to consume it
    fseek(in_file, 0, SEEK_SET);
    char buffer[256];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    (void)bytes_read;  // Use it to avoid compiler warnings
    
    // Create output file (for demonstration, though we don't write much)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to output file
        const char *output_msg = "Fuzzing output";
        fwrite(output_msg, 1, strlen(output_msg), out_file);
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    if (db) {
        sqlite3_close(db);
    }
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}