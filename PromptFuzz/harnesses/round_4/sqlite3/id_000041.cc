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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"sqlite3Close":[[178872,9,178872,23,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_pointer","sqlite3_bind_null","sqlite3_step","sqlite3_stmt_busy","sqlite3_reset","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_wal_checkpoint_v2","sqlite3_exec","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_create_function","sqlite3_create_module","sqlite3_str_new","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_pointer","sqlite3_step","sqlite3_stmt_busy","sqlite3_reset","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_wal_checkpoint_v2","sqlite3_exec","sqlite3_free","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified SQLite APIs:

*/


// Custom function implementation
static void  id_000041_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        sqlite3_int64 val = sqlite3_value_int64(argv[0]);
        sqlite3_result_int64(context, val * 2);
    } else {
        sqlite3_result_int64(context, 0);
    }
}

// Custom module implementation
static int  id_000041_dummy_xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv,
                        sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int  id_000041_dummy_xConnect(sqlite3* db, void* pAux, int argc, const char* const* argv,
                         sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static sqlite3_module  id_000041_dummy_module = {
    0,                          // iVersion
     id_000041_dummy_xCreate,              // xCreate
     id_000041_dummy_xConnect,             // xConnect
    0,                          // xBestIndex
    0,                          // xDisconnect
    0,                          // xDestroy
    0,                          // xOpen
    0,                          // xClose
    0,                          // xFilter
    0,                          // xNext
    0,                          // xEof
    0,                          // xColumn
    0,                          // xRowid
    0,                          // xUpdate
    0,                          // xBegin
    0,                          // xSync
    0,                          // xCommit
    0,                          // xRollback
    0,                          // xFindFunction
    0,                          // xRename
    0,                          // xSavepoint
    0,                          // xRelease
    0,                          // xRollbackTo
    0                           // xShadowName
};

// Callback for sqlite3_exec
static int  id_000041_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<207) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* err_msg = nullptr;
    int rc;
    
    // 1. Create a temporary file with input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open database (in memory)
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create custom function
    rc = sqlite3_create_function(db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, nullptr,
                                  id_000041_custom_function, nullptr, nullptr);
    
    // 4. Create module
    rc = sqlite3_create_module(db, fuzz_str_4, & id_000041_dummy_module, nullptr);
    
    // 5. Create sqlite3_str and use it
    sqlite3_str* str = sqlite3_str_new(db);
    if (str) {
        // Use first part of input as string to append
        size_t str_len = size > 100 ? 100 : size;
        sqlite3_str_append(str, (const char*)data, str_len);
        char* result_str = sqlite3_str_finish(str);
        if (result_str) {
            // Could use result_str for something
            sqlite3_free(result_str);
        }
    }
    
    // 6. Create table and insert data using input
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_5, nullptr, nullptr, &err_msg);
    if (err_msg) sqlite3_free(err_msg);
    
    // 7. Prepare statement with input data
    const char* insert_sql = "INSERT INTO test VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_6, -1, &stmt, nullptr);
    
    if (stmt) {
        // Bind values using input data
        int id = data[0];
        size_t text_len = size > 50 ? 50 : size;
        const char* text_data = (const char*)data;
        
        // Bind int
        sqlite3_bind_int(stmt, fuzz_int32_t_7, id);
        
        // Bind text (use first part of input)
        sqlite3_bind_text(stmt, fuzz_int32_t_8, text_data, text_len, SQLITE_TRANSIENT);
        
        // Bind pointer (use sqlite3_str as pointer data)
        if (str) {
            sqlite3_bind_pointer(stmt, fuzz_int32_t_10, str, fuzz_str_9, nullptr);
        } else {
            sqlite3_bind_null(stmt, fuzz_int32_t_11);
        }
        
        // Execute statement
        rc = sqlite3_step(stmt);
        
        // Check if statement is busy
        int busy = sqlite3_stmt_busy(stmt);
        
        // Reset statement
        sqlite3_reset(stmt);
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // 8. Use clientdata functions
    int* client_data = (int*)malloc(sizeof(int));
    if (client_data) {
        *client_data = 42;
        sqlite3_set_clientdata(db, fuzz_str_12, client_data, free);
        
        void* retrieved = sqlite3_get_clientdata(db, fuzz_str_13);
        // retrieved should be client_data
    }
    
    // 9. Execute WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, fuzz_str_14, fuzz_int32_t_15, &pnLog, &pnCkpt);
    
    // 10. Execute some SQL using the input as SQL commands
    // Use first part of input as potential SQL (limited to avoid buffer overflows)
    size_t sql_len = size > 1000 ? 1000 : size;
    char* sql_cmd = (char*)malloc(sql_len + 1);
    if (sql_cmd) {
        memcpy(sql_cmd, data, sql_len);
        sql_cmd[sql_len] = '\0';
        
        // Try to execute as SQL
        rc = sqlite3_exec(db, sql_cmd,  id_000041_exec_callback, nullptr, &err_msg);
        if (err_msg) sqlite3_free(err_msg);
        free(sql_cmd);
    }
    
    // 11. Test vtab_in functions (though our dummy module doesn't implement them)
    // We'll create a simple test with sqlite3_value
    if (size > sizeof(sqlite3_int64)) {
        sqlite3_int64 int_val = *reinterpret_cast<const sqlite3_int64*>(data);
        // Create a value from the int (simulated)
        // Note: In real usage, we'd need to create a sqlite3_value from the data
        // For fuzzing purposes, we'll skip complex value construction
    }
    
    // 12. Close database
    sqlite3_close(db);
    
    // 13. Clean up file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}