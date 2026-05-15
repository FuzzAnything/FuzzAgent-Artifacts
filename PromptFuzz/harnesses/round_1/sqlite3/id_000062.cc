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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"vfsUnlink":[[26025,7,26025,14,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_create_collation16","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_type","sqlite3_column_text","sqlite3_strglob","sqlite3_strlike","sqlite3_next_stmt","sqlite3_finalize","sqlite3_create_function","sqlite3_vtab_nochange","sqlite3_value_type","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_value_type","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_thread_cleanup","sqlite3_vfs_unregister","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_create_collation16","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_type","sqlite3_column_text","sqlite3_strglob","sqlite3_strlike","sqlite3_next_stmt","sqlite3_finalize","sqlite3_create_function","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_value_type","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_value_type","sqlite3_vtab_nochange","sqlite3_thread_cleanup","sqlite3_vfs_unregister","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Main database handle
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt *next_stmt = nullptr;
    int rc = 0;
    
    // 1. Open database from memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Create custom collation
    rc = sqlite3_create_collation16(
        db,
        L"custom_collation",
        SQLITE_UTF16,
        nullptr,
        [](void *pArg, int n1, const void *s1, int n2, const void *s2) -> int {
            // Simple byte comparison collation
            int min_len = n1 < n2 ? n1 : n2;
            int cmp = memcmp(s1, s2, min_len);
            if (cmp == 0) {
                return n1 - n2;
            }
            return cmp;
        }
    );
    
    // 3. Prepare a simple statement
    const char *sql = "SELECT 1, 'test'";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 4. Execute the statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Get column count
            int cols = sqlite3_column_count(stmt);
            
            for (int i = 0; i < cols; i++) {
                // 5. Use sqlite3_value_type on column values
                sqlite3_value *value = sqlite3_column_value(stmt, i);
                if (value) {
                    int val_type = sqlite3_value_type(value);
                    
                    // 6. String pattern matching
                    const char *col_text = (const char*)sqlite3_column_text(stmt, i);
                    if (col_text) {
                        // Use sqlite3_strglob for pattern matching
                        int glob_match = sqlite3_strglob("*test*", col_text);
                        
                        // Use sqlite3_strlike for LIKE pattern matching
                        int like_match = sqlite3_strlike("%test%", col_text, '\\');
                    }
                }
            }
        }
        
        // 7. Get next statement (though there's only one)
        next_stmt = sqlite3_next_stmt(db, stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // 8. Create a custom function context
    sqlite3_value *test_value = nullptr;
    
    // 9. Simulate a virtual table no-change check
    // sqlite3_vtab_nochange needs a sqlite3_context, so we create a custom function
    rc = sqlite3_create_function(
        db,
        "test_func",
        1,
        SQLITE_UTF8,
        nullptr,
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // 10. Check vtab nochange (will return 0 as we're not in vtab context)
            int nochange = sqlite3_vtab_nochange(ctx);
            
            // 11. Use sqlite3_value_type on the argument
            if (argc > 0) {
                int arg_type = sqlite3_value_type(argv[0]);
                
                // 12. Simulate out of memory condition
                if (arg_type == SQLITE_NULL) {
                    sqlite3_result_error_nomem(ctx);
                    return;
                }
                
                // Return the type as integer
                sqlite3_result_int(ctx, arg_type);
            }
        },
        nullptr,
        nullptr
    );
    
    // 13. Call thread cleanup
    sqlite3_thread_cleanup();
    
    // 14. Unregister a VFS (try to unregister null)
    sqlite3_vfs_unregister(nullptr);
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}