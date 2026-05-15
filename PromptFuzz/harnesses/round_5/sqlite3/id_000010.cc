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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"sqlite3_vtab_collation":[[164465,7,164465,15,0,0,4,0],[164465,19,164465,46,0,0,4,1]]},"library_calls":["sqlite3_sourceid","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_db_handle","sqlite3_db_readonly","sqlite3_malloc","sqlite3_msize","sqlite3_free","sqlite3_vtab_collation","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_sourceid","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_db_handle","sqlite3_db_readonly","sqlite3_malloc","sqlite3_msize","sqlite3_free","sqlite3_vtab_collation","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used

    // Use sqlite3_sourceid() - get SQLite version info
    const char *source_id = sqlite3_sourceid();
    if (source_id && out_file) {
        fwrite(source_id, strlen(source_id), 1, out_file);
    }

    // Open a database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create a simple table and prepare a statement
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data TEXT);";
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);

    sqlite3_stmt *stmt = nullptr;
    const char *select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);

    // Use sqlite3_db_handle() to get database from statement
    if (stmt) {
        sqlite3 *stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db) {
            // Use sqlite3_db_readonly() to check if database is read-only
            int is_readonly = sqlite3_db_readonly(stmt_db, "main");
            if (out_file) {
                fwrite(&is_readonly, sizeof(is_readonly), 1, out_file);
            }
        }
    }

    // Allocate some memory using sqlite3 to demonstrate sqlite3_msize()
    void *mem_block = sqlite3_malloc(1024);
    if (mem_block) {
        // Use sqlite3_msize() to get allocated size
        sqlite3_uint64 mem_size = sqlite3_msize(mem_block);
        if (out_file) {
            fwrite(&mem_size, sizeof(mem_size), 1, out_file);
        }
        sqlite3_free(mem_block);
    }

    // Note: sqlite3_vtab_collation() requires a valid sqlite3_index_info*
    // which is typically used in virtual table implementations.
    // We'll create a minimal structure to demonstrate the API call,
    // but it won't be functional without proper virtual table setup.
    sqlite3_index_info idx_info = {0};
    const char *collation = sqlite3_vtab_collation(&idx_info, 0);
    if (collation && out_file) {
        // collation might be null, but we still call the API
        fwrite(collation, strlen(collation ? collation : ""), 1, out_file);
    }

    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}