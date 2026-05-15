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
//<ID> 278
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 23
//<Quality> {"density":20,"unique_branches":{"btreeNext":[[75988,11,75988,13,0,0,4,0]],"sqlite3_uri_int64":[[182371,7,182371,8,0,0,4,1]],"fillInUnixFile":[[43532,28,43532,54,0,0,4,0]],"sqlite3_get_table":[[150010,7,150010,20,0,0,4,0]],"fileHasMoved":[[39346,10,39346,26,0,0,4,1]],"unixOpen":[[44183,7,44183,30,0,0,4,0]],"sqlite3VdbeDeleteAuxData":[[87638,35,87638,69,0,0,4,0]],"sqlite3ParseUri":[[180701,11,180701,18,0,0,4,0],[180701,22,180701,47,0,0,4,0]],"btreeLast":[[75376,9,75376,22,0,0,4,1]],"uriParameter":[[180801,34,180801,46,0,0,4,0],[180804,9,180804,13,0,0,4,1]],"sqlite3VdbeExec":[[97733,11,97733,24,0,0,4,0],[98597,7,98597,22,0,0,4,0]],"moveToRightmost":[[75336,9,75336,11,0,0,4,0]],"sqlite3PagerOpen":[[61209,12,61209,14,0,0,4,0]],"sqlite3_get_table_cb":[[149899,7,149899,17,0,0,4,1],[149904,7,149904,34,0,0,4,0],[149908,9,149908,17,0,0,4,1],[149915,7,149915,17,0,0,4,1],[149922,13,149922,34,0,0,4,1]],"findCreateFileMode":[[43921,13,43921,36,0,0,4,0],[43927,9,43927,10,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_uri_int64","sqlite3_db_filename","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_index","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_get_table","sqlite3_free_table","sqlite3_free","sqlite3_serialize","sqlite3_get_auxdata","sqlite3_set_auxdata","sqlite3_result_int","sqlite3_get_auxdata","sqlite3_set_auxdata","sqlite3_result_int","sqlite3_create_function_v2","sqlite3_exec","sqlite3_commit_hook","sqlite3_snapshot_open","sqlite3_snapshot_free","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_uri_int64","sqlite3_db_filename","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_index","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_get_table","sqlite3_free_table","sqlite3_free","sqlite3_serialize","sqlite3_result_int","sqlite3_set_auxdata","sqlite3_get_auxdata","sqlite3_result_int","sqlite3_set_auxdata","sqlite3_get_auxdata","sqlite3_create_function_v2","sqlite3_exec","sqlite3_commit_hook","sqlite3_snapshot_open","sqlite3_snapshot_free","sqlite3_free","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<180) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create database connection with URI
    sqlite3 *db = nullptr;
    const char *uri = "file:test.db?vfs=unix-none";
    int rc = sqlite3_open_v2(uri, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Use sqlite3_uri_int64 to read a URI parameter
    sqlite3_int64 cache_size = sqlite3_uri_int64(sqlite3_db_filename(db, fuzz_str_3), fuzz_str_1, fuzz_int64_t_2);

    // 3. Create a simple table
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT)";
    sqlite3_exec(db, fuzz_str_4, nullptr, nullptr, nullptr);

    // 4. Prepare a statement with named parameters
    const char *insert_sql = "INSERT INTO test (name) VALUES (:name)";
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK) {
        // 5. Use sqlite3_bind_parameter_count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 6. Use sqlite3_bind_parameter_index to get index of named parameter
        int name_index = sqlite3_bind_parameter_index(stmt, fuzz_str_5);
        
        if (name_index > 0 && param_count > 0) {
            // Read some data from input file for binding
            char buffer[256];
            size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, in_file);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                sqlite3_bind_text(stmt, name_index, buffer, -1, SQLITE_TRANSIENT);
            }
        }
        
        // Execute the statement
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // 7. Use sqlite3_get_table to retrieve data
    char **results = nullptr;
    int rows = 0, cols = 0;
    char *errmsg = nullptr;
    const char *select_sql = "SELECT * FROM test";
    sqlite3_get_table(db, fuzz_str_6, &results, &rows, &cols, &errmsg);
    
    // Clean up table results
    if (results) {
        sqlite3_free_table(results);
    }
    if (errmsg) {
        sqlite3_free(errmsg);
    }

    // 8. Use sqlite3_serialize to get database serialization
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized_data = sqlite3_serialize(db, fuzz_str_7, &serialized_size, fuzz_uint32_t_8);
    
    // 9. Create a user-defined function to demonstrate sqlite3_get_auxdata
    struct FuncContext {
        int counter;
    };
    
    auto func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        // Use sqlite3_get_auxdata
        FuncContext *fc = static_cast<FuncContext*>(sqlite3_get_auxdata(ctx, 0));
        if (!fc) {
            fc = new FuncContext{0};
            sqlite3_set_auxdata(ctx, 0, fc, [](void *p) { delete static_cast<FuncContext*>(p); });
        }
        fc->counter++;
        sqlite3_result_int(ctx, fc->counter);
    };
    
    sqlite3_create_function_v2(db, fuzz_str_9, fuzz_int32_t_10, fuzz_int32_t_11, nullptr, 
                              func, nullptr, nullptr, nullptr);
    
    // Call the function to trigger auxdata usage
    sqlite3_exec(db, fuzz_str_12, nullptr, nullptr, nullptr);

    // 10. Set a commit hook
    auto commit_hook = [](void *user_data) -> int {
        return 0; // Allow commit
    };
    sqlite3_commit_hook(db, commit_hook, nullptr);

    // 11. For sqlite3_vtab_in - create a virtual table if possible
    // This is simplified - in reality would need full virtual table implementation
    struct DummyVTabInfo {
        sqlite3_index_info *info;
    } dummy_vtab;
    
    // Just call the function with dummy data (won't actually work without proper vtab)
    // sqlite3_vtab_in(nullptr, 0, 0); // Can't call without proper context

    // 12. Use sqlite3_snapshot_free (requires a snapshot first)
    sqlite3_snapshot *snapshot = nullptr;
    // Try to get a snapshot (may fail if no write transaction)
    if (sqlite3_snapshot_open(db, fuzz_str_13, nullptr) == SQLITE_OK) {
        // In real usage, would use the snapshot
        sqlite3_snapshot_free(snapshot);
    }

    // Clean up serialized data
    if (serialized_data) {
        sqlite3_free(serialized_data);
    }

    // 13. Create output file for demonstration
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to output file
        fwrite("SQLite fuzzing output", 1, 21, out_file);
        assert_file_closed(&out_file);;
    }

    // Close database
    sqlite3_close(db);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}