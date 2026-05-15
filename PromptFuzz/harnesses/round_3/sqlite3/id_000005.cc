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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 65, nr_unique_branch: 18
//<Quality> {"density":13,"unique_branches":{"sqlite3VdbeExec":[[96146,9,96146,40,0,0,4,0]],"sqlite3HexToBlob":[[35927,14,35927,17,0,0,4,0]],"sqlite3VdbeHalt":[[87349,32,87349,43,0,0,4,0]],"patternCompare":[[128189,11,128189,17,0,0,4,1],[128192,13,128192,19,0,0,4,0],[128209,16,128209,43,0,0,4,0],[128209,16,128209,43,0,0,4,1],[128210,15,128210,20,0,0,4,0],[128210,15,128210,20,0,0,4,1],[128212,15,128212,37,0,0,4,0],[128212,15,128212,37,0,0,4,1],[128258,20,128258,53,0,0,4,0],[128258,61,128258,67,0,0,4,0],[128258,61,128258,67,0,0,4,1],[128258,71,128258,78,0,0,4,0],[128082,37,128082,46,2,0,4,1],[128082,37,128082,46,12,0,4,0],[128082,37,128082,46,12,0,4,1]]},"library_calls":["sqlite3_open16","sqlite3_create_module_v2","sqlite3_strlike","sqlite3_sleep","sqlite3_last_insert_rowid","sqlite3_vtab_collation","sqlite3_prepare_v2","sqlite3_expired","sqlite3_finalize","sqlite3_exec","sqlite3_exec","sqlite3_malloc","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_last_insert_rowid","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_open16","sqlite3_create_module_v2","sqlite3_strlike","sqlite3_sleep","sqlite3_last_insert_rowid","sqlite3_vtab_collation","sqlite3_prepare_v2","sqlite3_expired","sqlite3_finalize","sqlite3_exec","sqlite3_exec","sqlite3_malloc","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_last_insert_rowid","sqlite3_exec","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<154) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    const sqlite3_module *pModule = nullptr;
    void *pClientData = nullptr;
    int rc;
    
    // 1. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open database using sqlite3_open16
    // Convert "input_file" to UTF-16 for sqlite3_open16
    const wchar_t *filename = L"input_file";
    rc = sqlite3_open16(filename, &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create a virtual table module using sqlite3_create_module_v2
    // We need to define a minimal sqlite3_module structure
    static sqlite3_module dummy_module = {
        0,                              // iVersion
        nullptr,                        // xCreate
        nullptr,                        // xConnect
        nullptr,                        // xBestIndex
        nullptr,                        // xDisconnect
        nullptr,                        // xDestroy
        nullptr,                        // xOpen
        nullptr,                        // xClose
        nullptr,                        // xFilter
        nullptr,                        // xNext
        nullptr,                        // xEof
        nullptr,                        // xColumn
        nullptr,                        // xRowid
        nullptr,                        // xUpdate
        nullptr,                        // xBegin
        nullptr,                        // xSync
        nullptr,                        // xCommit
        nullptr,                        // xRollback
        nullptr,                        // xFindFunction
        nullptr,                        // xRename
        nullptr,                        // xSavepoint
        nullptr,                        // xRelease
        nullptr,                        // xRollbackTo
        nullptr                         // xShadowName
    };
    
    rc = sqlite3_create_module_v2(db, fuzz_str_1, &dummy_module, 
                                   pClientData, nullptr);
    
    // 4. Use sqlite3_strlike for pattern matching
    // Create some test strings from input data
    char pattern[256];
    char test_string[256];
    size_t pattern_len = size < 255 ? size : 255;
    memcpy(pattern, data, pattern_len);
    pattern[pattern_len] = '\0';
    
    // Use a portion of data for test string
    size_t test_len = size > 100 ? 100 : size;
    memcpy(test_string, data + (size > 100 ? size - 100 : 0), test_len);
    test_string[test_len] = '\0';
    
    // Use sqlite3_strlike (similar to LIKE operator)
    rc = sqlite3_strlike(pattern, test_string, fuzz_uint32_t_2);
    
    // 5. Use sqlite3_sleep to introduce delay (minimal for fuzzing)
    sqlite3_sleep(fuzz_int32_t_3);  // Sleep for 1ms
    
    // 6. Use sqlite3_last_insert_rowid
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    
    // 7. Use sqlite3_vtab_collation (requires sqlite3_index_info)
    // Create a minimal sqlite3_index_info structure
    sqlite3_index_info idx_info = {0};
    const char *collation = sqlite3_vtab_collation(&idx_info, fuzz_int32_t_4);
    // collation will be nullptr since we didn't setup proper structure
    
    // 8. Prepare a statement to use with sqlite3_expired
    const char *simple_sql = "SELECT 1";
    rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 9. Check if statement is expired using sqlite3_expired
        int expired = sqlite3_expired(stmt);
        
        // Clean up the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 10. Create a simple table and insert data to test last_insert_rowid
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB)";
    sqlite3_exec(db, fuzz_str_5, nullptr, nullptr, nullptr);
    
    // Insert some data using the input
    sqlite3_exec(db, fuzz_str_6, nullptr, nullptr, nullptr);
    
    char insert_sql[512];
    // Use a portion of data for insertion
    size_t insert_size = size > 1000 ? 1000 : size;
    char *escaped_data = (char *)sqlite3_malloc(insert_size * 2 + 1);
    if (escaped_data) {
        // Simple escaping for demonstration
        memcpy(escaped_data, data, insert_size);
        escaped_data[insert_size] = '\0';
        
        snprintf(insert_sql, sizeof(insert_sql), 
                 "INSERT INTO test(data) VALUES (x'%02x%02x%02x')", 
                 data[0] & 0xFF, 
                 size > 1 ? data[1] & 0xFF : 0, 
                 size > 2 ? data[2] & 0xFF : 0);
        
        sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
        sqlite3_free(escaped_data);
    }
    
    sqlite3_exec(db, fuzz_str_7, nullptr, nullptr, nullptr);
    
    // Get the last insert rowid again
    last_rowid = sqlite3_last_insert_rowid(db);
    
    // Cleanup
    assert_file_closed(&in_file);;
    
    if (db) {
        // Drop the test table
        sqlite3_exec(db, fuzz_str_8, nullptr, nullptr, nullptr);
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);
	return 0;
}