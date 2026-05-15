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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 198, nr_unique_branch: 31
//<Quality> {"density":22,"unique_branches":{"yy_reduce":[[174395,7,174395,15,0,0,4,0],[174604,7,174604,28,0,0,4,0],[174606,9,174606,28,0,0,4,1],[175077,7,175077,15,0,0,4,0],[14098,27,14098,28,276,0,4,0]],"analyzeOneTable":[[118308,26,118308,30,0,0,4,0],[118315,9,118315,17,0,0,4,1],[118316,9,118316,31,0,0,4,0],[118317,9,118317,18,0,0,4,1],[118324,18,118324,35,0,0,4,1],[118413,9,118413,19,0,0,4,0],[118417,11,118417,23,0,0,4,1],[118431,11,118431,22,0,0,4,0],[118431,26,118431,42,0,0,4,0],[118438,16,118438,26,0,0,4,0],[118438,16,118438,26,0,0,4,1],[118460,16,118460,26,0,0,4,0],[118460,16,118460,26,0,0,4,1],[118500,11,118500,29,0,0,4,1],[118598,22,118598,34,0,0,4,1],[18627,31,18627,54,20,0,4,0]],"yy_destructor":[[172768,5,172768,13,0,0,4,0]],"sqlite3SrcListAppendList":[[125186,7,125186,9,0,0,4,0],[125188,9,125188,16,0,0,4,1]],"sqlite3Pragma":[[137695,7,137695,16,0,0,4,0]],"sqlite3GetInt32":[[35347,7,35347,19,0,0,4,0],[35391,7,35391,10,0,0,4,0]],"sqlite3VdbeReleaseRegisters":[[85467,28,85467,54,0,0,4,0]],"analyzeVdbeCommentIndexWithColumnName":[[118211,13,118211,23,0,0,4,1],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1]]},"library_calls":["sqlite3_create_filename","sqlite3_uri_parameter","sqlite3_uri_key","sqlite3_open_v2","sqlite3_free_filename","sqlite3_busy_handler","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_double","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_value_double","sqlite3_value_double","sqlite3_result_double","sqlite3_value_double","sqlite3_value_double","sqlite3_result_double","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_column_origin_name16","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_close","sqlite3_mprintf","sqlite3_free","sqlite3_status64","sqlite3_free_filename","sqlite3_close_v2"],"critical_calls":["sqlite3_create_filename","sqlite3_uri_parameter","sqlite3_uri_key","sqlite3_open_v2","sqlite3_busy_handler","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_double","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_result_double","sqlite3_value_double","sqlite3_value_double","sqlite3_result_double","sqlite3_value_double","sqlite3_value_double","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_column_origin_name16","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_close","sqlite3_mprintf","sqlite3_free","sqlite3_status64","sqlite3_free_filename","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Use input data for various operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_blob *blob = nullptr;
    char *err_msg = nullptr;
    int rc;

    // 1. Create filename using input data
    const char *db_name = reinterpret_cast<const char*>(data);
    size_t db_name_len = strnlen(db_name, size);
    if (db_name_len >= size) {
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    sqlite3_filename filename = sqlite3_create_filename(
        db_name, "journal", "wal", 0, nullptr);
    
    if (!filename) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Extract URI parameters from created filename
    const char *param = sqlite3_uri_parameter(filename, "mode");
    const char *key = sqlite3_uri_key(filename, 0);

    // 3. Open database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        sqlite3_free_filename(filename);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Set busy handler
    sqlite3_busy_handler(db, [](void*, int) -> int { return 0; }, nullptr);

    // 5. Execute SQL commands from input data
    const char *sql = reinterpret_cast<const char*>(data + db_name_len + 1);
    size_t sql_len = size - db_name_len - 1;
    if (sql_len > 0) {
        // Limit SQL length to avoid buffer overreads
        size_t safe_sql_len = sql_len > 1024 ? 1024 : sql_len;
        char *safe_sql = (char*)malloc(safe_sql_len + 1);
        if (safe_sql) {
            memcpy(safe_sql, sql, safe_sql_len);
            safe_sql[safe_sql_len] = '\0';
            
            rc = sqlite3_exec(db, safe_sql, nullptr, nullptr, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            free(safe_sql);
        }
    }

    // 6. Prepare and bind statements
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, name TEXT, value REAL, data BLOB);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    
    const char *insert_sql = "INSERT INTO test(name, value, data) VALUES(?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind text - use part of input data
        const char *text_data = reinterpret_cast<const char*>(data);
        size_t text_len = size > 100 ? 100 : size;
        sqlite3_bind_text(stmt, 1, text_data, text_len, SQLITE_TRANSIENT);
        
        // Bind double - extract from input bytes
        double dbl_val = 0.0;
        if (size >= sizeof(double) + text_len) {
            memcpy(&dbl_val, data + text_len, sizeof(double));
        }
        sqlite3_bind_double(stmt, 2, dbl_val);
        
        // Bind blob - use remaining input data
        const void *blob_data = data;
        int blob_len = size > 200 ? 200 : size;
        sqlite3_bind_blob(stmt, 3, blob_data, blob_len, SQLITE_TRANSIENT);
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // 7. Create custom function
    auto custom_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        if (argc == 2) {
            double a = sqlite3_value_double(argv[0]);
            double b = sqlite3_value_double(argv[1]);
            sqlite3_result_double(ctx, a + b);
        }
    };
    
    rc = sqlite3_create_function_v2(
        db, "custom_add", 2, SQLITE_UTF8, nullptr,
        custom_func, nullptr, nullptr, nullptr);

    // 8. Query data
    const char *select_sql = "SELECT custom_add(value, 1.0), name FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            double result = sqlite3_column_double(stmt, 0);
            const void *col_name = sqlite3_column_origin_name16(stmt, 1);
            // Do something with results
        }
        sqlite3_finalize(stmt);
    }

    // 9. Open blob
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    if (rowid > 0) {
        rc = sqlite3_blob_open(db, "main", "test", "data", rowid, 0, &blob);
        if (rc == SQLITE_OK && blob) {
            int blob_size = sqlite3_blob_bytes(blob);
            sqlite3_blob_close(blob);
        }
    }

    // 10. Use mprintf and free
    char *formatted = sqlite3_mprintf("Test %d", 123);
    if (formatted) {
        sqlite3_free(formatted);
    }

    // 11. Use status64
    sqlite3_int64 current = 0, highwater = 0;
    sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 0);

    // 12. Cleanup
    sqlite3_free_filename(filename);
    sqlite3_close_v2(db);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}