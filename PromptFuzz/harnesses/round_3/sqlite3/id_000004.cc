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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 14
//<Quality> {"density":15,"unique_branches":{"sqlite3_create_module_v2":[[153776,7,153776,32,0,0,4,1],[153776,36,153776,44,0,0,4,1]],"vdbeCompareMemString":[[88370,7,88370,29,0,0,4,1],[88385,10,88385,15,0,0,4,1],[88385,19,88385,24,0,0,4,1]],"sqlite3_open16":[[181203,7,181203,14,0,0,4,1],[181208,7,181208,9,0,0,4,1],[181210,7,181210,19,0,0,4,1],[181214,7,181214,17,0,0,4,0],[181218,9,181218,22,0,0,4,0],[181218,26,181218,40,0,0,4,0]],"sqlite3VdbeExec":[[93573,9,93573,32,0,0,4,1]],"sqlite3_sleep":[[181613,7,181613,14,0,0,4,1],[181618,30,181618,34,0,0,4,1]]},"library_calls":["sqlite3_open16","sqlite3_create_module_v2","sqlite3_sleep","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_prepare_v2","sqlite3_expired","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open16","sqlite3_create_module_v2","sqlite3_sleep","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_prepare_v2","sqlite3_expired","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Open database using sqlite3_open16
    sqlite3 *db = nullptr;
    const char16_t *db_name = u":memory:"; // Use in-memory database for fuzzing
    int rc = sqlite3_open16(db_name, &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Create a virtual table module using sqlite3_create_module_v2
    static sqlite3_module test_module = {
        3,                              // iVersion
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
        nullptr                         // xRollbackTo
    };

    rc = sqlite3_create_module_v2(db, fuzz_str_1, &test_module, 
                                 nullptr, nullptr);
    // Continue even if module creation fails

    // 3. Use sqlite3_strlike for pattern matching
    // Read some data for pattern and string
    uint8_t pattern_buf[100];
    uint8_t str_buf[100];
    size_t read_size = fread(pattern_buf, 1, sizeof(pattern_buf), in_file);
    if (read_size > 0) {
        // Read string data
        fread(str_buf, 1, sizeof(str_buf), in_file);
        
        // Use sqlite3_strlike (note: this is actually sqlite3_strlike, not sqlite3_strglob)
        // We'll use a simple pattern and string
        const char *pattern = "test%";
        const char *str = "test_string";
        unsigned int esc = 0;
        // Note: sqlite3_strlike is not in the provided API list, so we'll skip it
        // and use another pattern matching function if available
    }

    // 4. Use sqlite3_sleep - sleep for very short time (0 or 1 ms)
    int sleep_ms = (data[0] % 2); // 0 or 1 ms
    sqlite3_sleep(sleep_ms);

    // 5. Create a simple table and insert data to get last insert rowid
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB)";
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Insert some data using the input
    const char *insert_sql = "INSERT INTO test (data) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Use first part of input as blob data
        size_t blob_size = size > 1000 ? 1000 : size;
        sqlite3_bind_blob(stmt, fuzz_int32_t_2, data, blob_size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Get last insert rowid
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);

    // 6. Use sqlite3_vtab_collation - we need an sqlite3_index_info structure
    // Create a minimal index_info structure for testing
    struct MinimalIndexInfo {
        int nConstraint;
        void *aConstraint;
        int nOrderBy;
        void *aOrderBy;
        void *aConstraintUsage;
        int idxNum;
        char *idxStr;
        int needToFreeIdxStr;
        int orderByConsumed;
        double estimatedCost;
        double estimatedRows;
        int idxFlags;
        sqlite3_uint64 colUsed;
        int *aiRowEst;
    } idx_info = {0};

    // We can't properly call sqlite3_vtab_collation without a real virtual table
    // context, so we'll just declare the function call but not execute it
    // const char *collation = sqlite3_vtab_collation((sqlite3_index_info*)&idx_info, 0);

    // 7. Use sqlite3_expired - create a prepared statement and check if expired
    const char *select_sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Check if statement is expired (should return 0 for fresh statement)
        int is_expired = sqlite3_expired(stmt);
        (void)is_expired; // Mark as used
        
        // Clean up the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Clean up all resources
    assert_file_closed(&in_file);;
    
    // Finalize any remaining statements
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Close database
    sqlite3_close(db);

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}